
/////////////////////////////////////////////////////////////////////
// Mapper 205
void NES_mapper205_Reset();
void NES_mapper205_MemoryWriteLow(uint32 addr, uint8 data);

void NES_mapper205_Init();
/////////////////////////////////////////////////////////////////////


void NES_mapper205_UpdatePrg(unsigned int addr,unsigned int bank);
void NES_mapper205_UpdateChr(unsigned int addr,unsigned int bank);
