//Gs2015
void Gs2015_Reset();
void Gs2015_MemoryWrite(uint32 addr, uint8 data);
void Gs2015_Init();


//Gs2013
void Gs2013_Reset();
void Gs2013_MemoryWrite(uint32 addr, uint8 data);
void Gs2013_MemoryReadSaveRAM(uint32 addr);
void Gs2013_Init();


//Gs2004
void Gs2004_Reset();
void Gs2004_MemoryWrite(uint32 addr, uint8 data);
void Gs2004_MemoryReadSaveRAM(uint32 addr);
void Gs2004_Init();

					
//TetrisFamily
void TetrisFamily_Reset();
void TetrisFamily_MemoryWrite(uint32 addr, uint8 data);
void TetrisFamily_Init();