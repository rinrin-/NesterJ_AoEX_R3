
/////////////////////////////////////////////////////////////////////
// Mapper 217
void NES_mapper217_Reset();
void NES_mapper217_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper217_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper217_HSync(uint32 scanline);
void NES_mapper217_MMC3_set_CPU_banks();
void NES_mapper217_MMC3_set_PPU_banks();


void NES_mapper217_Init();
/////////////////////////////////////////////////////////////////////

