/////////////////////////////////////////////////////////////////////
// Mapper 115
void NES_mapper115_Init()
{
	g_NESmapper.Reset = NES_mapper115_Reset;
	g_NESmapper.MemoryWrite = NES_mapper115_MemoryWrite;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper115_MemoryWriteLow;
	g_NESmapper.HSync = NES_mapper115_HSync;
}

void NES_mapper115_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) g_NESmapper.Mapper115.reg[i] = 0x00;

	g_NESmapper.Mapper115.prg0 = g_NESmapper.Mapper115.prg0L = 0;
	g_NESmapper.Mapper115.prg1 = g_NESmapper.Mapper115.prg1L = 1;
	g_NESmapper.Mapper115.prg2 = g_NESmapper.num_8k_ROM_banks-2;
	g_NESmapper.Mapper115.prg3 = g_NESmapper.num_8k_ROM_banks-1;

	g_NESmapper.Mapper115.ExPrgSwitch = 0;
	g_NESmapper.Mapper115.ExChrSwitch = 0;

	NES_mapper115_SetBank_CPU();

	if( g_NESmapper.num_1k_VROM_banks ) {
		g_NESmapper.Mapper115.chr0 = 0;
		g_NESmapper.Mapper115.chr1 = 1;
		g_NESmapper.Mapper115.chr2 = 2;
		g_NESmapper.Mapper115.chr3 = 3;
		g_NESmapper.Mapper115.chr4 = 4;
		g_NESmapper.Mapper115.chr5 = 5;
		g_NESmapper.Mapper115.chr6 = 6;
		g_NESmapper.Mapper115.chr7 = 7;
		NES_mapper115_SetBank_PPU();
	} else {
		g_NESmapper.Mapper115.chr0 = 
		g_NESmapper.Mapper115.chr2 = 
		g_NESmapper.Mapper115.chr4 = 
		g_NESmapper.Mapper115.chr5 = 
		g_NESmapper.Mapper115.chr6 = 
		g_NESmapper.Mapper115.chr7 = 0;
		g_NESmapper.Mapper115.chr1 = 
		g_NESmapper.Mapper115.chr3 = 1;
	}

	g_NESmapper.Mapper115.irq_enable = 0;	// Disable
	g_NESmapper.Mapper115.irq_counter = 0;
	g_NESmapper.Mapper115.irq_latch = 0;
}

void NES_mapper115_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch ( addr ){
		case	0x6000:
			g_NESmapper.Mapper115.ExPrgSwitch = data; //data
			NES_mapper115_SetBank_CPU();
			break;
		case	0x6001:
			g_NESmapper.Mapper115.ExChrSwitch = data&0x1;
			NES_mapper115_SetBank_PPU();
			break;
	}
}

void NES_mapper115_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper115.reg[0] = data;
			NES_mapper115_SetBank_CPU();
			NES_mapper115_SetBank_PPU();
			break;
		case	0x8001:
			g_NESmapper.Mapper115.reg[1] = data;
			switch( g_NESmapper.Mapper115.reg[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper115.chr0 = data & 0xFE;
					g_NESmapper.Mapper115.chr1 = g_NESmapper.Mapper115.chr0+1;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x01:
					g_NESmapper.Mapper115.chr2 = data & 0xFE;
					g_NESmapper.Mapper115.chr3 = g_NESmapper.Mapper115.chr2+1;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x02:
					g_NESmapper.Mapper115.chr4 = data;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x03:
					g_NESmapper.Mapper115.chr5 = data;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x04:
					g_NESmapper.Mapper115.chr6 = data;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x05:
					g_NESmapper.Mapper115.chr7 = data;
					NES_mapper115_SetBank_PPU();
					break;
				case	0x06:
					g_NESmapper.Mapper115.prg0 = g_NESmapper.Mapper115.prg0L = data;
					NES_mapper115_SetBank_CPU();
					break;
				case	0x07:
					g_NESmapper.Mapper115.prg1 = g_NESmapper.Mapper115.prg1L = data;
					NES_mapper115_SetBank_CPU();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper115.reg[2] = data;
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if( data & 0x01 )	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}			
			break;
		case	0xA001:
			g_NESmapper.Mapper115.reg[3] = data;
			break;
		case	0xC000:
			g_NESmapper.Mapper115.reg[4] = data;
			g_NESmapper.Mapper115.irq_counter = data;
			g_NESmapper.Mapper115.irq_enable = 0xFF;
			break;
		case	0xC001:
			g_NESmapper.Mapper115.reg[5] = data;
			g_NESmapper.Mapper115.irq_latch = data;
			break;
		case	0xE000:
			g_NESmapper.Mapper115.reg[6] = data;
			g_NESmapper.Mapper115.irq_enable = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper115.reg[7] = data;
			g_NESmapper.Mapper115.irq_enable = 0xFF;
			break;
	}
}


void NES_mapper115_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) {
			if( g_NESmapper.Mapper115.irq_enable ) {
				if( !(g_NESmapper.Mapper115.irq_counter--) ) {
					g_NESmapper.Mapper115.irq_counter = g_NESmapper.Mapper115.irq_latch;
					nes6502_SetIRQ( IRQ_MAPPER );
				}
			}
		}
	}
}

void	NES_mapper115_SetBank_CPU()
{
	if( g_NESmapper.Mapper115.ExPrgSwitch & 0x80 ) {
		g_NESmapper.Mapper115.prg0 = ((g_NESmapper.Mapper115.ExPrgSwitch<<1)&0x1e);
		g_NESmapper.Mapper115.prg1 = g_NESmapper.Mapper115.prg0+1;

		g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper115.prg0, g_NESmapper.Mapper115.prg1, g_NESmapper.Mapper115.prg0+2, g_NESmapper.Mapper115.prg1+2);
	} else {
		g_NESmapper.Mapper115.prg0 = g_NESmapper.Mapper115.prg0L;
		g_NESmapper.Mapper115.prg1 = g_NESmapper.Mapper115.prg1L;
		if( g_NESmapper.Mapper115.reg[0] & 0x40 ) {
			g_NESmapper.SetPROM_32K_Banks( g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.Mapper115.prg1, g_NESmapper.Mapper115.prg0, g_NESmapper.num_8k_ROM_banks-1 );
		} else {
			g_NESmapper.SetPROM_32K_Banks( g_NESmapper.Mapper115.prg0, g_NESmapper.Mapper115.prg1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
		}
	}
}

void	NES_mapper115_SetBank_PPU()
{
	if( g_NESmapper.num_1k_VROM_banks ) {
		if( g_NESmapper.Mapper115.reg[0] & 0x80 ) {
			g_NESmapper.SetVROM_8K_Banks( 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr4, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr5,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr6, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr7,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr0, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr1,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr2, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr3 );
		} else {
			g_NESmapper.SetVROM_8K_Banks( 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr0, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr1,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr2, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr3,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr4, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr5,
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr6, 
			(g_NESmapper.Mapper115.ExChrSwitch<<8)+g_NESmapper.Mapper115.chr7 );
		}
	}
}

/////////////////////////////////////////////////////////////////////