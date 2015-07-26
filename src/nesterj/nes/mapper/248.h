
/////////////////////////////////////////////////////////////////////
// Mapper 248
void NES_mapper248_Reset();
void NES_mapper248_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper248_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper248_HSync(uint32 scanline);
void NES_mapper248_MMC3_set_CPU_banks();
void NES_mapper248_MMC3_set_PPU_banks();
void NES_mapper248_SNSS_fixup(); // HACK HACK HACK HACK

void NES_mapper248_Init();
/////////////////////////////////////////////////////////////////////

