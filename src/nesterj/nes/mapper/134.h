
/////////////////////////////////////////////////////////////////////
// Mapper 134 Family4646B
void NES_mapper134_Reset();
void NES_mapper134_MemoryWriteSaveRAM(uint32 addr, uint8 data);

void NES_mapper134_Init();
/////////////////////////////////////////////////////////////////////


void NES_mapper134_UpdatePrg(unsigned int addr,unsigned int bank);
void NES_mapper134_UpdateChr(unsigned int addr,unsigned int bank);