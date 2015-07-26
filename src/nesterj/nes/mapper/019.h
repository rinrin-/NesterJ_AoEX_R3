
/////////////////////////////////////////////////////////////////////
// Mapper 19
void NES_mapper19_Reset();
uint8 NES_mapper19_MemoryReadLow(uint32 addr);
void NES_mapper19_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper19_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper19_HSync(uint32 scanline);

void NES_mapper19_Init();
/////////////////////////////////////////////////////////////////////

