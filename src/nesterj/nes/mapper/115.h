
/////////////////////////////////////////////////////////////////////
// Mapper 115
void NES_mapper115_Reset();
void NES_mapper115_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper115_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper115_HSync(uint32 scanline);

void NES_mapper115_SetBank_CPU();
void NES_mapper115_SetBank_PPU();
	
void NES_mapper115_Init();
/////////////////////////////////////////////////////////////////////

