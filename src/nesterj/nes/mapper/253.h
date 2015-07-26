
/////////////////////////////////////////////////////////////////////
// Mapper 253
void NES_mapper253_Reset();
void NES_mapper253_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper253_HSync(uint32 scanline);
void NES_mapper253_SetBank_PPUSUB( int bank, int page );
void NES_mapper253_set_PPU_bank( uint8 bank, uint32 page );
void NES_mapper253_Init();
/////////////////////////////////////////////////////////////////////

