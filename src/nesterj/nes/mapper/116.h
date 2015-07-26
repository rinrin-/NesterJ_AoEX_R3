//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Mapper 116
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////
void NES_mapper116_Init();
void	NES_mapper116_Reset();
void	NES_mapper116_MMC3_set_CPU_banks();
void	NES_mapper116_MMC3_set_PPU_banks();
void	NES_mapper116_MMC3_set_Nmt();
void Mapper116_Poke_Vrc2_8000(uint32 address,uint32 data);
void Mapper116_Poke_Vrc2_9000(uint32,uint32 data);
void Mapper116_Poke_Vrc2_B000(uint32 address,uint32 data);
void Mapper116_Poke_Mmc3_8000(uint32 address,uint32 data);
void Mapper116_Poke_Mmc3_A000(uint32 address,uint32 data);
void Mapper116_Poke_Mmc3_C000(uint32 address,uint32 data);
void Mapper116_Poke_Mmc3_E000(uint32 address,uint32);
void Mapper116_Poke_Mmc1_8000(uint32 address,uint32 data);
void NES_mapper116_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper116_MemoryWrite(uint32 address, uint8 data);
void NES_mapper116_HSync(uint32 scanline);

/////////////////////////////////////////////////////////////////////
// mapper 116b base on mmc3
void Mapper116B_MemoryWriteLow(uint32 addr, uint8 data);
void Mapper116B_UpdateChr(unsigned int addr,unsigned int bank);
void Mapper116B_Reset();
void Mapper116B_Init();
