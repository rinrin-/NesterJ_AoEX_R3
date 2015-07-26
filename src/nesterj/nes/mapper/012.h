
/////////////////////////////////////////////////////////////////////
// Mapper 12
void NES_mapper12_Reset();
uint8 NES_mapper12_MemoryReadLow(uint32 addr);
void NES_mapper12_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper12_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper12_HSync(uint32 scanline);
void NES_mapper12_MMC3_set_CPU_banks();
void NES_mapper12_MMC3_set_PPU_banks();

void NES_mapper12_Init();
/////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

