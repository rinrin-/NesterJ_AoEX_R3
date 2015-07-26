
/////////////////////////////////////////////////////////////////////
// Mapper 164
void NES_mapper164_Reset();

void NES_mapper164_MemoryWriteLow(uint32 addr, uint8 data);

void NES_mapper164_PPU_ExtLatchX( int x );
void NES_mapper164_PPU_ExtLatch( uint32 addr, uint8 chr_l, uint8 chr_h, uint8 attr );

void NES_mapper164_MMC3_set_CPU_banks();
void NES_mapper164_MMC3_set_PPU_banks();

void NES_mapper164_Init();
/////////////////////////////////////////////////////////////////////


void NES_mapper164_Reset();
//void NES_mapper164_UpdatePrg();
void NES_mapper164_Init();
void NES_mapper164_MemoryWriteLow(uint32 addr, uint8 data);
