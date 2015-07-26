/////////////////////////////////////////////////////////////////////
//S74x374a
void S74x374a_Init();
void S74x374a_Reset();

void S74LS374NASynco(void);
void S74x374a_MemoryWriteLow(uint32 addr, uint8 data);
void S74x374a_UpdateNmt(unsigned int);



/////////////////////////////////////////////////////////////////////
//S74x374b
void S74x374b_Init();
void S74x374b_Reset();
void S74x374b_MemoryWriteLow(uint32 addr, uint8 data);
void S74LS374NSynco(void);
uint8 S74x374b_MemoryReadLow( uint32 addr );



//SACHEN 8259A, Mapper 141
void S8259A_Init();
void S8259_Reset();
void S8259Synco(void);
void S8259_MemoryWriteLow(uint32 addr, uint8 data);


//SACHEN SA0161M, Mapper 146
void SA0161M_Init();
void SA0161M_Reset();
void SA0161MSynco();
void SA0161M_MemoryWriteLow(uint32 addr, uint8 data);


//SACHEN TCU01, Mapper 147
void TCU01_Init();
void TCU01_Reset();
void TCU01Synco();
void TCU01_MemoryWriteLow(uint32 addr, uint8 data);


//AVE 74*161 / SACHEN SA0037, Mapper 148
void SA0037_Init();
void SA0037_Reset();
void SA0037_MemoryWrite(uint32 addr, uint8 data);

//S74LS374N_Init mapper 150
void S74LS374N_Init();
void S74LS374N_Reset();
void S74LS374N_MemoryWriteLow(uint32 addr, uint8 data);
uint8 S74LS374N_MemoryReadLow( uint32 addr );






