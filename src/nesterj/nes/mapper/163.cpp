
/////////////////////////////////////////////////////////////////////
// Mapper 163

void NES_mapper163_Init()
{
	g_NESmapper.Reset = NES_mapper163_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper163_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper163_MemoryReadLow;
	//g_NESmapper.MemoryWriteSaveRAM = NES_mapper163_MemoryWriteSaveRAM;
//	g_NESmapper.MemoryReadSaveRAM = NES_mapper163_MemoryReadSaveRAM;
	g_NESmapper.HSync = NES_mapper163_HSync;
}


void NES_mapper163_Reset()
{
	int i;
	for(i=0; i<8; i++)
		g_NESmapper.Mapper163.DRegs[i]=0;
	g_NESmapper.Mapper163.DRegs[1]=0xFF;
	g_NESmapper.Mapper163.laststrobe=1;
	g_NESmapper.Mapper163.cmd=0;
	g_NESmapper.SetCRAM_8K_Bank(0);
	NES_mapper163_Sync();
}

uint8 NES_mapper163_MemoryReadLow(uint32 A)
{
	if( (A>=0x5000)&&(A<=0x5fff) )
	{
		switch (A&0x7700)
		{
			case 0x5100:
				return g_NESmapper.Mapper163.DRegs[2]; 
				break;
			case 0x5500:
				if(g_NESmapper.Mapper163.trigger)
					return g_NESmapper.Mapper163.DRegs[2];
				else
					return 0;
		}
		return 4;
	}
	return  (uint8)(A >> 8);
}


void NES_mapper163_MemoryReadSaveRAM(uint32 A)
{
	if( (A>=0x6000)&&(A<=0x7fff) )
	{
		nes6502_context cpu;
		NES6502_GetContext(&cpu);
		cpu.mem_page[A>>13][A&NES6502_BANKMASK] = g_NESmapper.Mapper163.wram[A-0x6000];
		NES6502_SetContext(&cpu);
	}	
}


void NES_mapper163_MemoryWriteSaveRAM(uint32 A, uint8 V)
{
	if( (A>=0x6000)&&(A<=0x7fff) )
		g_NESmapper.Mapper163.wram[A-0x6000]=V;
}

void NES_mapper163_MemoryWriteLow(uint32 A, uint8 V)
{
	if( (A>=0x4020)&&(A<=0x5fff) )
	{
		if(A==0x5101)
		{
			if(g_NESmapper.Mapper163.laststrobe&&!V)
			{
				g_NESmapper.Mapper163.trigger^=1;
			}
			g_NESmapper.Mapper163.laststrobe=V;
		}else if(A==0x5100&&V==6) //damn thoose protected games
			g_NESmapper.SetPROM_32K_Bank(3);
		else
		switch (A&0x7300)
		{
			case 0x5200: 
				g_NESmapper.Mapper163.DRegs[0]=V; 		
				NES_mapper163_Sync();
				break;
			case 0x5000: 
				g_NESmapper.Mapper163.DRegs[1]=V; 		
				NES_mapper163_Sync(); 
				if(!(g_NESmapper.Mapper163.DRegs[1]&0x80)&&(NES_PPU_GetScanlineNo()<128)) 
					g_NESmapper.SetCRAM_8K_Bank(0);
				break;
			case 0x5300:
				g_NESmapper.Mapper163.DRegs[2]=V; 
				break;
		}
	}
}

void NES_mapper163_Sync()
{
	g_NESmapper.SetPROM_32K_Bank((g_NESmapper.Mapper163.DRegs[0]<<4)|(g_NESmapper.Mapper163.DRegs[1]&0xF));
}

void NES_mapper163_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper163.DRegs[1]&0x80)
	if( NES_PPU_spr_enabled() || NES_PPU_bg_enabled() )
	{
		if(scanline==239)
		{
			g_NESmapper.SetCRAM_4K_Bank(0,0);
			g_NESmapper.SetCRAM_4K_Bank(4,0);
		}
		else if(scanline==128)//127
		{
			g_NESmapper.SetCRAM_4K_Bank(0,1);
			g_NESmapper.SetCRAM_4K_Bank(4,1);
		}
	}	
}
