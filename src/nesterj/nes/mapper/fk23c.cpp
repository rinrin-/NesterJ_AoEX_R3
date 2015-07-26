/////////////////////////////////////////////////////////////////////
// fk23c
void fk23c_Init()
{
	Mmc3_Init();
	UpdateChr = fk23c_UpdateChr2p;
	UpdatePrg = fk23c_UpdatePrg2p;
	g_NESmapper.Reset = fk23c_Reset;
	g_NESmapper.MemoryWriteLow = fk23c_MemoryWriteLow;//0x5000U, 0x5FFFU
	g_NESmapper.MemoryWrite = fk23c_MemoryWrite;
}

void fk23c_Reset()
{
	uint8 i;
	for (i=0; i < 8; ++i)
		g_NESmapper.fk23c.exRegs[i] = 0xFF;
		
	if( g_NESmapper.num_16k_ROM_banks <=32)
	//if (prg.Source().Size() <= SIZE_512K)
	{
		for (i=0; i < 4; ++i)
			g_NESmapper.fk23c.exRegs[i] = 0x00;
	}
		
	g_NESmapper.fk23c.unromChr = 0x0;
	g_NESmapper.fk23c.mode = 0;
	//dipswitch++;
	//dipswitch&=7;
  
	Mmc3_Reset();	
	fk23c_UpdatePrg();
	fk23c_UpdateChr();
}

void fk23c_MemoryWrite(uint32 A, uint8 V)
{
	if (g_NESmapper.fk23c.exRegs[0] & 0x40U)
	{
		g_NESmapper.fk23c.unromChr = (g_NESmapper.fk23c.exRegs[0] & 0x30U) ? 0x0 : V & 0x3;
		fk23c_UpdateChr();
	}
	else switch (A & 0xE001)
	{
		case 0x8000: Poke_Mmc3_8000(A,V); break;
		case 0x8001:

			if (g_NESmapper.fk23c.exRegs[3] << 2 & (g_NESmapper.Mmc3.ctrl0 & 0x8))
			{
				g_NESmapper.fk23c.exRegs[4 | (g_NESmapper.Mmc3.ctrl0 & 0x3)] = V;

				fk23c_UpdatePrg();
				fk23c_UpdateChr();
			}
			else
			{
				Poke_Mmc3_8001(A,V);
			}
			break;

		case 0xA000: //SetMirroringHV(V)					
					if(V)
						g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
					else
						g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
					/*g_NESmapper.set_mirroring2(V);*/ break;
		case 0xA001: Poke_Mmc3_A001(A,V); break;
		case 0xC000: Poke_Mmc3_C000(A,V); break;
		case 0xC001: Poke_Mmc3_C001(A,V); break;
		case 0xE000: Poke_Mmc3_E000(A,V); break;
		case 0xE001: Poke_Mmc3_E001(A,V); break;

		default: ;
	}
}
void fk23c_MemoryWriteLow(uint32 A, uint8 V)
{
	if(A>=0x5000)
	{
		if (A & (1U << (g_NESmapper.fk23c.mode+ 4)))
		{
			g_NESmapper.fk23c.exRegs[A & 0x3] = V;

			fk23c_UpdatePrg();
			fk23c_UpdateChr();
		}
	}
}

void fk23c_UpdatePrg2p(unsigned int A,unsigned int V)
{
	if ((g_NESmapper.fk23c.exRegs[0] & 0x7U) - 3 > 1 && (!(g_NESmapper.fk23c.exRegs[3] & 0x2U) || A < 0x4000))
	{
		if (g_NESmapper.fk23c.exRegs[0] & 0x3U)
			V = (V & (0x3FU >> (g_NESmapper.fk23c.exRegs[0] & 0x3U))) | (g_NESmapper.fk23c.exRegs[1] << 1);

		g_NESmapper.SetPROM_8K_Bank((A>>13)+4, V );
	}				
}

void fk23c_UpdateChr2p(unsigned int A,unsigned int V)
{
	if (!(g_NESmapper.fk23c.exRegs[0] & 0x40U) && (!(g_NESmapper.fk23c.exRegs[3] & 0x2U) || (A != 0x400 && A != 0xC00)))
		g_NESmapper.SetVROM_1K_Bank( A>>10, (g_NESmapper.fk23c.exRegs[2] & 0x7FU) << 3 | V );
}


void fk23c_UpdatePrg()
{
	if ((g_NESmapper.fk23c.exRegs[0] & 0x7U) == 4)
	{
		g_NESmapper.SetPROM_32K_Bank( g_NESmapper.fk23c.exRegs[1] >> 1 );
	}
	else if ((g_NESmapper.fk23c.exRegs[0] & 0x7U) == 3)
	{
		g_NESmapper.SetPROM_16K_Bank(4,g_NESmapper.fk23c.exRegs[1]);
		g_NESmapper.SetPROM_16K_Bank(6,g_NESmapper.fk23c.exRegs[1]);
	}
	else
	{
		if (g_NESmapper.fk23c.exRegs[3] & 0x2U)
		{
		g_NESmapper.SetPROM_8K_Bank(6,g_NESmapper.fk23c.exRegs[4]);
		g_NESmapper.SetPROM_8K_Bank(7,g_NESmapper.fk23c.exRegs[5]);
		}

		Mmc3_UpdatePrg();
	}
}

void fk23c_UpdateChr()
{
	if (g_NESmapper.fk23c.exRegs[0] & 0x40U)
	{
		g_NESmapper.SetVROM_8K_Bank(g_NESmapper.fk23c.exRegs[2] | g_NESmapper.fk23c.unromChr);
	}
	else
	{
		if (g_NESmapper.fk23c.exRegs[3] & 0x2U)
		{
			const unsigned int base = (g_NESmapper.fk23c.exRegs[2] & 0x7FU) << 3;

			g_NESmapper.SetVROM_1K_Bank(1,base | g_NESmapper.fk23c.exRegs[6]);
			g_NESmapper.SetVROM_1K_Bank(3,base | g_NESmapper.fk23c.exRegs[7]);
		}

		Mmc3_UpdateChr();
	}
}