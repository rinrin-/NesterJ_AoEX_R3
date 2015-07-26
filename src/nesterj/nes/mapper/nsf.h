
/////////////////////////////////////////////////////////////////////
// Mapper NSF - private mapper number = 12 (decimal)
void NES_mapperNSF_Reset();
void NES_mapperNSF_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapperNSF_MemoryWriteSaveRAM(uint32 addr, uint8 data);
void NES_mapperNSF_MemoryWrite(uint32 addr, uint8 data);
uint8 NES_mapperNSF_MemoryReadLow(uint32 addr);
void NES_mapperNSF_BankSwitch(uint8 num, uint8 bank);
void NES_mapperNSF_LoadPlayer();

void NES_mapperNSF_Init();
/////////////////////////////////////////////////////////////////////

