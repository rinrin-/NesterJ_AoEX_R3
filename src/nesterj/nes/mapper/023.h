
/////////////////////////////////////////////////////////////////////
// Mapper 23
void NES_mapper23_Reset();
void NES_mapper23_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper23_HSync(uint32 scanline);
void NES_mapper23_SNSS_fixup();

void NES_mapper23_Init();
/////////////////////////////////////////////////////////////////////

// VCR4
void VCR4_Reset();
void VCR4_MemoryWrite(uint32 addr, uint8 data);
void VCR4_HSync(uint32 scanline);
void VCR4_Init();
