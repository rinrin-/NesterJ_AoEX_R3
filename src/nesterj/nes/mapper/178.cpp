/////////////////////////////////////////////////////////////////////
// Mapper 178


void NES_mapper178_Init()
{
	g_NESmapper.Reset = NES_mapper178_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper178_MemoryWriteLow;	
}


void NES_mapper178_Reset()
{
	g_NESmapper.Mapper178.regs[0]=1;
	g_NESmapper.Mapper178.regs[1]=0;
	g_NESmapper.Mapper178.regs[2]=0;
	g_NESmapper.Mapper178.regs[3]=0;
	NES_mapper178_Sync();	
}

void NES_mapper178_MemoryWriteLow(uint32 A, uint8 V)
{
	if((A>=0x4800)&&(A<=0x4803))
	{
		g_NESmapper.Mapper178.regs[A&3]=V;
		NES_mapper178_Sync();
	}
}

void NES_mapper178_Sync()
{
	uint8 bank = (g_NESmapper.Mapper178.regs[2]&3)<<3;
  
	if( g_NESmapper.Mapper178.regs[0]&1 ) //SetVRAM_Mirror( VRAM_HMIRROR );		
		g_NESmapper.set_mirroring(0,0,1,1);
	else			  //SetVRAM_Mirror( VRAM_VMIRROR );
		g_NESmapper.set_mirroring(0,1,0,1);
				
	//setprg8r(0x10,0x6000,0);
	//g_NESmapper.SetPROM_8K_Bank(3,0);
	g_NESmapper.SetVROM_8K_Bank(0);
	if(g_NESmapper.Mapper178.regs[0]&2)
	{
		g_NESmapper.SetPROM_16K_Bank(4,(g_NESmapper.Mapper178.regs[1]&7)|bank);
		g_NESmapper.SetPROM_16K_Bank(6,((~0)&7)|bank);
	}
	else
	{
		g_NESmapper.SetPROM_16K_Bank(4,(g_NESmapper.Mapper178.regs[1]&6)|bank);
		g_NESmapper.SetPROM_16K_Bank(6,(g_NESmapper.Mapper178.regs[1]&6)|bank|1);
	}
}
/////////////////////////////////////////////////////////////////////
