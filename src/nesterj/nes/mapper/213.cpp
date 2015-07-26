
/////////////////////////////////////////////////////////////////////
// Mapper 213


void NES_mapper213_Init()
{
	g_NESmapper.Reset = NES_mapper213_Reset;
	g_NESmapper.MemoryWrite = NES_mapper213_MemoryWrite;
}



void NES_mapper213_Reset()
{
	NES_mapper213_MemoryWrite(0x8000,0);
}


void NES_mapper213_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.SetPROM_32K_Bank(A >> 1);
	g_NESmapper.SetVROM_8K_Bank(A >> 3);	
}
