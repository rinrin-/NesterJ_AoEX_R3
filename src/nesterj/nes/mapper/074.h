
/////////////////////////////////////////////////////////////////////
// Mapper 074
void NES_mapper074_Reset();
void NES_mapper074_MemoryWrite(uint32 addr, uint8 data);
uint8 NES_mapper074_MemoryReadLow(uint32 addr);
void NES_mapper074_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper074_HSync(uint32 scanline);

void (*NES_WaiXing_MMC3_set_PPU_banks)();

void NES_mapper074_MMC3_set_CPU_banks();
void NES_mapper074_MMC3_set_PPU_banks();
void NES_mapper074_MMC3_set_PPUSUB( int bank, int page ,int bRAM);

void NES_mapper74_Init();
/////////////////////////////////////////////////////////////////////
// 191
void NES_mapper191_Init();
void NES_mapper191_MMC3_set_PPU_banks();

/////////////////////////////////////////////////////////////////////
// 192
void NES_mapper192_Init();
void NES_mapper192_MMC3_set_PPU_banks();

/////////////////////////////////////////////////////////////////////
// 194
void NES_mapper194_Init();
void NES_mapper194_MMC3_set_PPU_banks();

/////////////////////////////////////////////////////////////////////
// 195
void NES_mapper195_Init();
void NES_mapper195_MMC3_set_PPU_banks();



/////////////////////////////////////////////////////////////////////
//198
void NES_mapper198_Init();
void NES_mapper198_Reset();
void NES_mapper198_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper198_MMC3_set_CPU_banks();
void NES_mapper198_MMC3_set_PPU_banks();