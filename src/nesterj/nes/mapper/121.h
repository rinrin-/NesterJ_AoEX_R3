// Mapper 121 Base On mmc3
void NES_mapper121_Init();
void NES_mapper121_Reset();

void Mapper121_UpdatePrg(unsigned int,unsigned int);
uint8 NES_mapper121_MemoryReadLow(uint32 addr);//5000~5FFF
void NES_mapper121_MemoryWriteLow(uint32 addr, uint8 data);

//8000~9FFF
void NES_mapper121_MemoryWrite(uint32 addr, uint8 data);