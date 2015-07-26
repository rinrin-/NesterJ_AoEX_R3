
/////////////////////////////////////////////////////////////////////
// Mapper 43
void NES_mapper43_Reset();
uint8 NES_mapper43_MemoryReadLow(uint32 addr);
void NES_mapper43_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper43_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper43_HSync(uint32 scanline);

void NES_mapper43_Init();
/////////////////////////////////////////////////////////////////////

