/////////////////////////////////////////////////////////////////////
// 
void Unl6035052_Init()
{
	Mmc3_Init();
	g_NESmapper.Reset = Unl6035052_Reset;
	g_NESmapper.MemoryWriteLow = Unl6035052_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = Unl6035052_MemoryReadLow;
}

void Unl6035052_Reset()
{
	Mmc3_Reset();		
}

uint8 Unl6035052_MemoryReadLow(uint32 A)
{
	if(A>=0x4020)
		return g_NESmapper.Unl6035052.exRegs;
	return (uint8)(A>>8);
}
void Unl6035052_MemoryWriteLow(uint32 A, uint8 V)
{
	const uint8 lut[4] = { 0x00, 0x02, 0x02, 0x03 };
	g_NESmapper.Unl6035052.exRegs=lut[V&3];  
}
/////////////////////////////////////////////////////////////////////
