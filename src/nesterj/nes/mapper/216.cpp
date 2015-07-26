
/////////////////////////////////////////////////////////////////////
// Mapper 216


void NES_mapper216_Init()
{
	g_NESmapper.Reset = NES_mapper216_Reset;
	g_NESmapper.MemoryWrite = NES_mapper216_MemoryWrite;
}



void NES_mapper216_Reset()
{
	//NES_mapper216_MemoryWrite(0x8000,0);
}


void NES_mapper216_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.SetPROM_32K_Bank(A);
	g_NESmapper.SetVROM_8K_Bank(A>>1);	
}
