void H2288_Reset()
{
	g_NESmapper.Unl.exRegs[0] = 0;
	g_NESmapper.Unl.exRegs[1] = 0;
	Mmc3_Reset();
}

void H2288_Init()
{
	Mmc3_Init();
	
	UpdatePrg = H2288_UpdatePrg;
	Poke_8000 = Poke_H2288_8000;
	g_NESmapper.Reset = H2288_Reset;
	g_NESmapper.MemoryWriteLow = H2288_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = H2288_MemoryReadLow;
}

void H2288_UpdatePrg(unsigned int A, unsigned int V)
{
	if (!(g_NESmapper.Unl.exRegs[0] & 0x40))
		Mmc3_UpdatePrg2p( A, V );
}

uint8 H2288_MemoryReadLow(uint32 A)//0x5000~0x5FFF
{
	if( (A>=0x5000)&&(A<=0x5FFF) )
		return (A >> 8 & 0xFE) | (((~A & 0x1) & (A >> 8 & 0x1)) ^ 0x1);
	return (uint8)(A>>8);
}

void H2288_MemoryWriteLow(uint32 A, uint8 V)//0x5800~0x5FFF
{
	if( (A<0x5800)||(A>0x5FFF) )
		return;
		
	A &= 0x1;

	if (g_NESmapper.Unl.exRegs[A] != V)
	{
		g_NESmapper.Unl.exRegs[A] = V;

		if (g_NESmapper.Unl.exRegs[0] & 0x40)
		{
			V = (g_NESmapper.Unl.exRegs[0] & 0x5) | (g_NESmapper.Unl.exRegs[0] >> 2 & 0x2) | (g_NESmapper.Unl.exRegs[0] >> 2 & 0x8);
			A = g_NESmapper.Unl.exRegs[0] >> 1 & 0x1;

			g_NESmapper.SetPROM_16K_Bank(4,V & ~A);
			g_NESmapper.SetPROM_16K_Bank(6,V | A);
		}
		else
		{
			Mmc3_UpdatePrg();
		}
	}
}

void Poke_H2288_8000(uint32 A,uint8 V)//0x8000+=2~<0x9000
{
	static const uint8 security[8] = {0,3,1,5,6,7,2,4};
	Poke_Mmc3_8000(A,(V & 0xC0) | security[V & 0x07]);	
}




void EDU2000_Reset()
{
	EDU2000_MemoryWrite(0x8000,0);
	
	g_NESmapper.Unl.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;	
}
void EDU2000_Init()
{
	g_NESmapper.Reset = EDU2000_Reset;
	g_NESmapper.MemoryReadSaveRAM = EDU2000_MemoryReadSaveRAM;
	g_NESmapper.MemoryWriteSaveRAM = EDU2000_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = EDU2000_MemoryWrite;
}
void EDU2000_MemoryReadSaveRAM(uint32 A)//0x6000~0x7FFF
{
	nes6502_context cpu;
	NES6502_GetContext(&cpu);		
	if( A >= 0x6000 && A <= 0x7FFF )
	{
		cpu.mem_page[A>>13][A&NES6502_BANKMASK]=g_NESmapper.Unl.wram[A - 0x6000];
	}
	NES6502_SetContext(&cpu);
}
void EDU2000_MemoryWriteSaveRAM(uint32 A, uint8 V)//0x6000~0x7FFF
{
	g_NESmapper.Unl.wram[A - 0x6000] = V;
}
void EDU2000_MemoryWrite(uint32 A, uint8 V)//0x8000~0xFFFF
{
	g_NESmapper.SetPROM_32K_Bank( V );
	g_NESmapper.SetPROM_8K_Bank(3,V>>6 );
}







void Pocahontas2_Reset()
{	
	g_NESmapper.Unl.exRegs[0] = 0x00;
	g_NESmapper.Unl.exRegs[1] = 0x00;
	g_NESmapper.Unl.exRegs[2] = 0;
	Mmc3_Reset();
}

void Pocahontas2_Init()
{
	Mmc3_Init();
	UpdatePrg = Pocahontas2_UpdatePrg;
	UpdateChr = Pocahontas2_UpdateChr;
	g_NESmapper.Reset = Pocahontas2_Reset;
	g_NESmapper.MemoryWriteLow = Pocahontas2_MemoryWriteLow;
	g_NESmapper.MemoryWrite = Pocahontas2_MemoryWrite;
}
void Pocahontas2_UpdateChr(unsigned int A, unsigned int V)
{
	g_NESmapper.SetVROM_1K_Bank( A>>10, (g_NESmapper.Unl.exRegs[1] << 6 & 0x100) | V );
}

void Pocahontas2_UpdatePrg(unsigned int A, unsigned int V)
{
	if (!(g_NESmapper.Unl.exRegs[0] & 0x80))
		g_NESmapper.SetPROM_8K_Bank( (A>>13)+4, V );
}

void Pocahontas2_MemoryWriteLow(uint32 A, uint8 V)//5000,5001
{
	if(A==0x5000)
	{
		if (g_NESmapper.Unl.exRegs[0] != V)
		{
			g_NESmapper.Unl.exRegs[0] = V;

			if (g_NESmapper.Unl.exRegs[0] & 0x80)
			{
				const unsigned int bank = g_NESmapper.Unl.exRegs[0] & 0xF;

				if (g_NESmapper.Unl.exRegs[0] & 0x20)
					g_NESmapper.SetPROM_32K_Bank( bank >> 1 );
				else{
					g_NESmapper.SetPROM_16K_Bank(4, bank);
					g_NESmapper.SetPROM_16K_Bank(6, bank);
					}
			}
			else
			{
				Mmc3_UpdatePrg();
			}
		}
	}
	else if(A==0x5001)
	{
		if (g_NESmapper.Unl.exRegs[1] != V)
		{
			g_NESmapper.Unl.exRegs[1] = V;
			Mmc3_UpdateChr();
		}
	}
}
void Pocahontas2_MemoryWrite(uint32 A, uint8 V)
{
	if(A<=0x9FFF)
	{	
		if(V >> 7 | V ) 
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else		    
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);			
	}else if(A<=0xBFFF)
	{
		static const uint8 lut[8] = {0,2,6,1,7,3,4,5};

		V = (V & 0xC0) | lut[V & 0x07];
		g_NESmapper.Unl.exRegs[2] = 1;

		Poke_Mmc3_8000(0x8000,V);	
	}else if(A<=0xDFFF)
	{
		if (g_NESmapper.Unl.exRegs[2])
		{
			g_NESmapper.Unl.exRegs[2] = 0;
			Poke_Mmc3_8001(0x8001,V);
		}
	}else if(A<=0xEFFF)
	{
		//E000
		if( A & 0xE001)
		Poke_Mmc3_E000(A,V);
	}else if(A<=0xFFFF)
	{
		Poke_Mmc3_E001(0xE001,V);
		Poke_Mmc3_C000(0xC000,V);
		Poke_Mmc3_C001(0xC001,V);
	}
}
