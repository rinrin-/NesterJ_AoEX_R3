
/////////////////////////////////////////////////////////////////////
// Mapper 162
void NES_mapper162_Reset();
void NES_mapper162_MemoryWriteLow(uint32 addr, uint8 data);

void NES_mapper162_MMC3_set_CPU_banks();
void NES_mapper162_MMC3_set_PPU_banks();

void NES_mapper162_HSync(uint32 scanline);
void NES_mapper162_Init();
/////////////////////////////////////////////////////////////////////

