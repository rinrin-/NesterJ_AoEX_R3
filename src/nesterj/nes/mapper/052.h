
/////////////////////////////////////////////////////////////////////
// Mapper 52
void NES_mapper52_Reset();
void NES_mapper52_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper52_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper52_HSync(uint32 scanline);
void NES_mapper52_MMC3_set_CPU_banks();
void NES_mapper52_MMC3_set_PPU_banks();
void NES_mapper52_SNSS_fixup(); // HACK HACK HACK HACK

void NES_mapper52_Init();
/////////////////////////////////////////////////////////////////////

