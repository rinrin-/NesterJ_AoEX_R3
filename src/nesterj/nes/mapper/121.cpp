

void NES_mapper121_Init()
{
	Mmc3_Init();
	
	UpdatePrg = Mapper121_UpdatePrg;
	
	g_NESmapper.Reset = NES_mapper121_Reset;
	g_NESmapper.MemoryReadLow  = NES_mapper121_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper121_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper121_MemoryWrite;
}
void NES_mapper121_Reset()
{
	g_NESmapper.Mapper121.exRegs[0] = 0;
	g_NESmapper.Mapper121.exRegs[1] = 0;
	g_NESmapper.Mapper121.exRegs[2] = 0;	
	Mmc3_Reset();		
}

void Mapper121_UpdatePrg(unsigned int addr,unsigned int bank)
{
	if (addr == 0x4000)
	{
		if (g_NESmapper.Mapper121.exRegs[0])
			bank = g_NESmapper.Mapper121.exRegs[0];
	}
	else if (addr == 0x6000)
	{
		if (g_NESmapper.Mapper121.exRegs[1])
			bank = g_NESmapper.Mapper121.exRegs[1];
	}

	//prg.SwapBank<SIZE_8K>( address, bank );
	g_NESmapper.SetPROM_8K_Bank( (addr>>13)+4, bank );	
}

void NES_mapper121_MemoryWriteLow(uint32 addr, uint8 data)
{
	static const uint8 lut[] =
	{
		0x00, 0x83, 0x42, 0x00
	};

	if( (addr>=0x5000)&&(addr<=0x5fff) )
		g_NESmapper.Mapper121.exRegs[2] = lut[data & 0x3];
}

uint8 NES_mapper121_MemoryReadLow(uint32 addr)
{
	if( (addr>=0x5000)&&(addr<=0x5fff) )
			return g_NESmapper.Mapper121.exRegs[2];
	return (uint8)(addr>>8);
}

void NES_mapper121_MemoryWrite(uint32 address, uint8 data)
{
	if( (address>=0x8000)&&(address<=0x9fff) )
	{
		if ((address & 0x3) == 0x3)
		{
			switch (data)
			{
				case 0x28: g_NESmapper.Mapper121.exRegs[0] = 0x0C; break;
				case 0x26: g_NESmapper.Mapper121.exRegs[1] = 0x08; break;
				case 0xAB: g_NESmapper.Mapper121.exRegs[1] = 0x07; break;
				case 0xEC: g_NESmapper.Mapper121.exRegs[1] = 0x0D; break;
				case 0xEF: g_NESmapper.Mapper121.exRegs[1] = 0x0D; break;
				case 0xFF: g_NESmapper.Mapper121.exRegs[1] = 0x09; break;

				case 0x20: g_NESmapper.Mapper121.exRegs[1] = 0x13; break;
				case 0x29: g_NESmapper.Mapper121.exRegs[1] = 0x1B; break;

				default:   g_NESmapper.Mapper121.exRegs[0] = 0x0;
						   g_NESmapper.Mapper121.exRegs[1] = 0x0; break;
			}
		}
		else if (address & 0x1)
		{
			Poke_Mmc3_8001(address,data);
		}
		else
		{
			Poke_Mmc3_8000(address,data);
		}
		Mmc3_UpdatePrg();	
	}
	else
	{
		Mmc3_MemoryWrite(address,data);
	}
}