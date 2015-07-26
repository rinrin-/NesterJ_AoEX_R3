
/////////////////////////////////////////////////////////////////////
// Mapper 90
void NES_mapper90_Reset();
uint8 NES_mapper90_MemoryReadLow(uint32 addr);
void NES_mapper90_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper90_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper90_HSync(uint32 scanline);

void NES_mapper90_SetBank_CPU();
void NES_mapper90_SetBank_PPU();
void NES_mapper90_SetBank_VRAM();

void NES_mapper90_Init();
/////////////////////////////////////////////////////////////////////

