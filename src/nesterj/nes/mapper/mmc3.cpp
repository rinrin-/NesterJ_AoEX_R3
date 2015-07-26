void (*UpdateChr)(unsigned int,unsigned int)=0;
void (*UpdatePrg)(unsigned int,unsigned int)=0;
unsigned int (*GetChrSource)(unsigned int)=0;

void (*Poke_8000)(uint32 address,uint8 data)=0;
void (*Poke_8001)(uint32 address,uint8 data)=0;
void (*Poke_A000)(uint32 address,uint8 data)=0;
void (*Poke_A001)(uint32 address,uint8 data)=0;
void (*Poke_C000)(uint32 address,uint8 data)=0;
void (*Poke_C001)(uint32 address,uint8 data)=0;
void (*Poke_E000)(uint32 address,uint8 data)=0;
void (*Poke_E001)(uint32 address,uint8 data)=0;
//////////////////////////////////////////////////////////////////////////
// MMC3 MEM WRITE 8000~FFFF
//////////////////////////////////////////////////////////////////////////
void Mmc3_MemoryWrite(uint32 address, uint8 data)
{
	switch( address & 0xE001 ) 
	{
		case	0x8000:	Poke_8000(address,data);break;	
		case	0x8001:	Poke_8001(address,data);break;	
		case	0xA000:	Poke_A000(address,data);break;	
		case	0xA001:	Poke_A001(address,data);break;	
		case	0xC000:	Poke_C000(address,data);break;	
		case	0xC001:	Poke_C001(address,data);break;				
		case	0xE000:	Poke_E000(address,data);break;
		case	0xE001:	Poke_E001(address,data);break;
	}
}
void Poke_Mmc3_8000(uint32 address,uint8 data)
{
	const unsigned int diff = g_NESmapper.Mmc3.ctrl0 ^ data;
	g_NESmapper.Mmc3.ctrl0 = data;

	if (diff & 0x40)
	{
		const unsigned int v[2] =
		{
			g_NESmapper.Mmc3.prg[(data >> 5 & 0x2) ^ 0],
			g_NESmapper.Mmc3.prg[(data >> 5 & 0x2) ^ 2]
		};

		UpdatePrg( 0x0000, v[0] );
		UpdatePrg( 0x4000, v[1] );
	}

	if (diff & 0x80)
		Mmc3_UpdateChr();
}
void Poke_Mmc3_8001(uint32 address,uint8 data)
{
	unsigned int addr = g_NESmapper.Mmc3.ctrl0 & 0x7;

	if (addr < 6)
	{
		unsigned int base = g_NESmapper.Mmc3.ctrl0 << 5 & 0x1000;

		if (addr < 2)
		{
			addr <<= 1;
			base |= addr << 10;
			UpdateChr( base | 0x0000, (g_NESmapper.Mmc3.chr[addr+0] = data & 0xFE) );
			UpdateChr( base | 0x0400, (g_NESmapper.Mmc3.chr[addr+1] = data | 0x01) );
		}
		else
		{
			UpdateChr( (base ^ 0x1000) | (addr-2) << 10, (g_NESmapper.Mmc3.chr[addr+2] = data) );
		}
	}
	else
	{
		UpdatePrg( (addr == 6) ? (g_NESmapper.Mmc3.ctrl0 << 8 & 0x4000) : 0x2000, (g_NESmapper.Mmc3.prg[addr-6] = data & 0x3F) );
	}		
}
void Poke_Mmc3_A000(uint32 address,uint8 data)
{
	if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
	{
		if( data & 0x01 )
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	}
}

void Poke_Mmc3_A001(uint32 address,uint8 data)
{
	g_NESmapper.Mmc3.ctrl1 = data;	
}

//////////////////////////////////////////////////////////////////////////
// MMC3 IRQ
//////////////////////////////////////////////////////////////////////////
void Poke_Mmc3_C000(uint32 address,uint8 data)
{
	g_NESmapper.Mmc3.count = data;
	g_NESmapper.Mmc3.reload = 0;
}
void Poke_Mmc3_C001(uint32 address,uint8 data)
{
	g_NESmapper.Mmc3.latch = data;
	g_NESmapper.Mmc3.reload = 0;
}
void Poke_Mmc3_E000(uint32 address,uint8 data)
{
	g_NESmapper.Mmc3.enabled = 0;
	g_NESmapper.Mmc3.reload = 0;
	nes6502_ClrIRQ( IRQ_MAPPER );
}
void Poke_Mmc3_E001(uint32 address,uint8 data)
{
	g_NESmapper.Mmc3.enabled = 1;
	g_NESmapper.Mmc3.reload = 0;
}

void Mmc3_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
	if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled())
	{
		if( g_NESmapper.Mmc3.enabled && !g_NESmapper.Mmc3.reload ) {
			if( scanline == 0 ) {
				if( g_NESmapper.Mmc3.count ) {
					g_NESmapper.Mmc3.count -= 1;
				}
			}
			if(!(g_NESmapper.Mmc3.count)){
				g_NESmapper.Mmc3.reload = 0xFF;
				g_NESmapper.Mmc3.count = g_NESmapper.Mmc3.latch;
				nes6502_SetIRQ( IRQ_MAPPER );
			}
			g_NESmapper.Mmc3.count--;
		}
	}
	}
}

void Poke_Nop(uint32 addr,uint8 data)
{
	return;
}

//////////////////////////////////////////////////////////////////////////
// MMC3 Init
//////////////////////////////////////////////////////////////////////////
void Mmc3_Init()
{
	UpdateChr    =Mmc3_UpdateChr2p;
	UpdatePrg    =Mmc3_UpdatePrg2p;
	GetChrSource =Mmc3_GetChrSource;
	
	Poke_8000 = Poke_Mmc3_8000;
	Poke_8001 = Poke_Mmc3_8001;
	Poke_A000 = Poke_Mmc3_A000;
	Poke_A001 = Poke_Mmc3_A001;
	Poke_C000 = Poke_Mmc3_C000;
	Poke_C001 = Poke_Mmc3_C001;
	Poke_E000 = Poke_Mmc3_E000;
	Poke_E001 = Poke_Mmc3_E001;
	
	g_NESmapper.Reset = Mmc3_Reset;	
	g_NESmapper.MemoryWrite = Mmc3_MemoryWrite;
	g_NESmapper.HSync = Mmc3_HSync;
}


//////////////////////////////////////////////////////////////////////////
// MMC3 Reset
//////////////////////////////////////////////////////////////////////////
void Mmc3_Reset()
{	
	int i;
	
	g_NESmapper.Mmc3.ctrl0 = 0;
	g_NESmapper.Mmc3.ctrl1 = 0;	
	
	for (i=0; i < 8; ++i)
		g_NESmapper.Mmc3.chr[i] = i;
	
	g_NESmapper.Mmc3.prg[0] = 0x00;
	g_NESmapper.Mmc3.prg[1] = 0x01;
	g_NESmapper.Mmc3.prg[2] = 0x3E;
	g_NESmapper.Mmc3.prg[3] = 0x3F;
	
	g_NESmapper.Mmc3.count = 0;
	g_NESmapper.Mmc3.latch = 0;
	g_NESmapper.Mmc3.reload = 0;
	g_NESmapper.Mmc3.enabled = 0;
	
	Mmc3_UpdatePrg (); 
	Mmc3_UpdateChr ();	
}


//////////////////////////////////////////////////////////////////////////
// MMC3
//////////////////////////////////////////////////////////////////////////


void Mmc3_SwapChr1K(uint8 page,uint32 bank)
{
	if(GetChrSource(bank))
	{
		g_NESmapper.SetCRAM_1K_Bank( page, bank );
	}else{
		g_NESmapper.SetVROM_1K_Bank( page, bank );
	}
}

void Mmc3_SwapChr2K(uint8 page,uint32 bank)
{
	if(GetChrSource(bank))
	{
		g_NESmapper.SetCRAM_2K_Bank( page, bank );
	}else{
		g_NESmapper.SetVROM_2K_Bank( page, bank );
	}
}

void Mmc3_UpdatePrg2p(unsigned int addr,unsigned int bank)
{
	//prg.SwapBank<SIZE_8K>( address, bank );
	g_NESmapper.SetPROM_8K_Bank( (addr>>13)+4, bank);
}

void Mmc3_UpdateChr2p(unsigned int addr,unsigned int bank)
{
	//chr.SwapBank<SIZE_1K>( address, bank );
	Mmc3_SwapChr1K(addr>>10,bank);
}


void Mmc3_UpdatePrg()
{
	const unsigned int x = g_NESmapper.Mmc3.ctrl0 >> 5 & 0x2;
		
	UpdatePrg( 0x0000, g_NESmapper.Mmc3.prg[0^x] );
	UpdatePrg( 0x2000, g_NESmapper.Mmc3.prg[1^0] );
	UpdatePrg( 0x4000, g_NESmapper.Mmc3.prg[2^x] );
	UpdatePrg( 0x6000, g_NESmapper.Mmc3.prg[3^0] );		
}

void Mmc3_UpdateChr()
{
	const unsigned int x = g_NESmapper.Mmc3.ctrl0 >> 5 & 0x4;
	unsigned int i=0;
	for (i=0; i < 8; ++i)
		UpdateChr( i * 0x400, g_NESmapper.Mmc3.chr[i^x] );
}

unsigned int Mmc3_GetChrSource(unsigned int dummy){return 0;}

