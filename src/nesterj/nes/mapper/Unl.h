//UNL-H2288:KAY_H2288-Mapper 123
//Base On MMC3					
void H2288_Reset();
uint8 H2288_MemoryReadLow(uint32 A);//0x5000~0x5FFF
void H2288_MemoryWriteLow(uint32 A, uint8 V);//0x5800~0x5FFF
void Poke_H2288_8000(uint32 A,uint8 V);//0x8000+=2~<0x9000
void H2288_UpdatePrg(unsigned int A, unsigned int V);
void H2288_Init();


//UNL-EDU2000:UNL_EDU2000
void EDU2000_Reset();
void EDU2000_Init();
void EDU2000_MemoryReadSaveRAM(uint32 A);//0x6000~0x7FFF
void EDU2000_MemoryWriteSaveRAM(uint32 A, uint8 V);//0x6000~0x7FFF
void EDU2000_MemoryWrite(uint32 A, uint8 V);//0x8000~0xFFFF


//UNL-8237:SUPERGAME_POCAHONTAS2
//Base On MMC3
void Pocahontas2_Reset();
void Pocahontas2_Init();
void Pocahontas2_UpdateChr(unsigned int A, unsigned int V);
void Pocahontas2_UpdatePrg(unsigned int A, unsigned int V);
void Pocahontas2_MemoryWriteLow(uint32 A, uint8 V);//5000,5001
void Pocahontas2_MemoryWrite(uint32 A, uint8 V);













