
/////////////////////////////////////////////////////////////////////
// Mapper 163
void NES_mapper163_Reset();
uint8 NES_mapper163_MemoryReadLow(uint32 addr);
void NES_mapper163_MemoryWriteLow(uint32 addr, uint8 data);

void NES_mapper163_MemoryReadSaveRAM(uint32 addr);
void NES_mapper163_MemoryWriteSaveRAM(uint32 addr, uint8 data);

void NES_mapper163_HSync(uint32 scanline);
void NES_mapper163_Sync();

void NES_mapper163_Init();
/////////////////////////////////////////////////////////////////////

