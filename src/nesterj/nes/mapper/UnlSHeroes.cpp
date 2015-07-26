/////////////////////////////////////////////////////////////////////
// 
void UnlSHeroes_Init()
{
	Mmc3_Init();
	
	UpdateChr = UnlSHeroes_UpdateChr;
	g_NESmapper.Reset = UnlSHeroes_Reset;
	g_NESmapper.MemoryWriteLow = UnlSHeroes_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = UnlSHeroes_MemoryReadLow;
	g_NESmapper.UnlSHeroes.tekker=0x00;
}

void UnlSHeroes_Reset()
{
	Mmc3_Reset();
	g_NESmapper.UnlSHeroes.exRegs=0;
	g_NESmapper.UnlSHeroes.tekker^=0xFF; 	
}

uint8 UnlSHeroes_MemoryReadLow(uint32 A)
{
	if(A==0x4100)
		return(g_NESmapper.UnlSHeroes.tekker);
	return (uint8)(A>>8);
}
void UnlSHeroes_MemoryWriteLow(uint32 A, uint8 V)
{
	if(A==0x4100)
	{
		g_NESmapper.UnlSHeroes.exRegs=V;
		Mmc3_UpdateChr();
	}
}

void UnlSHeroes_UpdateChr(unsigned int A, unsigned int V)
{
	V = V&0xff;
  if(g_NESmapper.UnlSHeroes.exRegs&0x40)
		//setchr8r(0x10,0);
		g_NESmapper.SetCRAM_8K_Bank(0);
  else
  {
    if(A<0x800)
      //setchr1(A,V|((exRegs&8)<<5));
		g_NESmapper.SetVROM_1K_Bank(A>>10,V|((g_NESmapper.UnlSHeroes.exRegs&8)<<5));
    else if(A<0x1000)
      //setchr1(A,V|((exRegs&4)<<6));
		g_NESmapper.SetVROM_1K_Bank(A>>10,V|((g_NESmapper.UnlSHeroes.exRegs&4)<<6));
    else if(A<0x1800)
      //setchr1(A,V|((exRegs&1)<<8));
		g_NESmapper.SetVROM_1K_Bank(A>>10,V|((g_NESmapper.UnlSHeroes.exRegs&1)<<8));
    else
      //setchr1(A,V|((exRegs&2)<<7));
		g_NESmapper.SetVROM_1K_Bank(A>>10,V|((g_NESmapper.UnlSHeroes.exRegs&2)<<7));
  }
}
/////////////////////////////////////////////////////////////////////
