/////////////////////////////////////////////////////////////////////
// Mapper 201
void NES_mapper201_Init()
{
	g_NESmapper.Reset = NES_mapper201_Reset;
	g_NESmapper.MemoryWrite = NES_mapper201_MemoryWrite;
}

void NES_mapper201_Reset()
{
	NES_mapper201_MemoryWrite(0x8000,0);
}


void NES_mapper201_MemoryWrite(uint32 A, uint8 V)
{
	
    const unsigned int bank = A & 0x3;	
	g_NESmapper.SetPROM_32K_Bank(bank);	
	g_NESmapper.SetVROM_8K_Bank(bank);
}
/////////////////////////////////////////////////////////////////////

