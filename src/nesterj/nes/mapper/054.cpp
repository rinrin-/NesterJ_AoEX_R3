
/////////////////////////////////////////////////////////////////////
// Mapper 54


void NES_mapper54_Init()
{
	g_NESmapper.Reset = NES_mapper54_Reset;
	g_NESmapper.MemoryWrite = NES_mapper54_MemoryWrite;
}



void NES_mapper54_Reset()
{
	NES_mapper54_MemoryWrite(0x8000,0);
}


void NES_mapper54_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.SetPROM_32K_Bank(A);
	g_NESmapper.SetVROM_8K_Bank(A);	
}
