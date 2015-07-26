
/////////////////////////////////////////////////////////////////////
// Mapper 165


void NES_mapper165_Init()
{
	g_NESmapper.Reset = NES_mapper165_Reset;
	g_NESmapper.MemoryWrite = NES_mapper165_MemoryWrite;
	g_NESmapper.PPU_Latch_FDFE  = NES_mapper165_PPU_ChrLatch;	
	//g_NESmapper.PPU_Latch_Address = NES_mapper165_PPU_ChrLatch;	
}

void NES_mapper165_Reset()
{
	int i;
	for(i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper165.regs[i] = 0x00;
	}
		
	g_NESmapper.Mapper165.prg0 = 0;
	g_NESmapper.Mapper165.prg1 = 1;
	NES_mapper165_MMC3_set_CPU_banks();

	g_NESmapper.Mapper165.chr0 = 0;
	g_NESmapper.Mapper165.chr1 = 0;
	g_NESmapper.Mapper165.chr2 = 4;
	g_NESmapper.Mapper165.chr3 = 4;
	g_NESmapper.Mapper165.latch = 0xFD;
	NES_mapper165_MMC3_set_PPU_banks();

	g_NESmapper.Mapper165.we_sram  = 0;	// Disable
	//g_PPU.vram_write_protect = 0;
}

void NES_mapper165_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper165.regs[0] = data;
			NES_mapper165_MMC3_set_CPU_banks();
			NES_mapper165_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			g_NESmapper.Mapper165.regs[1] = data;

			switch( g_NESmapper.Mapper165.regs[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper165.chr0 = data & 0xFC;
					if( g_NESmapper.Mapper165.latch == 0xFD )
						NES_mapper165_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper165.chr1 = data & 0xFC;
					if( g_NESmapper.Mapper165.latch == 0xFE )
						NES_mapper165_MMC3_set_PPU_banks();
					break;

				case	0x02:
					g_NESmapper.Mapper165.chr2 = data & 0xFC;
					if( g_NESmapper.Mapper165.latch == 0xFD )
						NES_mapper165_MMC3_set_PPU_banks();
					break;
				case	0x04:
					g_NESmapper.Mapper165.chr3 = data & 0xFC;
					if( g_NESmapper.Mapper165.latch == 0xFE )
						NES_mapper165_MMC3_set_PPU_banks();
					break;

				case	0x06:
					g_NESmapper.Mapper165.prg0 = data;
					NES_mapper165_MMC3_set_CPU_banks();
					break;
				case	0x07:
					g_NESmapper.Mapper165.prg1 = data;
					NES_mapper165_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper165.regs[2] = data;
			if(data & 0x01)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}			
			break;
		case	0xA001:
			g_NESmapper.Mapper165.regs[3] = data;
			break;
		default:
			break;
	}
}



void NES_mapper165_MMC3_set_CPU_banks()
{
	g_NESmapper.SetPROM_32K_Banks(
								g_NESmapper.Mapper165.prg0,
								g_NESmapper.Mapper165.prg1,
								g_NESmapper.num_8k_ROM_banks-2,
								g_NESmapper.num_8k_ROM_banks-1);
}                            

void NES_mapper165_MMC3_set_PPU_banks()
{
	if(g_NESmapper.Mapper165.latch==0xFD){
		NES_mapper165_MMC3_set_PPUSUB_banks( 0, g_NESmapper.Mapper165.chr0 );
		NES_mapper165_MMC3_set_PPUSUB_banks( 4, g_NESmapper.Mapper165.chr2 );
	}else{
		NES_mapper165_MMC3_set_PPUSUB_banks( 0, g_NESmapper.Mapper165.chr1 );
		NES_mapper165_MMC3_set_PPUSUB_banks( 4, g_NESmapper.Mapper165.chr3 );
	}
}

void NES_mapper165_MMC3_set_PPUSUB_banks( int bank, int page )
{	
	if( page == 0 ) {
		g_NESmapper.SetCRAM_4K_Bank( bank, page>>2 );			
	} else{                   
		g_NESmapper.SetVROM_4K_Bank( bank, page>>2 );
	}
}


void NES_mapper165_PPU_ChrLatch(uint32 addr)
{
	uint32 mask = addr&0x1FF0;

	if( mask == 0x1FD0 ) {
		g_NESmapper.Mapper165.latch = 0xFD;
		NES_mapper165_MMC3_set_PPU_banks();
	} else if( mask == 0x1FE0 ) {
		g_NESmapper.Mapper165.latch = 0xFE;
		NES_mapper165_MMC3_set_PPU_banks();
	}
}
/////////////////////////////////////////////////////////////////////

