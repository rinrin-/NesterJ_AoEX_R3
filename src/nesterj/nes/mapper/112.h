
/////////////////////////////////////////////////////////////////////
// Mapper 112
void NES_mapper112_Reset();
void NES_mapper112_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper112_HSync(uint32 scanline);
void NES_mapper112_MMC3_set_CPU_banks();
void NES_mapper112_MMC3_set_PPU_banks();
void NES_mapper112_SNSS_fixup();

void NES_mapper112_Init();
/////////////////////////////////////////////////////////////////////

