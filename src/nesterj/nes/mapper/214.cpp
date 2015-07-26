
/////////////////////////////////////////////////////////////////////
// Mapper 214


void NES_mapper214_Init()
{
	g_NESmapper.Reset = NES_mapper214_Reset;
	g_NESmapper.MemoryWrite = NES_mapper214_MemoryWrite;
}



void NES_mapper214_Reset()
{
	NES_mapper214_MemoryWrite(0x8000,0);
}


void NES_mapper214_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.SetPROM_16K_Bank(4,A >> 2);
	g_NESmapper.SetPROM_16K_Bank(6,A >> 2);
	g_NESmapper.SetVROM_8K_Bank(A);	
}
