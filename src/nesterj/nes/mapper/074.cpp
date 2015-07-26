/////////////////////////////////////////////////////////////////////
// Mapper 074
void NES_mapper74_Init()
{

	NES_WaiXing_MMC3_set_PPU_banks=NES_mapper074_MMC3_set_PPU_banks;
	g_NESmapper.Reset = NES_mapper074_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper074_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper074_MemoryReadLow;
	g_NESmapper.MemoryWrite = NES_mapper074_MemoryWrite;
	g_NESmapper.HSync = NES_mapper074_HSync;
}

void NES_mapper074_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) g_NESmapper.Mapper074.regs[i] = 0x00;
	g_NESmapper.Mapper074.prg0 = 0;
	g_NESmapper.Mapper074.prg1 = 1;
	NES_mapper074_MMC3_set_CPU_banks();

	g_NESmapper.Mapper074.chr[0]  = 0;
	g_NESmapper.Mapper074.chr[2]  = 2;
	g_NESmapper.Mapper074.chr[4]  = 4;
	g_NESmapper.Mapper074.chr[5]  = 5;
	g_NESmapper.Mapper074.chr[6]  = 6;
	g_NESmapper.Mapper074.chr[7]  = 7;
	NES_WaiXing_MMC3_set_PPU_banks();

	g_NESmapper.Mapper074.we_sram  = 0;	// Disable
	g_NESmapper.Mapper074.irq_enable = 0;	// Disable
	g_NESmapper.Mapper074.irq_counter = 0;
	g_NESmapper.Mapper074.irq_latch = 0;
	g_NESmapper.Mapper074.irq_request = 0;	

	
	g_NESmapper.Mapper074.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;
	g_NESmapper.SetPROM_Bank( 2, g_NESmapper.Mapper074.wram);
}


uint8 NES_mapper074_MemoryReadLow(uint32 addr)
{
	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		return	g_NESmapper.Mapper074.wram[addr-0x4000];
	}else{
		return  (uint8)(addr >> 8);
	}
}


void NES_mapper074_MemoryWriteLow(uint32 addr, uint8 data)
{	
	if( addr >= 0x5000 && addr <= 0x5FFF ) 
	{
		g_NESmapper.Mapper074.wram[addr-0x4000] = data;
	} 
}


void NES_mapper074_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 cmd;
	switch( addr & 0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper074.regs[0] = data;
			NES_mapper074_MMC3_set_CPU_banks();
			NES_WaiXing_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			g_NESmapper.Mapper074.regs[1] = data;
			cmd = g_NESmapper.Mapper074.regs[0] & 0x07;

			switch( cmd ) {
				case	0x00:
					g_NESmapper.Mapper074.chr[0] = data & 0xFE;
					NES_WaiXing_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper074.chr[2] = data & 0xFE;
					NES_WaiXing_MMC3_set_PPU_banks();
					break;
					
				case	0x02:
				case	0x03:
				case	0x04:
				case	0x05:
					g_NESmapper.Mapper074.chr[ cmd+2] = data;
					NES_WaiXing_MMC3_set_PPU_banks();
					break;
					
				case	0x06:
					g_NESmapper.Mapper074.prg0 = data;
					NES_mapper074_MMC3_set_CPU_banks();
					break;
				case	0x07:
					g_NESmapper.Mapper074.prg1 = data;
					NES_mapper074_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper074.regs[2] = data;
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if( data & 0x01 ) 	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				else				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}			
			break;
		case	0xA001:
			g_NESmapper.Mapper074.regs[3] = data;
			break;
		case	0xC000:
			g_NESmapper.Mapper074.regs[4] = data;
			g_NESmapper.Mapper074.irq_counter = data;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
		case	0xC001:
			g_NESmapper.Mapper074.regs[5] = data;
			g_NESmapper.Mapper074.irq_latch = data;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
		case	0xE000:
			g_NESmapper.Mapper074.regs[6] = data;
			g_NESmapper.Mapper074.irq_enable = 0;
			g_NESmapper.Mapper074.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper074.regs[7] = data;
			g_NESmapper.Mapper074.irq_enable = 1;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
	}	
}


void NES_mapper074_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
		{
			if( g_NESmapper.Mapper074.irq_enable && !g_NESmapper.Mapper074.irq_request ) {
				if( scanline == 0 ) {
					if( g_NESmapper.Mapper074.irq_counter ) {
						g_NESmapper.Mapper074.irq_counter -= 1;
					}
				}
				if(!(g_NESmapper.Mapper074.irq_counter)){
					g_NESmapper.Mapper074.irq_request = 0xFF;
					g_NESmapper.Mapper074.irq_counter = g_NESmapper.Mapper074.irq_latch;
					nes6502_SetIRQ( IRQ_MAPPER );
				}
				g_NESmapper.Mapper074.irq_counter--;
			}
		}
	}
}


void NES_mapper074_MMC3_set_CPU_banks()
{
	if( g_NESmapper.Mapper074.regs[0] & 0x40 ) 
	{
		g_NESmapper.set_CPU_banks4(
			g_NESmapper.num_8k_ROM_banks-2, 
			g_NESmapper.Mapper074.prg1, 
			g_NESmapper.Mapper074.prg0,
			g_NESmapper.num_8k_ROM_banks-1);
	}
	else 
	{
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper074.prg0, g_NESmapper.Mapper074.prg1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
	}
}

void NES_mapper074_MMC3_set_PPUSUB( int bank, int page ,int bRAM)
{
	if(bRAM) {
		g_NESmapper.SetCRAM_1K_Bank( bank, page&7 );
	} else {
		g_NESmapper.SetVROM_1K_Bank( bank, page );
	}
}

void NES_mapper074_MMC3_set_PPU_banks()
{
	//if(g_NESmapper.num_1k_VROM_banks)
	g_NESmapper.Mapper074.chr[1]= g_NESmapper.Mapper074.chr[0]+1;
	g_NESmapper.Mapper074.chr[3]= g_NESmapper.Mapper074.chr[2]+1;

	if( g_NESmapper.Mapper074.regs[0] & 0x80 ) 
	{
		NES_mapper074_MMC3_set_PPUSUB( 4, g_NESmapper.Mapper074.chr[0] , (g_NESmapper.Mapper074.chr[0]==8)||(g_NESmapper.Mapper074.chr[0]==9));
		NES_mapper074_MMC3_set_PPUSUB( 5, g_NESmapper.Mapper074.chr[1] , (g_NESmapper.Mapper074.chr[1]==8)||(g_NESmapper.Mapper074.chr[1]==9));
		NES_mapper074_MMC3_set_PPUSUB( 6, g_NESmapper.Mapper074.chr[2] , (g_NESmapper.Mapper074.chr[2]==8)||(g_NESmapper.Mapper074.chr[2]==9));
		NES_mapper074_MMC3_set_PPUSUB( 7, g_NESmapper.Mapper074.chr[3] , (g_NESmapper.Mapper074.chr[3]==8)||(g_NESmapper.Mapper074.chr[3]==9));
		NES_mapper074_MMC3_set_PPUSUB( 0, g_NESmapper.Mapper074.chr[4] , (g_NESmapper.Mapper074.chr[4]==8)||(g_NESmapper.Mapper074.chr[4]==9));
		NES_mapper074_MMC3_set_PPUSUB( 1, g_NESmapper.Mapper074.chr[5] , (g_NESmapper.Mapper074.chr[5]==8)||(g_NESmapper.Mapper074.chr[5]==9));
		NES_mapper074_MMC3_set_PPUSUB( 2, g_NESmapper.Mapper074.chr[6] , (g_NESmapper.Mapper074.chr[6]==8)||(g_NESmapper.Mapper074.chr[6]==9));
		NES_mapper074_MMC3_set_PPUSUB( 3, g_NESmapper.Mapper074.chr[7] , (g_NESmapper.Mapper074.chr[7]==8)||(g_NESmapper.Mapper074.chr[7]==9));
	} else {
		NES_mapper074_MMC3_set_PPUSUB( 0, g_NESmapper.Mapper074.chr[0] , (g_NESmapper.Mapper074.chr[0]==8)||(g_NESmapper.Mapper074.chr[0]==9));
		NES_mapper074_MMC3_set_PPUSUB( 1, g_NESmapper.Mapper074.chr[1] , (g_NESmapper.Mapper074.chr[1]==8)||(g_NESmapper.Mapper074.chr[1]==9));
		NES_mapper074_MMC3_set_PPUSUB( 2, g_NESmapper.Mapper074.chr[2] , (g_NESmapper.Mapper074.chr[2]==8)||(g_NESmapper.Mapper074.chr[2]==9));
		NES_mapper074_MMC3_set_PPUSUB( 3, g_NESmapper.Mapper074.chr[3] , (g_NESmapper.Mapper074.chr[3]==8)||(g_NESmapper.Mapper074.chr[3]==9));
		NES_mapper074_MMC3_set_PPUSUB( 4, g_NESmapper.Mapper074.chr[4] , (g_NESmapper.Mapper074.chr[4]==8)||(g_NESmapper.Mapper074.chr[4]==9));
		NES_mapper074_MMC3_set_PPUSUB( 5, g_NESmapper.Mapper074.chr[5] , (g_NESmapper.Mapper074.chr[5]==8)||(g_NESmapper.Mapper074.chr[5]==9));
		NES_mapper074_MMC3_set_PPUSUB( 6, g_NESmapper.Mapper074.chr[6] , (g_NESmapper.Mapper074.chr[6]==8)||(g_NESmapper.Mapper074.chr[6]==9));
		NES_mapper074_MMC3_set_PPUSUB( 7, g_NESmapper.Mapper074.chr[7] , (g_NESmapper.Mapper074.chr[7]==8)||(g_NESmapper.Mapper074.chr[7]==9));
	}
}

/////////////////////////////////////////////////////////////////////
//191
void NES_mapper191_Init()
{
	NES_mapper74_Init();
	NES_WaiXing_MMC3_set_PPU_banks=NES_mapper191_MMC3_set_PPU_banks;
}

void NES_mapper191_MMC3_set_PPU_banks()
{
	int i;
	g_NESmapper.Mapper074.chr[1]=g_NESmapper.Mapper074.chr[0]+1;
	g_NESmapper.Mapper074.chr[3]=g_NESmapper.Mapper074.chr[2]+1;

	if( g_NESmapper.Mapper074.regs[0] & 0x80 ) 
	{
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[(i+4)&7],g_NESmapper.Mapper074.chr[(i+4)&7]&0x80);			
		}
	} else {
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[i],g_NESmapper.Mapper074.chr[i]&0x80);
		}
	}
}

/////////////////////////////////////////////////////////////////////
//192
void NES_mapper192_Init()
{
	NES_mapper74_Init();
	NES_WaiXing_MMC3_set_PPU_banks=NES_mapper192_MMC3_set_PPU_banks;
}

void NES_mapper192_MMC3_set_PPU_banks()
{
	int i;
	g_NESmapper.Mapper074.chr[1]=g_NESmapper.Mapper074.chr[0]+1;
	g_NESmapper.Mapper074.chr[3]=g_NESmapper.Mapper074.chr[2]+1;

	if( g_NESmapper.Mapper074.regs[0] & 0x80 ) 
	{
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[(i+4)&7],(g_NESmapper.Mapper074.chr[(i+4)&7]==0x8)||
																				(g_NESmapper.Mapper074.chr[(i+4)&7]==0x9)||
																				(g_NESmapper.Mapper074.chr[(i+4)&7]==0xa)||
																				(g_NESmapper.Mapper074.chr[(i+4)&7]==0xb));
		}
	} else {
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[i],(g_NESmapper.Mapper074.chr[i]==0x8)||
																		(g_NESmapper.Mapper074.chr[i]==0x9)||
																		(g_NESmapper.Mapper074.chr[i]==0xa)||
																		(g_NESmapper.Mapper074.chr[i]==0xb));
		}
	}
}

/////////////////////////////////////////////////////////////////////
//194
void NES_mapper194_Init()
{
	NES_mapper74_Init();
	NES_WaiXing_MMC3_set_PPU_banks=NES_mapper194_MMC3_set_PPU_banks;
}

void NES_mapper194_MMC3_set_PPU_banks()
{
	int i;
	g_NESmapper.Mapper074.chr[1]=g_NESmapper.Mapper074.chr[0]+1;
	g_NESmapper.Mapper074.chr[3]=g_NESmapper.Mapper074.chr[2]+1;

	if( g_NESmapper.Mapper074.regs[0] & 0x80 ) 
	{
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[(i+4)&7],(g_NESmapper.Mapper074.chr[(i+4)&7]< 0x02));
		}
	} else {
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[i],(g_NESmapper.Mapper074.chr[i]< 0x02));
		}
	}
}
/////////////////////////////////////////////////////////////////////
//195
void NES_mapper195_Init()
{
	NES_mapper74_Init();
	NES_WaiXing_MMC3_set_PPU_banks=NES_mapper195_MMC3_set_PPU_banks;
}

void NES_mapper195_MMC3_set_PPU_banks()
{
	int i;
	g_NESmapper.Mapper074.chr[1]=g_NESmapper.Mapper074.chr[0]+1;
	g_NESmapper.Mapper074.chr[3]=g_NESmapper.Mapper074.chr[2]+1;

	if( g_NESmapper.Mapper074.regs[0] & 0x80 ) 
	{
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[(i+4)&7],(g_NESmapper.Mapper074.chr[(i+4)&7]< 0x04));
		}
	} else {
		for(i=0; i<8; i++)
		{
			NES_mapper074_MMC3_set_PPUSUB(i,g_NESmapper.Mapper074.chr[i],(g_NESmapper.Mapper074.chr[i]< 0x04));
		}
	}
}

/////////////////////////////////////////////////////////////////////
//198
void NES_mapper198_Init()
{
	g_NESmapper.Reset = NES_mapper198_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper074_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper074_MemoryReadLow;
	g_NESmapper.MemoryWrite = NES_mapper198_MemoryWrite;
	g_NESmapper.HSync = NES_mapper074_HSync;
}

void NES_mapper198_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) g_NESmapper.Mapper074.regs[i] = 0x00;
	
	g_NESmapper.Mapper074.prg0 = 0;
	g_NESmapper.Mapper074.prg1 = 1;
	g_NESmapper.Mapper074.prg2 = g_NESmapper.num_8k_ROM_banks -2;
	g_NESmapper.Mapper074.prg3 = g_NESmapper.num_8k_ROM_banks -1;
	NES_mapper198_MMC3_set_CPU_banks();

	g_NESmapper.Mapper074.chr[0] = 0;
	g_NESmapper.Mapper074.chr[1] = 1;
	g_NESmapper.Mapper074.chr[2] = 2;
	g_NESmapper.Mapper074.chr[3] = 3;
	g_NESmapper.Mapper074.chr[4]  = 4;
	g_NESmapper.Mapper074.chr[5]  = 5;
	g_NESmapper.Mapper074.chr[6]  = 6;
	g_NESmapper.Mapper074.chr[7]  = 7;
	NES_mapper198_MMC3_set_PPU_banks();

	g_NESmapper.Mapper074.we_sram  = 0;	// Disable
	g_NESmapper.Mapper074.irq_enable = 0;	// Disable
	g_NESmapper.Mapper074.irq_counter = 0;
	g_NESmapper.Mapper074.irq_latch = 0;
	g_NESmapper.Mapper074.irq_request = 0;	
	
	g_NESmapper.Mapper074.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;
	g_NESmapper.SetPROM_Bank( 2, g_NESmapper.Mapper074.wram);
}


void NES_mapper198_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr & 0xE001 ) {
		case	0x8000:
			g_NESmapper.Mapper074.regs[0] = data;
			NES_mapper198_MMC3_set_CPU_banks();
			NES_mapper198_MMC3_set_PPU_banks();
			break;
		case	0x8001:
			g_NESmapper.Mapper074.regs[1] = data;

			switch( g_NESmapper.Mapper074.regs[0] & 0x07 ) {
				case	0x00:
					g_NESmapper.Mapper074.chr[0] = data;
					NES_mapper198_MMC3_set_PPU_banks();
					break;
				case	0x01:
					g_NESmapper.Mapper074.chr[2] = data;
					NES_mapper198_MMC3_set_PPU_banks();
					break;
					
				case	0x02:
				case	0x03:
				case	0x04:
				case	0x05:
					g_NESmapper.Mapper074.chr[(g_NESmapper.Mapper074.regs[0]&0x7)+2] = data;
					NES_mapper198_MMC3_set_PPU_banks();
					break;
					
				case	0x06:
					g_NESmapper.Mapper074.prg0 = data;
					NES_mapper198_MMC3_set_CPU_banks();
					break;
				case	0x07:
					g_NESmapper.Mapper074.prg1 = data;
					NES_mapper198_MMC3_set_CPU_banks();
					break;
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper074.regs[2] = data;
			data &= 0x03;
			if( data == 0 )	     g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			else if( data == 1 ) g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			else if( data == 2 ) g_NESmapper.set_mirroring(0,0,0,0);
			else		     g_NESmapper.set_mirroring(1,1,1,1);
			break;
		case	0xA001:
			g_NESmapper.Mapper074.regs[3] = data;
			break;
		case	0xC000:
			g_NESmapper.Mapper074.regs[4] = data;
			g_NESmapper.Mapper074.irq_counter = data;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
		case	0xC001:
			g_NESmapper.Mapper074.regs[5] = data;
			g_NESmapper.Mapper074.irq_latch = data;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
		case	0xE000:
			g_NESmapper.Mapper074.regs[6] = data;
			g_NESmapper.Mapper074.irq_enable = 0;
			g_NESmapper.Mapper074.irq_request = 0;
			nes6502_ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			g_NESmapper.Mapper074.regs[7] = data;
			g_NESmapper.Mapper074.irq_enable = 1;
			g_NESmapper.Mapper074.irq_request = 0;
			break;
	}	
}

void NES_mapper198_MMC3_set_CPU_banks()
{
	if (g_NESmapper.Mapper074.prg0 >= 0x50)
		g_NESmapper.Mapper074.prg0 &= 0x4F;
				
	if( g_NESmapper.Mapper074.regs[0] & 0x40 ) {
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper074.prg2, g_NESmapper.Mapper074.prg1, g_NESmapper.Mapper074.prg0, g_NESmapper.Mapper074.prg3);
	} else {
		g_NESmapper.set_CPU_banks4(g_NESmapper.Mapper074.prg0, g_NESmapper.Mapper074.prg1, g_NESmapper.Mapper074.prg2, g_NESmapper.Mapper074.prg3);
	}
}

void NES_mapper198_MMC3_set_PPU_banks()
{
	const uint32 swap = (g_NESmapper.Mapper074.regs[0] & 0x80) << 5;

	g_NESmapper.SetVROM_1K_Bank( (0x0000 ^ swap)/0x400, g_NESmapper.Mapper074.chr[0]  );
	g_NESmapper.SetVROM_1K_Bank( (0x0400 ^ swap)/0x400, g_NESmapper.Mapper074.chr[0]+1  );
	g_NESmapper.SetVROM_1K_Bank( (0x0800 ^ swap)/0x400, g_NESmapper.Mapper074.chr[2]  );
	g_NESmapper.SetVROM_1K_Bank( (0x0C00 ^ swap)/0x400, g_NESmapper.Mapper074.chr[2]+1  );
	g_NESmapper.SetVROM_1K_Bank( (0x1000 ^ swap)/0x400, g_NESmapper.Mapper074.chr[4]          );
	g_NESmapper.SetVROM_1K_Bank( (0x1400 ^ swap)/0x400, g_NESmapper.Mapper074.chr[5]          );
	g_NESmapper.SetVROM_1K_Bank( (0x1800 ^ swap)/0x400, g_NESmapper.Mapper074.chr[6]          );
	g_NESmapper.SetVROM_1K_Bank( (0x1C00 ^ swap)/0x400, g_NESmapper.Mapper074.chr[7]          );
}