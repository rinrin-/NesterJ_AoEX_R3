
/////////////////////////////////////////////////////////////////////
// Mapper 20
void NES_mapper20_Reset();
uint8 NES_mapper20_MemoryReadLow(uint32 addr);
void NES_mapper20_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper20_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper20_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper20_HSync(uint32 scanline);
void NES_mapper20_VSync();
void NES_mapper20_SNSS_fixup();

uint8 NES_mapper20_GetDiskSideNum();
uint8 NES_mapper20_GetDiskSide();
void  NES_mapper20_SetDiskSide(uint8 side);
uint8 *NES_mapper20_GetDiskDatap();
uint8 NES_mapper20_DiskAccessed();

void NES_mapper20_Init();
/////////////////////////////////////////////////////////////////////

