
/////////////////////////////////////////////////////////////////////
// Mapper 5
void NES_mapper5_Reset();
uint8 NES_mapper5_MemoryReadLow(uint32 addr);
void NES_mapper5_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper5_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper5_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper5_HSync(uint32 scanline);
void NES_mapper5_MMC5_set_CPU_bank(uint8 page, uint8 bank);
void NES_mapper5_MMC5_set_WRAM_bank(uint8 page, uint8 bank);
uint8 NES_mapper5_PPU_Latch_RenderScreen(uint8 mode, uint32 addr);
void NES_mapper5_sync_Chr_banks(uint8 mode);

void NES_mapper5_Init();
/////////////////////////////////////////////////////////////////////

