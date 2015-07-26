
/////////////////////////////////////////////////////////////////////
// Mapper 165
void NES_mapper165_Reset();

void NES_mapper165_MemoryWrite(uint32 addr, uint8 data);

void NES_mapper165_PPU_ChrLatch(uint32 addr);
void NES_mapper165_MMC3_set_CPU_banks();
void NES_mapper165_MMC3_set_PPU_banks();
void NES_mapper165_MMC3_set_PPUSUB_banks( int bank, int page );

void NES_mapper165_Init();
/////////////////////////////////////////////////////////////////////

