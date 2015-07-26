

/////////////////////////////////////////////////////////////////////
//   KAISER KS-7058, Mapper 171

void Ks7058_Init()
{
	g_NESmapper.Reset = Ks7058_Reset;
	g_NESmapper.MemoryWrite = Ks7058_MemoryWrite;
}

void Ks7058_Reset()
{
	g_NESmapper.SetPROM_32K_Bank(0);
	g_NESmapper.SetVROM_8K_Banks(0,1,2,3,4,5,6,7);
}

void Ks7058_MemoryWrite(uint32 addr, uint8 data)
{
	if( addr>=0xF000 )
	{
		if( (addr&0xff)<0x80 )
		{
			g_NESmapper.SetVROM_4K_Bank( 0, data );
		}
		else
		{
			g_NESmapper.SetVROM_4K_Bank( 4, data );
		}
	}
}
/////////////////////////////////////////////////////////////////////

