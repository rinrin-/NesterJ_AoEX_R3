/////////////////////////////////////////////////////////////////////
// Mapper 252
void NES_mapper252_Init()
{
	g_NESmapper.Reset = NES_mapper252_Reset;
	g_NESmapper.MemoryWrite = NES_mapper252_MemoryWrite;
	g_NESmapper.HSync = NES_mapper252_HSync;
	//g_NESmapper.Clock = NES_mapper252_Clock;
}

void NES_mapper252_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper252.regs[i] = i;
	}
	g_NESmapper.Mapper252.regs[8] = 0;
	
	g_NESmapper.Mapper252.irq_enable = 0;
	g_NESmapper.Mapper252.irq_counter = 0;
	g_NESmapper.Mapper252.irq_latch = 0;
	g_NESmapper.Mapper252.irq_clock = 0;
	g_NESmapper.Mapper252.irq_occur = 0;

	//SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
	g_NESmapper.SetPROM_32K_Banks(0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1);
	
	//SetVROM_8K_Bank( 0 );
	g_NESmapper.SetVROM_8K_Bank(0);
	
	
	g_PPU.vram_write_protect = 0;
	//g_PPU.bChrLatch = 1;

	//nes->SetRenderMethod( NES::POST_RENDER );ÐèÒªÐÞ¸Ä
}


void NES_mapper252_MemoryWrite(uint32 addr, uint8 data)
{	
	if( (addr & 0xF000) == 0x8000 ) {
		//SetPROM_8K_Bank( 4, data );
		g_NESmapper.SetPROM_8K_Bank( 4, data );
		return;
	}
	if( (addr & 0xF000) == 0xA000 ) {
		g_NESmapper.SetPROM_8K_Bank( 5, data );
		return;
	}
	switch( addr & 0xF00C ) {
		case 0xB000:
			g_NESmapper.Mapper252.regs[0] = (g_NESmapper.Mapper252.regs[0] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 0, g_NESmapper.Mapper252.regs[0] );
			g_NESmapper.SetVROM_1K_Bank( 0, g_NESmapper.Mapper252.regs[0] );
			break;	
		case 0xB004:
			g_NESmapper.Mapper252.regs[0] = (g_NESmapper.Mapper252.regs[0] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 0, reg[0] );
			g_NESmapper.SetVROM_1K_Bank( 0, g_NESmapper.Mapper252.regs[0] );
			break;
		case 0xB008:
			g_NESmapper.Mapper252.regs[1] = (g_NESmapper.Mapper252.regs[1] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 1, reg[1] );			
			g_NESmapper.SetVROM_1K_Bank( 1, g_NESmapper.Mapper252.regs[1] );
			break;
		case 0xB00C:
			g_NESmapper.Mapper252.regs[1] = (g_NESmapper.Mapper252.regs[1] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 1, reg[1] );
			g_NESmapper.SetVROM_1K_Bank( 1, g_NESmapper.Mapper252.regs[1] );
			break;

		case 0xC000:
			g_NESmapper.Mapper252.regs[2] = (g_NESmapper.Mapper252.regs[2] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 2, reg[2] );
			g_NESmapper.SetVROM_1K_Bank( 2, g_NESmapper.Mapper252.regs[2] );
			break;
		case 0xC004:
			g_NESmapper.Mapper252.regs[2] = (g_NESmapper.Mapper252.regs[2] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 2, g_NESmapper.Mapper252.regs[2] );
			g_NESmapper.SetVROM_1K_Bank( 2, g_NESmapper.Mapper252.regs[2] );
			break;
		case 0xC008:
			g_NESmapper.Mapper252.regs[3] = (g_NESmapper.Mapper252.regs[3] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 3, g_NESmapper.Mapper252.regs[3] );
			g_NESmapper.SetVROM_1K_Bank( 3, g_NESmapper.Mapper252.regs[3] );
			break;
		case 0xC00C:
			g_NESmapper.Mapper252.regs[3] = (g_NESmapper.Mapper252.regs[3] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 3, g_NESmapper.Mapper252.regs[3] );
			g_NESmapper.SetVROM_1K_Bank( 3, g_NESmapper.Mapper252.regs[3] );
			break;

		case 0xD000:
			g_NESmapper.Mapper252.regs[4] = (g_NESmapper.Mapper252.regs[4] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 4, g_NESmapper.Mapper252.regs[4] );
			g_NESmapper.SetVROM_1K_Bank( 4, g_NESmapper.Mapper252.regs[4] );
			break;
		case 0xD004:
			g_NESmapper.Mapper252.regs[4] = (g_NESmapper.Mapper252.regs[4] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 4, g_NESmapper.Mapper252.regs[4] );
			g_NESmapper.SetVROM_1K_Bank( 4, g_NESmapper.Mapper252.regs[4] );
			break;
		case 0xD008:
			g_NESmapper.Mapper252.regs[5] = (g_NESmapper.Mapper252.regs[5] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 5, g_NESmapper.Mapper252.regs[5] );
			g_NESmapper.SetVROM_1K_Bank( 5, g_NESmapper.Mapper252.regs[5] );
			break;
		case 0xD00C:
			g_NESmapper.Mapper252.regs[5] = (g_NESmapper.Mapper252.regs[5] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 5, g_NESmapper.Mapper252.regs[5] );
			g_NESmapper.SetVROM_1K_Bank( 5, g_NESmapper.Mapper252.regs[5] );
			break;

		case 0xE000:
			g_NESmapper.Mapper252.regs[6] = (g_NESmapper.Mapper252.regs[6] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 6, reg[6] );
			g_NESmapper.SetVROM_1K_Bank( 6, g_NESmapper.Mapper252.regs[6] );
			break;
		case 0xE004:
			g_NESmapper.Mapper252.regs[6] = (g_NESmapper.Mapper252.regs[6] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 6, reg[6] );
			g_NESmapper.SetVROM_1K_Bank( 6, g_NESmapper.Mapper252.regs[6] );
			break;
		case 0xE008:
			g_NESmapper.Mapper252.regs[7] = (g_NESmapper.Mapper252.regs[7] & 0xF0) | (data & 0x0F);
			//SetVROM_1K_Bank( 7, reg[7] );
			g_NESmapper.SetVROM_1K_Bank( 7, g_NESmapper.Mapper252.regs[7] );
			break;
		case 0xE00C:
			g_NESmapper.Mapper252.regs[7] = (g_NESmapper.Mapper252.regs[7] & 0x0F) | ((data & 0x0F) << 4);
			//SetVROM_1K_Bank( 7, reg[7] );
			g_NESmapper.SetVROM_1K_Bank( 7, g_NESmapper.Mapper252.regs[7] );
			break;

		case 0xF000:
			g_NESmapper.Mapper252.irq_latch = (g_NESmapper.Mapper252.irq_latch & 0xF0) | (data & 0x0F);
			g_NESmapper.Mapper252.irq_occur = 0;
			break;
		case 0xF004:
			g_NESmapper.Mapper252.irq_latch = (g_NESmapper.Mapper252.irq_latch & 0x0F) | ((data & 0x0F) << 4);
			g_NESmapper.Mapper252.irq_occur = 0;
			break;

		case 0xF008:
			g_NESmapper.Mapper252.irq_enable = data & 0x03;
			if( g_NESmapper.Mapper252.irq_enable & 0x02 ) {
				g_NESmapper.Mapper252.irq_counter = g_NESmapper.Mapper252.irq_latch;
				g_NESmapper.Mapper252.irq_clock = 0;
			}
			g_NESmapper.Mapper252.irq_occur = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;

		case 0xF00C:
			g_NESmapper.Mapper252.irq_enable = (g_NESmapper.Mapper252.irq_enable & 0x01) * 3;
			g_NESmapper.Mapper252.irq_occur = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
	}	
}

void NES_mapper252_Clock(int cycles )
{
	if( g_NESmapper.Mapper252.irq_enable & 0x02 ) {
		if( (g_NESmapper.Mapper252.irq_clock+=cycles) >= 0x72 ) {
			g_NESmapper.Mapper252.irq_clock -= 0x72;
			if( g_NESmapper.Mapper252.irq_counter == 0xFF ) {
				g_NESmapper.Mapper252.irq_occur = 0xFF;
				g_NESmapper.Mapper252.irq_counter = g_NESmapper.Mapper252.irq_latch;
				g_NESmapper.Mapper252.irq_enable = (g_NESmapper.Mapper252.irq_enable & 0x01) * 3;
				nes6502_SetIRQ( IRQ_MAPPER );	
			} else {
				g_NESmapper.Mapper252.irq_counter++;
			}
		}
	}
}


void NES_mapper252_HSync(uint32 scanline)
{
	if( g_NESmapper.Mapper252.irq_enable & 0x02 ) 
	{
		if( g_NESmapper.Mapper252.irq_counter == 0xFF ) 
		{
			g_NESmapper.Mapper252.irq_occur = 0xFF;
			g_NESmapper.Mapper252.irq_counter = g_NESmapper.Mapper252.irq_latch;
			g_NESmapper.Mapper252.irq_enable = (g_NESmapper.Mapper252.irq_enable & 0x01) * 3;
		}
		else 
		{
	      g_NESmapper.Mapper252.irq_counter++;
		}
  }
  if( g_NESmapper.Mapper252.irq_occur ) 
  {
    nes6502_SetIRQ( IRQ_MAPPER );
  }
}

/////////////////////////////////////////////////////////////////////

