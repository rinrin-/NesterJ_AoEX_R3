
/////////////////////////////////////////////////////////////////////
// Mapper 215
void NES_mapper215_Reset();

void Poke_M215_5000(uint32 A,uint8 V);
void Poke_M215_5001(uint32 A,uint8 V);
void Poke_M215_5007(uint32 A,uint8 V);
void Poke_M215_8000(uint32 A,uint8 V);
void Poke_M215_8001(uint32 A,uint8 V);
void Poke_M215_A000(uint32 A,uint8 V);
void Poke_M215_C000(uint32 A,uint8 V);
void Poke_M215_C001(uint32 A,uint8 V);
void Poke_M215_E001(uint32 A,uint8 V);
void NES_mapper215_UpdatePrg();
void NES_mapper215_UpdatePrg2p(unsigned int,unsigned int);
void NES_mapper215_UpdateChr2p(unsigned int,unsigned int);

void NES_mapper215_MemoryWriteLow(uint32 addr, uint8 data);
void NES_mapper215_MemoryWrite(uint32 addr, uint8 data);
void NES_mapper215_MemoryWriteSaveRAM(uint32 addr,uint8);

void NES_mapper215_Init();
/////////////////////////////////////////////////////////////////////