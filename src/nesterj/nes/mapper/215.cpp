
/////////////////////////////////////////////////////////////////////
// Mapper 215


void NES_mapper215_Init()
{
	Mmc3_Init();
	UpdateChr = NES_mapper215_UpdateChr2p;
	UpdatePrg = NES_mapper215_UpdatePrg2p;	
	
	Poke_8000 = Poke_M215_8000;
	Poke_8001 = Poke_M215_8001;
	Poke_A000 = Poke_M215_A000;
	Poke_C000 = Poke_M215_C000;
	Poke_C001 = Poke_M215_C001;
	Poke_E001 = Poke_M215_E001;	
	
	g_NESmapper.Reset = NES_mapper215_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper215_MemoryWriteLow;
	//g_NESmapper.MemoryWrite = NES_mapper215_MemoryWrite;
	
	if( (NES_ROM_get_num_16k_ROM_banks()==16) &&(NES_ROM_get_num_8k_VROM_banks()==64) )
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper215_MemoryWriteSaveRAM;
}

void NES_mapper215_Reset()
{
	g_NESmapper.Mapper215.exRegs[0] = 0x00;
	g_NESmapper.Mapper215.exRegs[1] = 0xFF;
	g_NESmapper.Mapper215.exRegs[2] = 0x04;
	g_NESmapper.Mapper215.exRegs[3] = 0;
	Mmc3_Reset();
}

void Poke_M215_5000(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[0] != V)
	{
		g_NESmapper.Mapper215.exRegs[0] = V;
		NES_mapper215_UpdatePrg();
	}
}
void Poke_M215_5001(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[1] != V)
	{
		g_NESmapper.Mapper215.exRegs[1] = V;
		Mmc3_UpdateChr();
	}
}
void Poke_M215_5007(uint32 A,uint8 V)
{
	g_NESmapper.Mmc3.ctrl0 = 0;

	if (g_NESmapper.Mapper215.exRegs[2] != V)
	{
		g_NESmapper.Mapper215.exRegs[2] = V;
		NES_mapper215_UpdatePrg();
		Mmc3_UpdateChr();
	}
}

void Poke_M215_8000(uint32 A,uint8 V)
{
	if (!g_NESmapper.Mapper215.exRegs[2])
		Poke_Mmc3_8000(0x8000,V);
}
void Poke_M215_8001(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[2])
	{
		if (g_NESmapper.Mapper215.exRegs[3] && ((g_NESmapper.Mapper215.exRegs[0] & 0x80U) == 0 || (g_NESmapper.Mmc3.ctrl0 & 0x7) < 6))
		{
			g_NESmapper.Mapper215.exRegs[3] = 0;
			Poke_Mmc3_8001(0x8001,V);
		}
	}
	else
	{
		Poke_Mmc3_8001(0x8001,V);
	}
}
void Poke_M215_A000(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[2])
	{
		static const uint8 security[8] = {0,2,5,3,6,1,7,4};

		V = (V & 0xC0) | security[V & 0x07];
		g_NESmapper.Mapper215.exRegs[3] = 1;

		Poke_Mmc3_8000(0x8000,V);
	}
	else
	{
		g_NESmapper.set_mirroring2( V );
	}
}
void Poke_M215_C000(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[2])
		g_NESmapper.set_mirroring2( V >> 7 | V );
	else
		Poke_Mmc3_C000(0xC000,V);
}
void Poke_M215_C001(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[2])
		Poke_Mmc3_E001(0xE001,V);
	else
		Poke_Mmc3_C001(0xC001,V);
}
void Poke_M215_E001(uint32 A,uint8 V)
{
	if (g_NESmapper.Mapper215.exRegs[2])
	{
		Poke_Mmc3_C000(0xC000,V);
		Poke_Mmc3_C001(0xC001,V);
	}
	else
	{
		Poke_Mmc3_E001(0xE001,V);
	}
}


void NES_mapper215_UpdatePrg()
{
	if (g_NESmapper.Mapper215.exRegs[0] & 0x80U)
	{
		unsigned int bank = (g_NESmapper.Mapper215.exRegs[0] & 0x0FU) | (g_NESmapper.Mapper215.exRegs[1] & 0x10U);
		g_NESmapper.SetPROM_16K_Bank(4,bank);
		g_NESmapper.SetPROM_16K_Bank(6,bank);
	}
	else
	{
		Mmc3_UpdatePrg();
	}
}

void NES_mapper215_UpdatePrg2p(unsigned int A,unsigned int V)
{
	if (!(g_NESmapper.Mapper215.exRegs[0] & 0x80U))
	{
		if (g_NESmapper.Mapper215.exRegs[1] & 0x8U)
			V = (V & 0x1F) | 0x20;
		else
			V = (V & 0x0F) | (g_NESmapper.Mapper215.exRegs[1] & 0x10U);

		g_NESmapper.SetPROM_8K_Bank((A>>13)+4,V);
	}
}

void NES_mapper215_UpdateChr2p(unsigned int A,unsigned int V)
{
	if (g_NESmapper.Mapper215.exRegs[1] & 0x4U)
		V = (V | 0x100);
	else
		V = (V & 0x7F) | (g_NESmapper.Mapper215.exRegs[1] << 3 & 0x80U);

	g_NESmapper.SetVROM_1K_Bank(A>>10,V);
}

void NES_mapper215_MemoryWriteLow(uint32 A,uint8 V)
{	
	switch(A)
	{
		case 0x5000: Poke_M215_5000(A,V);break;
		case 0x5001: Poke_M215_5001(A,V);break;
		case 0x5007: Poke_M215_5007(A,V);break;
	}
}
void NES_mapper215_MemoryWriteSaveRAM(uint32 A, uint8 V)
{
	//if(board == Type::SUPERGAME_MK3E)
	//	return;
		
	switch(A)
	{
		case 0x6000:Poke_M215_5000(A,V);break;
		case 0x6001:Poke_M215_5001(A,V);break;
		case 0x6007:Poke_M215_5007(A,V);break;
	}	
}

void NES_mapper215_MemoryWrite(uint32 A, uint8 V)
{
	switch(A&0xF001)
	{
		case 0x8000:Poke_M215_8000(A,V);break;
		case 0x8001:Poke_M215_8001(A,V);break;
		case 0xA000:Poke_M215_A000(A,V);break;
		case 0xC000:Poke_M215_C000(A,V);break;
		case 0xC001:Poke_M215_C001(A,V);break;
		case 0xE001:Poke_M215_E001(A,V);break;
	}
}