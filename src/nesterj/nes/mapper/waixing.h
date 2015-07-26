//////////////////////////////////////////////////////////////////////////
// Õ‚–«”Œœ∑  BaseON MMC3 
//////////////////////////////////////////////////////////////////////////
// TYPEI
void WaixingTypeI_Init();
void WaixingTypeI_Reset();

unsigned int WaixingTypeI_GetChrIndex(unsigned int);
unsigned int WaixingTypeI_GetPrgIndex(unsigned int);

uint8 WaixingTypeI_MemoryReadLow(uint32 addr);
void WaixingTypeI_MemoryWriteLow(uint32 addr, uint8 data);
//////////////////////////////////////////////////////////////////////////
// TYPEA Base On TypeI
void WaixingTypeA_Init();
void WaixingTypeA_Reset();

void Poke_WaixingTypeA_A000(uint32 address,uint8 data);
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
// TYPEE Base On TypeA  WAIXING (e), Mapper 195
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
void WaixingTypeG_Init();
void WaixingTypeG_Reset();

void Poke_WaixingTypeG_8001(uint32 address,uint8 data);
void WaixingTypeG_UpdateChr(unsigned int,unsigned int);
void WaixingTypeG_UpdatePrg(unsigned int,unsigned int);
unsigned int WaixingTypeG_GetChrSource(unsigned int);

//NES_DECL_POKE( 8001 );
//byte exPrg[4];
//byte exChr[8];
//////////////////////////////////////////////////////////////////////////
// TYPEH Base On Mmc3

//////////////////////////////////////////////////////////////////////////
// TYPEJ Base On TypeI









