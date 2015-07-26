
/////////////////////////////////////////////////////////////////////
// Mapper 119
void NES_mapper119_Reset();
void NES_mapper119_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper119_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper119_HSync(uint32 scanline);
void NES_mapper119_MMC3_set_CPU_banks();
void NES_mapper119_MMC3_set_PPU_banks();

void NES_mapper119_Init();
/////////////////////////////////////////////////////////////////////

