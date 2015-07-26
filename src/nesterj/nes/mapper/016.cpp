#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 16
void NES_mapper16_Init()
{
	g_NESmapper.Reset = NES_mapper16_Reset;
	g_NESmapper.MemoryWrite = NES_mapper16_MemoryWrite;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper16_MemoryWriteSaveRAM;
	g_NESmapper.MemoryReadSaveRAM = NES_mapper16_MemoryReadSaveRAM;
	g_NESmapper.HSync = NES_mapper16_HSync;
	g_NESmapper.SetBarcodeValue = NES_mapper16_SetBarcodeValue;
}

void NES_mapper16_Reset()
{
	g_NESmapper.Mapper16.patch = g_NESmapper.Mapper16.patch2 = 0;

	if(NES_crc32() == 0x3f15d20d) // Famicom Jump 2 - Saikyou no 7 Nin
	{
		g_NESmapper.Mapper16.patch2 =1;
		g_NESmapper.Mapper16.patch = 1;
	}
	if(NES_crc32() == 0x09499f4d || // DragonBallZ3-RessenJinzouNingen(J)
	        NES_crc32() == 0x170250de || // RokudenashiBlues(J)
	        NES_crc32() == 0x73ac76db || // SD Gundam Gaiden 2
	        NES_crc32() == 0x0be0a328 || // SD Gundam Wars
	        NES_crc32() == 0x19e81461 ||  //
			NES_crc32() == 0x6c6c2feb)		//DragonBall 3
	{
		g_NESmapper.Mapper16.patch2 = 1;
	}


	if(!g_NESmapper.Mapper16.patch)
	{
		nes6502_context context;
		NES6502_GetContext(&context);
		context.mem_page[3] = g_NESmapper.Mapper16.serial_out;
		NES6502_SetContext(&context);
		g_NESmapper.Mapper16.eeprom_status = 0x00;
		g_NESmapper.Mapper16.barcode_status = 0x00;
		g_NESmapper.Mapper16.barcode_enabled = 0;
		//memset(serial_out, eeprom_status | barcode_status, 0x2000);
		g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
	}

	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper16.regs[0] = 0;
	g_NESmapper.Mapper16.regs[1] = 0;
	g_NESmapper.Mapper16.regs[2] = 0;

	g_NESmapper.Mapper16.irq_enabled = 0;
	g_NESmapper.Mapper16.irq_counter = 0;
	g_NESmapper.Mapper16.irq_latch = 0;
}

void NES_mapper16_MemoryWrite(uint32 addr, uint8 data)
{
	//LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
	if(g_NESmapper.Mapper16.patch)
	{
		NES_mapper16_MemoryWrite3(addr, data);
	}
	else
	{
		NES_mapper16_MemoryWrite2(addr, data);
	}
}

void NES_mapper16_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	if(g_NESmapper.Mapper16.patch)
	{
		//    MemoryWrite3(addr, data);
	}
	else
	{
		NES_mapper16_MemoryWrite2(addr, data);
	}
}

void NES_mapper16_MemoryReadSaveRAM(uint32 addr)
{
	if(g_NESmapper.Mapper16.barcode_enabled && addr == 0x6000)
	{
		if(g_NESmapper.Mapper16.barcode_phase == 1)
		{
			if(++g_NESmapper.Mapper16.barcode_wait == 10)
			{
				g_NESmapper.Mapper16.barcode_wait = 0;
				g_NESmapper.Mapper16.barcode_status = g_NESmapper.Mapper16.barcode[g_NESmapper.Mapper16.barcode_pt];
				g_NESmapper.Mapper16.barcode_pt++;
				if(g_NESmapper.Mapper16.barcode_pt == g_NESmapper.Mapper16.barcode_pt_max)
				{
					g_NESmapper.Mapper16.barcode_status = 0x08;
					g_NESmapper.Mapper16.barcode_phase = 2;
				}
				g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
			}
		}
	}
}

void NES_mapper16_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper16.barcode_enabled)
	{
		if(g_NESmapper.Mapper16.barcode_phase == 0)
		{
			g_NESmapper.Mapper16.barcode_status = 0x08;
			if(++g_NESmapper.Mapper16.barcode_wait > 600)
			{
				g_NESmapper.Mapper16.barcode_wait = 0;
				g_NESmapper.Mapper16.barcode_phase = 1;
			}
			g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
		}
		else if(g_NESmapper.Mapper16.barcode_phase == 2)
		{
			g_NESmapper.Mapper16.barcode_status = 0x08;
			if(++g_NESmapper.Mapper16.barcode_wait > 600)
			{
				g_NESmapper.Mapper16.barcode_status = 0x00;
				g_NESmapper.Mapper16.barcode_enabled = 0;
			}
			g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
		}
	}

	if(g_NESmapper.Mapper16.irq_enabled)
	{
		if(g_NESmapper.Mapper16.irq_counter <= (unsigned int)((g_NESmapper.Mapper16.patch2)?113:114))
		{
			NES6502_DoPendingIRQ();
			//    NES6502_DoIRQ();
			g_NESmapper.Mapper16.irq_counter = 0;
			g_NESmapper.Mapper16.irq_enabled = 0;
		}
		else
		{
			g_NESmapper.Mapper16.irq_counter -= (unsigned int)((g_NESmapper.Mapper16.patch2)?113:114);
		}
	}
}

void NES_mapper16_MemoryWrite2(uint32 addr, uint8 data)
{
	switch(addr & 0x000F)
	{
	case 0x0000:
		{
			g_NESmapper.set_PPU_bank0(data);
		}
		break;

	case 0x0001:
		{
			g_NESmapper.set_PPU_bank1(data);
		}
		break;

	case 0x0002:
		{
			g_NESmapper.set_PPU_bank2(data);
		}
		break;

	case 0x0003:
		{
			g_NESmapper.set_PPU_bank3(data);
		}
		break;

	case 0x0004:
		{
			g_NESmapper.set_PPU_bank4(data);
		}
		break;

	case 0x0005:
		{
			g_NESmapper.set_PPU_bank5(data);
		}
		break;

	case 0x0006:
		{
			g_NESmapper.set_PPU_bank6(data);
		}
		break;

	case 0x0007:
		{
			g_NESmapper.set_PPU_bank7(data);
		}
		break;

	case 0x0008:
		{
			g_NESmapper.set_CPU_bank4(data*2+0);
			g_NESmapper.set_CPU_bank5(data*2+1);
		}
		break;

	case 0x0009:
		{
			data &= 0x03;
			if(data == 0)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 1)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 2)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0x000A:
		{
			g_NESmapper.Mapper16.irq_enabled = data & 0x01;
			g_NESmapper.Mapper16.irq_counter = g_NESmapper.Mapper16.irq_latch;
		}
		break;

	case 0x000B:
		{
			g_NESmapper.Mapper16.irq_latch = (g_NESmapper.Mapper16.irq_latch & 0xFF00) | data;
		}
		break;

	case 0x000C:
		{
			g_NESmapper.Mapper16.irq_latch = ((uint32)data << 8) | (g_NESmapper.Mapper16.irq_latch & 0x00FF);
		}
		break;

	case 0x000D:
		{
//	LOG("eep W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
			if(data == 0x80)
			{
				// reset
				g_NESmapper.Mapper16.eeprom_addr = 0x00;
				g_NESmapper.Mapper16.eeprom_mode = 0x00;
				g_NESmapper.Mapper16.eeprom_wbit = 0x00;
				g_NESmapper.Mapper16.eeprom_rbit = 0x00;
			}
			else if(g_NESmapper.Mapper16.eeprom_cmd[3] == 0x00 && g_NESmapper.Mapper16.eeprom_cmd[2] == 0x20 &&
			        g_NESmapper.Mapper16.eeprom_cmd[1] == 0x60 && g_NESmapper.Mapper16.eeprom_cmd[0] == 0x40 && data == 0x00)
			{
				// reset ?
			}
			else if(g_NESmapper.Mapper16.eeprom_cmd[3] == 0x00 && g_NESmapper.Mapper16.eeprom_cmd[2] == 0x40 &&
			        g_NESmapper.Mapper16.eeprom_cmd[1] == 0x60 && g_NESmapper.Mapper16.eeprom_cmd[0] == 0x20 && data == 0x00)
			{
				// start
				g_NESmapper.Mapper16.eeprom_wbit = 0x01;
				g_NESmapper.Mapper16.eeprom_rbit = 0x01;
				g_NESmapper.Mapper16.eeprom_data = 0;
				g_NESmapper.Mapper16.eeprom_mode = 0;
			}
			else if (g_NESmapper.Mapper16.eeprom_cmd[0] == 0x60 && data == 0xE0)
			{
				if(!g_NESmapper.Mapper16.eeprom_mode)
				{
					// sync
					g_NESmapper.Mapper16.eeprom_wbit = 0x01;
					g_NESmapper.Mapper16.eeprom_rbit = 0x01;
					g_NESmapper.Mapper16.eeprom_data = 0;
					g_NESmapper.Mapper16.eeprom_mode = 1;
					g_NESmapper.Mapper16.eeprom_status = 0x00;
					//memset(serial_out, eeprom_status | barcode_status, 0x2000);
					g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
				}
				else
				{
					// read
					g_NESmapper.Mapper16.eeprom_data = NES_ReadSaveRAM(g_NESmapper.Mapper16.eeprom_addr);
					if(g_NESmapper.Mapper16.eeprom_data & g_NESmapper.Mapper16.eeprom_rbit)
					{
						g_NESmapper.Mapper16.eeprom_status = 0x10;
						//memset(serial_out, eeprom_status | barcode_status, 0x2000);
						g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
					}
					else
					{
						g_NESmapper.Mapper16.eeprom_status = 0x00;
						//memset(serial_out, eeprom_status | barcode_status, 0x2000);
						g_NESmapper.Mapper16.serial_out[0] = g_NESmapper.Mapper16.eeprom_status | g_NESmapper.Mapper16.barcode_status;
					}
					g_NESmapper.Mapper16.eeprom_rbit <<= 1;
					g_NESmapper.Mapper16.eeprom_wbit = 0x00;
				}
			}
			else if(g_NESmapper.Mapper16.eeprom_cmd[1] == 0x00 && g_NESmapper.Mapper16.eeprom_cmd[0] == 0x20 && data == 0x00)
			{
				// write 0
				g_NESmapper.Mapper16.eeprom_data &= 0xFF - g_NESmapper.Mapper16.eeprom_wbit;
				if(g_NESmapper.Mapper16.eeprom_wbit == 0x80)
				{
					if(g_NESmapper.Mapper16.eeprom_mode)
					{
						NES_WriteSaveRAM(g_NESmapper.Mapper16.eeprom_addr, g_NESmapper.Mapper16.eeprom_data);
					}
					else
					{
						g_NESmapper.Mapper16.eeprom_addr = g_NESmapper.Mapper16.eeprom_data & 0x7F;
					}
					g_NESmapper.Mapper16.eeprom_wbit = 0x00;
				}
				else
				{
					g_NESmapper.Mapper16.eeprom_wbit <<= 1;
				}
				g_NESmapper.Mapper16.eeprom_rbit = 0x00;
			}
			else if(g_NESmapper.Mapper16.eeprom_cmd[3] == 0x00 && g_NESmapper.Mapper16.eeprom_cmd[2] == 0x40 &&
			        g_NESmapper.Mapper16.eeprom_cmd[1] == 0x60 && g_NESmapper.Mapper16.eeprom_cmd[0] == 0x40 && data == 0x00)
			{
				// write 1
				g_NESmapper.Mapper16.eeprom_data |= g_NESmapper.Mapper16.eeprom_wbit;
				if(g_NESmapper.Mapper16.eeprom_wbit == 0x80)
				{
					if(g_NESmapper.Mapper16.eeprom_mode)
					{
						NES_WriteSaveRAM(g_NESmapper.Mapper16.eeprom_addr, g_NESmapper.Mapper16.eeprom_data);
					}
					else
					{
						g_NESmapper.Mapper16.eeprom_addr = g_NESmapper.Mapper16.eeprom_data & 0x7F;
					}
					g_NESmapper.Mapper16.eeprom_wbit = 0x00;
				}
				else
				{
					g_NESmapper.Mapper16.eeprom_wbit <<= 1;
				}
				g_NESmapper.Mapper16.eeprom_rbit = 0x00;
			}
			g_NESmapper.Mapper16.eeprom_cmd[3] = g_NESmapper.Mapper16.eeprom_cmd[2];
			g_NESmapper.Mapper16.eeprom_cmd[2] = g_NESmapper.Mapper16.eeprom_cmd[1];
			g_NESmapper.Mapper16.eeprom_cmd[1] = g_NESmapper.Mapper16.eeprom_cmd[0];
			g_NESmapper.Mapper16.eeprom_cmd[0] = data;
		}
		break;
	}
}

void NES_mapper16_MemoryWrite3(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x8000:
	case 0x8001:
	case 0x8002:
	case 0x8003:
		{
			g_NESmapper.Mapper16.regs[0] = data & 0x01;
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper16.regs[0]*0x20+g_NESmapper.Mapper16.regs[2]*2+0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper16.regs[0]*0x20+g_NESmapper.Mapper16.regs[2]*2+1);
		}
		break;

	case 0x8004:
	case 0x8005:
	case 0x8006:
	case 0x8007:
		{
			g_NESmapper.Mapper16.regs[1] = data & 0x01;
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper16.regs[1]*0x20+0x1E);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper16.regs[1]*0x20+0x1F);
		}
		break;

	case 0x8008:
		{
			g_NESmapper.Mapper16.regs[2] = data;
			g_NESmapper.set_CPU_bank4(g_NESmapper.Mapper16.regs[0]*0x20+g_NESmapper.Mapper16.regs[2]*2+0);
			g_NESmapper.set_CPU_bank5(g_NESmapper.Mapper16.regs[0]*0x20+g_NESmapper.Mapper16.regs[2]*2+1);
			g_NESmapper.set_CPU_bank6(g_NESmapper.Mapper16.regs[1]*0x20+0x1E);
			g_NESmapper.set_CPU_bank7(g_NESmapper.Mapper16.regs[1]*0x20+0x1F);
		}
		break;

	case 0x8009:
		{
			data &= 0x03;
			if(data == 0)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			else if(data == 1)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else if(data == 2)
			{
				g_NESmapper.set_mirroring(0,0,0,0);
			}
			else
			{
				g_NESmapper.set_mirroring(1,1,1,1);
			}
		}
		break;

	case 0x800A:
		{
			g_NESmapper.Mapper16.irq_enabled = data & 0x01;
			g_NESmapper.Mapper16.irq_counter = g_NESmapper.Mapper16.irq_latch;
		}
		break;

	case 0x800B:
		{
			g_NESmapper.Mapper16.irq_latch = (g_NESmapper.Mapper16.irq_latch & 0xFF00) | data;
		}
		break;

	case 0x800C:
		{
			g_NESmapper.Mapper16.irq_latch = ((uint32)data << 8) | (g_NESmapper.Mapper16.irq_latch & 0x00FF);
		}
		break;

	case 0x800D:
		{
			//write protect
		}
		break;
	}
}

void NES_mapper16_SetBarcodeValue(uint32 value_low, uint32 value_high)
{
	boolean prefix_parity_type[10][6] =
	    {
	        {0,0,0,0,0,0}, {0,0,1,0,1,1},
	        {0,0,1,1,0,1}, {0,0,1,1,1,0},
	        {0,1,0,0,1,1}, {0,1,1,0,0,1},
	        {0,1,1,1,0,0}, {0,1,0,1,0,1},
	        {0,1,0,1,1,0}, {0,1,1,0,1,0}
	    };
	boolean data_left_odd[10][7] =
	    {
	        {0,0,0,1,1,0,1}, {0,0,1,1,0,0,1},
	        {0,0,1,0,0,1,1}, {0,1,1,1,1,0,1},
	        {0,1,0,0,0,1,1}, {0,1,1,0,0,0,1},
	        {0,1,0,1,1,1,1}, {0,1,1,1,0,1,1},
	        {0,1,1,0,1,1,1}, {0,0,0,1,0,1,1}
	    };
	boolean data_left_even[10][7] =
	    {
	        {0,1,0,0,1,1,1}, {0,1,1,0,0,1,1},
	        {0,0,1,1,0,1,1}, {0,1,0,0,0,0,1},
	        {0,0,1,1,1,0,1}, {0,1,1,1,0,0,1},
	        {0,0,0,0,1,0,1}, {0,0,1,0,0,0,1},
	        {0,0,0,1,0,0,1}, {0,0,1,0,1,1,1}
	    };
	boolean data_right[10][7] =
	    {
	        {1,1,1,0,0,1,0}, {1,1,0,0,1,1,0},
	        {1,1,0,1,1,0,0}, {1,0,0,0,0,1,0},
	        {1,0,1,1,1,0,0}, {1,0,0,1,1,1,0},
	        {1,0,1,0,0,0,0}, {1,0,0,0,1,0,0},
	        {1,0,0,1,0,0,0}, {1,1,1,0,1,0,0}
	    };
	uint32 i, j, pt = 0;

	if(value_high != 0) // normal barcode (13 characters)
	{
		// set numbers
		uint8 prefix, num[12];
		uint32 odd, even;
		prefix = (uint8)(value_high / 10000); value_high %= 10000;
		num[0] = (uint8)(value_high / 1000); value_high %= 1000;
		num[1] = (uint8)(value_high / 100); value_high %= 100;
		num[2] = (uint8)(value_high / 10); value_high %= 10;
		num[3] = (uint8)value_high;
		num[4] = (uint8)(value_low / 10000000)%10; value_low %= 10000000;
		num[5] = (uint8)(value_low / 1000000); value_low %= 1000000;
		num[6] = (uint8)(value_low / 100000); value_low %= 100000;
		num[7] = (uint8)(value_low / 10000); value_low %= 10000;
		num[8] = (uint8)(value_low / 1000); value_low %= 1000;
		num[9] = (uint8)(value_low / 100); value_low %= 100;
		num[10] = (uint8)(value_low / 10); value_low %= 10;
		// check character
		odd = (num[10]+num[8]+num[6]+num[4]+num[2]+num[0])*3;
		even = num[9]+num[7]+num[5]+num[3]+num[1]+prefix;
		num[11] = (10 - ((odd + even) % 10)) % 10;
		// left guard  bar
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		// 6 left characters
		for(i = 0; i < 6; i++)
		{
			if(prefix_parity_type[prefix][i])
			{
				// prity is even
				for(j = 0; j < 7; j++)
				{
					g_NESmapper.Mapper16.barcode[pt] = (data_left_even[num[i]][j] == 1) ? 0x00 : 0x08;
					pt++;
				}
			}
			else
			{
				// parity is odd
				for(j = 0; j < 7; j++)
				{
					g_NESmapper.Mapper16.barcode[pt] = (data_left_odd[num[i]][j] == 1) ? 0x00 : 0x08;
					pt++;
				}
			}
		}
		// center bar
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		// 5 right characters and 1 check character
		for(i = 0; i < 6; i++)
		{
			for(j = 0; j < 7; j++)
			{
				g_NESmapper.Mapper16.barcode[pt] = (data_right[num[i+6]][j] == 1) ? 0x00 : 0x08;
				pt++;
			}
		}
		// right guard bar
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		// right margin
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
	}
	else // short barcode (8 characters)
	{
		// set numbers
		uint8 num[8];
		uint32 odd, even;
		num[0] = (uint8)(value_low / 10000000)%10; value_low %= 10000000;
		num[1] = (uint8)(value_low / 1000000); value_low %= 1000000;
		num[2] = (uint8)(value_low / 100000); value_low %= 100000;
		num[3] = (uint8)(value_low / 10000); value_low %= 10000;
		num[4] = (uint8)(value_low / 1000); value_low %= 1000;
		num[5] = (uint8)(value_low / 100); value_low %= 100;
		num[6] = (uint8)(value_low / 10); value_low %= 10;
		// check character
		odd = (num[6]+num[4]+num[2]+num[0])*3;
		even = num[5]+num[3]+num[1];
		num[7] = (10 - ((odd + even) % 10)) % 10;
		// left guard  bar
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		// 4 left characters
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 7; j++)
			{
				g_NESmapper.Mapper16.barcode[pt] = (data_left_odd[num[i]][j] == 1) ? 0x00 : 0x08;
				pt++;
			}
		}
		// center bar
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		// 4 right characters and 1 check character
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 7; j++)
			{
				g_NESmapper.Mapper16.barcode[pt] = (data_right[num[i+4]][j] == 1) ? 0x00 : 0x08;
				pt++;
			}
		}
		// right guard bar
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
		g_NESmapper.Mapper16.barcode[pt] = 0x00; pt++;
		// right margin
		g_NESmapper.Mapper16.barcode[pt] = 0x08; pt++;
	}
	g_NESmapper.Mapper16.barcode_pt_max = pt;

	g_NESmapper.Mapper16.barcode_pt = 0;
	g_NESmapper.Mapper16.barcode_phase = 0;
	g_NESmapper.Mapper16.barcode_wait = 0;
	g_NESmapper.Mapper16.barcode_enabled = 1;
}
/////////////////////////////////////////////////////////////////////

#endif
