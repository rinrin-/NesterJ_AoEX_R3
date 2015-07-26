
/////////////////////////////////////////////////////////////////////
// Mapper 16
void NES_mapper16_Reset();
void NES_mapper16_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper16_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapper16_MemoryReadSaveRAM(uint32 addr);
void NES_mapper16_HSync(uint32 scanline);
void NES_mapper16_MemoryWrite2(uint32 addr, uint8 data);
void NES_mapper16_MemoryWrite3(uint32 addr, uint8 data);
void NES_mapper16_SetBarcodeValue(uint32 value_low, uint32 value_high);


void NES_mapper16_Init();
/////////////////////////////////////////////////////////////////////

