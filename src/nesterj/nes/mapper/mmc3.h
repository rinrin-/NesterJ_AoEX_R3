//////////////////////////////////////////////////////////////////////////
// MMC3
//////////////////////////////////////////////////////////////////////////
//Memory Write 8000~FFFF
void Mmc3_MemoryWrite(uint32 address, uint8 data);
void Poke_Mmc3_8000(uint32 address,uint8 data);
void Poke_Mmc3_8001(uint32 address,uint8 data);
void Poke_Mmc3_A000(uint32 address,uint8 data);
void Poke_Mmc3_A001(uint32 address,uint8 data);
void Poke_Mmc3_C000(uint32 address,uint8 data);
void Poke_Mmc3_C001(uint32 address,uint8 data);
void Poke_Mmc3_E000(uint32 address,uint8 data);
void Poke_Mmc3_E001(uint32 address,uint8 data);
void Poke_Nop(uint32,uint8);

//初始化Mmc3函数指针
void Mmc3_Init();
void Mmc3_Reset();
void Mmc3_UpdatePrg();
void Mmc3_UpdateChr();
void Mmc3_UpdatePrg2p(unsigned int addr,unsigned int bank);
void Mmc3_UpdateChr2p(unsigned int addr,unsigned int bank);
unsigned int Mmc3_GetChrSource(unsigned int);
void Mmc3_HSync(uint32 scanline);

void Mmc3_SwapChr1K(uint8 page,uint32 bank);
void Mmc3_SwapChr2K(uint8 page,uint32 bank);

extern void (*UpdateChr)(unsigned int,unsigned int);
extern void (*UpdatePrg)(unsigned int,unsigned int);
extern unsigned int (*GetChrSource)(unsigned int);

extern void (*Poke_8000)(uint32 address,uint8 data);
extern void (*Poke_8001)(uint32 address,uint8 data);
extern void (*Poke_A000)(uint32 address,uint8 data);
extern void (*Poke_A001)(uint32 address,uint8 data);
extern void (*Poke_C000)(uint32 address,uint8 data);
extern void (*Poke_C001)(uint32 address,uint8 data);
extern void (*Poke_E000)(uint32 address,uint8 data);
extern void (*Poke_E001)(uint32 address,uint8 data);


