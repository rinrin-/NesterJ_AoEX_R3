
/////////////////////////////////////////////////////////////////////
// Mapper 4
void NES_mapper4_Reset();
uint8 NES_mapper4_MemoryReadLow(uint32 addr);
void NES_mapper4_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper4_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper4_HSync(uint32 scanline);
void NES_mapper4_MMC3_set_CPU_banks();
void NES_mapper4_MMC3_set_PPU_banks();

void NES_mapper4_MMC3_set_PPU_banksSUB( int bank, int page );

void NES_mapper4_Init();
/////////////////////////////////////////////////////////////////////

