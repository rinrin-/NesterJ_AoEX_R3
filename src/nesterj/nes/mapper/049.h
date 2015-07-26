
/////////////////////////////////////////////////////////////////////
// Mapper 49
void NES_mapper49_Reset();
void NES_mapper49_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper49_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper49_HSync(uint32 scanline);
void NES_mapper49_MMC3_set_CPU_banks();
void NES_mapper49_MMC3_set_PPU_banks();
void NES_mapper49_SNSS_fixup();

void NES_mapper49_Init();
/////////////////////////////////////////////////////////////////////

