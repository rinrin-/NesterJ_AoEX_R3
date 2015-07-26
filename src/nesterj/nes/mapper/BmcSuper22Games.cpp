//"BMC-42IN1RESETSWITCH",BMC_SUPER_22GAMES
void BmcSupper22Games_Init()
{
	g_NESmapper.Reset = BmcSupper22Games_Reset;
	g_NESmapper.MemoryWrite = BmcSupper22Games_MemoryWrite;
}


void BmcSupper22Games_Reset()
{
	BmcSupper22Games_MemoryWrite(0x8000,0);
}

void BmcSupper22Games_MemoryWrite(uint32 A, uint8 V)
{
	if (V & 0x20)
	{
		g_NESmapper.SetPROM_16K_Bank(4,V & 0x1F);
		g_NESmapper.SetPROM_16K_Bank(6,V & 0x1F);
	}
	else
	{
		g_NESmapper.SetPROM_32K_Bank(  (V >> 1 & 0xF) );
	}

	static const uint8 lut[4][4] =
	{
		{0,0,0,0},
		{0,1,0,1},
		{0,0,1,1},
		{1,1,1,1}
	};

	//g_NESmapper.set_mirroring(1,1,1,1);		
	g_NESmapper.set_mirroring( lut[V >> 6][0],lut[V >> 6][1],lut[V >> 6][2],lut[V >> 6][3] );
}