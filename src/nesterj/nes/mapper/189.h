
/////////////////////////////////////////////////////////////////////
// Mapper 189
void NES_mapper189_Reset();
void NES_mapper189_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper189_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper189_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper189_HSync(uint32 scanline);

void NES_mapper189_MMC3_set_PPU_banks();

void NES_mapper189_Init();
/////////////////////////////////////////////////////////////////////

