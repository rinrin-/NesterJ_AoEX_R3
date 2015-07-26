
	
void BmcHero_Init()
{
	Mmc3_Init();
	
	UpdateChr = BmcHero_UpdateChr;
	UpdatePrg = BmcHero_UpdatePrg;	
	g_NESmapper.Reset = BmcHero_Reset;
	g_NESmapper.MemoryWriteSaveRAM = BmcHero_MemoryWriteSaveRAM;
	//g_NESmapper.MemoryReadSaveRAM = BmcHero_MemoryReadSaveRAM;
		
}


void BmcHero_Reset()
{
	uint8 i;
	for (i=0; i < 5; ++i)g_NESmapper.BmcHero.exRegs[i] = 0;

	//g_NESmapper.BmcHero.wram = g_NES.mapper_extram;
	//g_NES.mapper_extramsize = 0x2000;		
	//g_NESmapper.SetPROM_Bank( 2, g_NESmapper.BmcHero.wram);	
	g_NESmapper.SetVROM_8K_Bank(0);
	Mmc3_Reset();	
}


void BmcHero_MemoryReadSaveRAM(uint32 A)
{
	nes6502_context cpu;
	NES6502_GetContext(&cpu);
	cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)g_NESmapper.BmcSV16in1.wram[A-0x6000];
	NES6502_SetContext(&cpu);
	//return g_NESmapper.BmcHero.wram[A - 0x4000];
}
	
void BmcHero_MemoryWriteSaveRAM(uint32 A, uint8 V)
{
	if (g_NESmapper.BmcHero.exRegs[3] & 0x40)
	{
		//NST_VERIFY( wrk.Writable(0) );
		//if (wrk.Writable(0))
		;//	wrk[0][address - 0x6000] = data;
		//g_NESmapper.BmcHero.wram[A - 0x6000]=V;
	}
	else
	{
		g_NESmapper.BmcHero.exRegs[g_NESmapper.BmcHero.exRegs[4]] = V;
		g_NESmapper.BmcHero.exRegs[4] = (g_NESmapper.BmcHero.exRegs[4] + 1) & 0x3;

		Mmc3_UpdatePrg();
		Mmc3_UpdateChr();
	}
}



void BmcHero_UpdatePrg(unsigned int A,unsigned int V)
{
	V&=(g_NESmapper.BmcHero.exRegs[3]&0x3F)^0x3F;
	V|=g_NESmapper.BmcHero.exRegs[1];
	g_NESmapper.SetPROM_8K_Bank((A>>13)+4, V);
}

void BmcHero_UpdateChr(unsigned int A,unsigned int V)
{
	//if (chr.Source().GetType() == Ram::ROM)
	//UNIF ≤ª¥¶¿Ì
	//if( NES_ROM_get_unifmapper_num()!=0xff )
	//if(!UNIFchrrama)
	{
		uint32 NV=V;
		if(g_NESmapper.BmcHero.exRegs[2]&8)
			NV&=(1<<((g_NESmapper.BmcHero.exRegs[2]&7)+1))-1;
		else
			if(g_NESmapper.BmcHero.exRegs[2])
		NV&=0; // hack ;( don't know exactly how it should be
		NV|=g_NESmapper.BmcHero.exRegs[0]|((g_NESmapper.BmcHero.exRegs[2]&0xF0)<<4);
		g_NESmapper.SetVROM_1K_Bank(A>>10,NV);
	}
}
