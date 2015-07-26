
/////////////////////////////////////////////////////////////////////
// Mapper 245
void NES_mapper245_Reset();
void NES_mapper245_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper245_HSync(uint32 scanline);

void NES_mapper245_SetBank_CPU();
void NES_mapper245_SetBank_PPU();
void NES_mapper245_Init();
/////////////////////////////////////////////////////////////////////

