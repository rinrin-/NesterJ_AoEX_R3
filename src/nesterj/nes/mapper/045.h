
/////////////////////////////////////////////////////////////////////
// Mapper 45
void NES_mapper45_Reset();
void NES_mapper45_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper45_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper45_HSync(uint32 scanline);
void NES_mapper45_MAP45_set_CPU_bank4(uint8 data);
void NES_mapper45_MAP45_set_CPU_bank5(uint8 data);
void NES_mapper45_MAP45_set_CPU_bank6(uint8 data);
void NES_mapper45_MAP45_set_CPU_bank7(uint8 data);
void NES_mapper45_MAP45_set_PPU_banks();

void NES_mapper45_Init();
/////////////////////////////////////////////////////////////////////
// BMC-SuperHIK8in1  UNIF MAP45 Base On MMC3
