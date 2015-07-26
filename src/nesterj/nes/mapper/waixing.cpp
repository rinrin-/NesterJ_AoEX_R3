//////////////////////////////////////////////////////////////////////////
// ÍâÐÇÓÎÏ·  BaseON MMC3 
//////////////////////////////////////////////////////////////////////////
// TYPEI
void WaixingTypeI_Init()
{

	Mmc3_Init();
	
	g_NESmapper.Reset = WaixingTypeI_Reset;
	g_NESmapper.MemoryReadLow  = WaixingTypeI_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = WaixingTypeI_MemoryWriteLow;
}
void WaixingTypeI_Reset()
{	
	Mmc3_Reset();	
}

unsigned int WaixingTypeI_GetChrIndex(unsigned int addr)
{
	return addr >> 10 ^ (g_NESmapper.Mmc3.ctrl0 >> 5 & 0x4);
}

unsigned int WaixingTypeI_GetPrgIndex(unsigned int addr)
{
	return addr >> 13 ^ (g_NESmapper.Mmc3.ctrl0 >> 5 & ~addr >> 12 & 0x2);
}

uint8 WaixingTypeI_MemoryReadLow(uint32 addr)
{
	if(addr>=0x5000)
		return g_NESmapper.waixing.wram[addr-(0x5000-0x2000)];
	return (uint8)(addr>>8);
}
void WaixingTypeI_MemoryWriteLow(uint32 addr, uint8 data)
{
	g_NESmapper.waixing.wram[addr-(0x5000-0x2000)]=data;
}
//////////////////////////////////////////////////////////////////////////
// TYPEA Base On TypeI
void WaixingTypeA_Init()
{
	WaixingTypeI_Init();		
	Poke_A000 = Poke_WaixingTypeA_A000;	
	Poke_A001 = Poke_Nop;	
	//Poke_8000 = Poke_Mmc3_8000;
	//Poke_8001 = Poke_Mmc3_8001;
}
void WaixingTypeA_Reset()
{
	WaixingTypeI_Reset();
}

void Poke_WaixingTypeA_A000(uint32 address,uint8 data)
{
	data &= 0x03;
	if( data == 0 )	     g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
	else if( data == 1 ) g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else if( data == 2 ) g_NESmapper.set_mirroring(0,0,0,0);
	else		     g_NESmapper.set_mirroring(1,1,1,1);
}

unsigned int WaixingTypeA_GetChrSource(unsigned int);
void WaixingTypeA_UpdateChr(unsigned int,unsigned int);
//////////////////////////////////////////////////////////////////////////
// TYPEB Base On TypeA
void WaixingTypeB_Init();
void WaixingTypeB_Reset();

unsigned int WaixingTypeB_GetChrSource(unsigned int);
//////////////////////////////////////////////////////////////////////////
// TYPEC Base On TypeA
void WaixingTypeC_Init();
void WaixingTypeC_Reset();

unsigned int WaixingTypeC_GetChrSource(unsigned int);
//////////////////////////////////////////////////////////////////////////
// TYPED Base On TypeA
void WaixingTypeD_Init();
void WaixingTypeD_Reset();

unsigned int WaixingTypeD_GetChrSource(unsigned int);
//////////////////////////////////////////////////////////////////////////
// TYPEE Base On TypeA
void WaixingTypeE_Init();
void WaixingTypeE_Reset();

unsigned int WaixingTypeB_GetChrSource(unsigned int);
//////////////////////////////////////////////////////////////////////////
// TYPEF Base On TypeA
void WaixingTypeF_Init();
void WaixingTypeF_Reset();

unsigned int WaixingTypeF_GetChrSource(unsigned int);
void WaixingTypeF_UpdatePrg(unsigned int,unsigned int);
//NES_DECL_POKE( 8001 );
//byte exPrg[4];
//////////////////////////////////////////////////////////////////////////
// TYPEG Base On TypeA
void WaixingTypeG_Init()
{
	WaixingTypeA_Init();	
	UpdateChr    =WaixingTypeG_UpdateChr;
	UpdatePrg    =WaixingTypeG_UpdatePrg;
	GetChrSource =WaixingTypeG_GetChrSource;
	Poke_8001 = Poke_WaixingTypeG_8001;	
	g_NESmapper.Reset = WaixingTypeG_Reset;
}
void WaixingTypeG_Reset()
{
	uint8 i;
	g_NESmapper.waixing.exPrg[0] = 0x00;
	g_NESmapper.waixing.exPrg[1] = 0x01;
	g_NESmapper.waixing.exPrg[2] = 0x3E;
	g_NESmapper.waixing.exPrg[3] = 0x3F;
	
	for (i=0; i < 8; ++i)
		g_NESmapper.waixing.exChr[0] = i;
						
	WaixingTypeA_Reset();
}

void Poke_WaixingTypeG_8001(uint32 address,uint8 data)
{
	switch (g_NESmapper.Mmc3.ctrl0 & 0xF)
	{
		case 0x0: g_NESmapper.waixing.exChr[0] = data; Mmc3_UpdateChr(); break;
		case 0x1: g_NESmapper.waixing.exChr[2] = data; Mmc3_UpdateChr(); break;
		case 0x2: g_NESmapper.waixing.exChr[4] = data; Mmc3_UpdateChr(); break;
		case 0x3: g_NESmapper.waixing.exChr[5] = data; Mmc3_UpdateChr(); break;
		case 0x4: g_NESmapper.waixing.exChr[6] = data; Mmc3_UpdateChr(); break;
		case 0x5: g_NESmapper.waixing.exChr[7] = data; Mmc3_UpdateChr(); break;
		case 0x6: g_NESmapper.waixing.exPrg[0] = data; Mmc3_UpdatePrg(); break;
		case 0x7: g_NESmapper.waixing.exPrg[1] = data; Mmc3_UpdatePrg(); break;
		case 0x8: g_NESmapper.waixing.exPrg[2] = data; Mmc3_UpdatePrg(); break;
		case 0x9: g_NESmapper.waixing.exPrg[3] = data; Mmc3_UpdatePrg(); break;
		case 0xA: g_NESmapper.waixing.exChr[1] = data; Mmc3_UpdateChr(); break;
		case 0xB: g_NESmapper.waixing.exChr[3] = data; Mmc3_UpdateChr(); break;
	}
}

void WaixingTypeG_UpdateChr(unsigned int addr,unsigned int data)
{
	unsigned int bank = g_NESmapper.waixing.exChr[WaixingTypeI_GetChrIndex(addr)];
	if(GetChrSource(bank))
	{
		g_NESmapper.SetCRAM_1K_Bank( addr>>10, bank );
	}else{
		g_NESmapper.SetVROM_1K_Bank( addr>>10, bank );
	}
}

void WaixingTypeG_UpdatePrg(unsigned int addr,unsigned int data)
{
	g_NESmapper.SetPROM_8K_Bank( (addr>>13)+4, g_NESmapper.waixing.exPrg[WaixingTypeI_GetPrgIndex(addr)] );
}
unsigned int WaixingTypeG_GetChrSource(unsigned int bank)
{
	return bank <= 7;
}
//NES_DECL_POKE( 8001 );
//////////////////////////////////////////////////////////////////////////
// TYPEH Base On Mmc3

//////////////////////////////////////////////////////////////////////////
// TYPEJ Base On TypeI









