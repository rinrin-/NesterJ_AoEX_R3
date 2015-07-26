
/////////////////////////////////////////////////////////////////////
// Mapper 27
void NES_mapper27_Init()
{
	g_NESmapper.Reset = NES_mapper27_Reset;
	g_NESmapper.MemoryWrite = NES_mapper27_MemoryWrite;
	g_NESmapper.HSync = NES_mapper27_HSync;
}

void NES_mapper27_Reset()
{
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	int i;
	for( i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper27.regs[i] = i;
	}
	g_NESmapper.Mapper27.regs[8] = 0;
	g_NESmapper.Mapper27.irq_enable = 0;
	g_NESmapper.Mapper27.irq_counter = 0;
	g_NESmapper.Mapper27.irq_latch = 0;
	g_NESmapper.Mapper27.irq_clock = 0;
}

void NES_mapper27_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xF0CF ) {
		case	0x8000:
			if(g_NESmapper.Mapper27.regs[8] & 0x02) {
				g_NESmapper.set_CPU_bank6(data);
			} else {
				g_NESmapper.set_CPU_bank4(data);
			}
			break;
		case	0xA000:
			g_NESmapper.set_CPU_bank5(data);
			break;

		case	0x9000:
			data &= 0x03;
			if( data == 0 )	     g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			else if( data == 1 ) g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			else if( data == 2 ) g_NESmapper.set_mirroring(0,0,0,0);
			else		     g_NESmapper.set_mirroring(1,1,1,1);
			break;

		case 0x9002:
		case 0x9080:
			g_NESmapper.Mapper27.regs[8] = data;
			break;

		case 0xB000:
			g_NESmapper.Mapper27.regs[0] = (g_NESmapper.Mapper27.regs[0] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper27.regs[0]);
			break;
		case 0xB001:
			g_NESmapper.Mapper27.regs[0] = (g_NESmapper.Mapper27.regs[0] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank0(g_NESmapper.Mapper27.regs[0]);
			break;

		case 0xB002:
			g_NESmapper.Mapper27.regs[1] = (g_NESmapper.Mapper27.regs[1] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper27.regs[1]);
			break;
		case 0xB003:
			g_NESmapper.Mapper27.regs[1] = (g_NESmapper.Mapper27.regs[1] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank1(g_NESmapper.Mapper27.regs[1]);
			break;

		case 0xC000:
			g_NESmapper.Mapper27.regs[2] = (g_NESmapper.Mapper27.regs[2] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper27.regs[2]);
			break;
		case 0xC001:
			g_NESmapper.Mapper27.regs[2] = (g_NESmapper.Mapper27.regs[2] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank2(g_NESmapper.Mapper27.regs[2]);
			break;

		case 0xC002:
			g_NESmapper.Mapper27.regs[3] = (g_NESmapper.Mapper27.regs[3] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper27.regs[3]);
			break;
		case 0xC003:
			g_NESmapper.Mapper27.regs[3] = (g_NESmapper.Mapper27.regs[3] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank3(g_NESmapper.Mapper27.regs[3]);
			break;

		case 0xD000:
			g_NESmapper.Mapper27.regs[4] = (g_NESmapper.Mapper27.regs[4] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper27.regs[4]);
			break;
		case 0xD001:
			g_NESmapper.Mapper27.regs[4] = (g_NESmapper.Mapper27.regs[4] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank4(g_NESmapper.Mapper27.regs[4]);
			break;

		case 0xD002:
			g_NESmapper.Mapper27.regs[5] = (g_NESmapper.Mapper27.regs[5] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper27.regs[5]);
			break;
		case 0xD003:
			g_NESmapper.Mapper27.regs[5] = (g_NESmapper.Mapper27.regs[5] & 0x0F) | (data << 4);
			g_NESmapper.set_PPU_bank5(g_NESmapper.Mapper27.regs[5]);
			break;

		case 0xE000:
			g_NESmapper.Mapper27.regs[6] = (g_NESmapper.Mapper27.regs[6] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper27.regs[6]);
			break;
		case 0xE001:
			g_NESmapper.Mapper27.regs[6] = (g_NESmapper.Mapper27.regs[6] & 0x0F) | (data << 4);
			g_NESmapper.set_PPU_bank6(g_NESmapper.Mapper27.regs[6]);
			break;

		case 0xE002:
			g_NESmapper.Mapper27.regs[7] = (g_NESmapper.Mapper27.regs[7] & 0xFF0) | (data & 0x0F);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper27.regs[7]);
			break;
		case 0xE003:
			g_NESmapper.Mapper27.regs[7] = (g_NESmapper.Mapper27.regs[7] & 0x0F) | (data<< 4);
			g_NESmapper.set_PPU_bank7(g_NESmapper.Mapper27.regs[7]);
			break;

		case 0xF000:
			g_NESmapper.Mapper27.irq_latch = (g_NESmapper.Mapper27.irq_latch & 0xF0) | (data & 0x0F);
			break;
		case 0xF001:
			g_NESmapper.Mapper27.irq_latch = (g_NESmapper.Mapper27.irq_latch & 0x0F) | ((data & 0x0F) << 4);
			break;

		case 0xF003:
			g_NESmapper.Mapper27.irq_enable = (g_NESmapper.Mapper27.irq_enable & 0x01) * 3;
			g_NESmapper.Mapper27.irq_clock = 0;
			break;

		case 0xF002:
			g_NESmapper.Mapper27.irq_enable = data & 0x03;
			if( g_NESmapper.Mapper27.irq_enable & 0x02 ) {
				g_NESmapper.Mapper27.irq_counter = g_NESmapper.Mapper27.irq_latch;
				g_NESmapper.Mapper27.irq_clock = 0;
			}
			break;
	}
}

void NES_mapper27_HSync(uint32 scanline)
{
	if( g_NESmapper.Mapper27.irq_enable & 0x02 ) {
			if( g_NESmapper.Mapper27.irq_counter == 0xFF ) {
				g_NESmapper.Mapper27.irq_counter = g_NESmapper.Mapper27.irq_latch;
				NES6502_DoIRQ();
			} else {
				g_NESmapper.Mapper27.irq_counter++;
			}
	}
}
/////////////////////////////////////////////////////////////////////

