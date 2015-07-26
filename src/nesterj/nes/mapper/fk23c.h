
/////////////////////////////////////////////////////////////////////
// fk23c
void fk23c_Reset();
void fk23c_Init();

void fk23c_MemoryWriteLow(uint32 addr, uint8 data);//5000~5FFF
void fk23c_MemoryWrite(uint32 addr, uint8 data);//8000~FFFF

void fk23c_UpdatePrg2p(unsigned int addr,unsigned int bank);
void fk23c_UpdateChr2p(unsigned int addr,unsigned int bank);
void fk23c_UpdatePrg();
void fk23c_UpdateChr();
