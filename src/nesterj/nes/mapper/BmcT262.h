

/////////////////////////////////////////////////////////////////////
// BmcT262
void BmcT262_Reset();

void BmcT262_Init();
void BmcT262_MemoryWrite(uint32 addr, uint8 data);
void BmcT262_Sync();
/////////////////////////////////////////////////////////////////////

//Bmc8157
void Bmc8157_Reset();

void Bmc8157_Init();
void Bmc8157_MemoryWrite(uint32 addr, uint8 data);
void Bmc8157_MemoryReadSaveRAM(uint32 addr);