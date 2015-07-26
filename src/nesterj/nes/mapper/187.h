
/////////////////////////////////////////////////////////////////////
// Mapper 187 kof96
void NES_mapper187_Init();
void NES_mapper187_Reset();

void  NES_mapper187_UpdateChr(unsigned int,unsigned int);
void  NES_mapper187_UpdatePrg(unsigned int,unsigned int);

void NES_mapper187_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper187_MemoryWriteLow(uint32 addr, uint8 data);
uint8 NES_mapper187_MemoryReadLow(uint32 addr);