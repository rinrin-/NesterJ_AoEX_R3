//////////////////////////////////////////////////////////////////////////
// Mapper132                                        //
//////////////////////////////////////////////////////////////////////////
// Mapper 132
void NES_mapper132_Init()
{

	g_NESmapper.Mapper132.is172=0;  
	g_NESmapper.Mapper132.is173=0;
	g_NESmapper.Reset = NES_mapper132_Reset;
	g_NESmapper.MemoryWrite = NES_mapper132_MemoryWrite;
	g_NESmapper.MemoryWriteLow = NES_mapper132_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper132_MemoryReadLow;
}

void NES_mapper172_Init()
{
	NES_mapper132_Init();
	g_NESmapper.Mapper132.is172=1;  
}

void NES_mapper173_Init()
{
	NES_mapper132_Init();
	g_NESmapper.Mapper132.is173=1;
}

void NES_mapper132_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( (addr>=0x4100)&&(addr<=0x4103) )
		g_NESmapper.Mapper132.regs[addr & 0x3] = data;
}

uint8 NES_mapper132_MemoryReadLow(uint32 addr)
{
	if(addr==0x4100)
	{
		return (g_NESmapper.Mapper132.regs[1]^g_NESmapper.Mapper132.regs[2])|(g_NESmapper.Mapper132.is173?0x01:0x40);
	}
	return (uint8)(addr>>8);
	
}

void NES_mapper132_Reset()
{
	int i;
	for (i=0; i < 4; ++i)
		g_NESmapper.Mapper132.regs[i] = 0;

	g_NESmapper.SetPROM_32K_Bank((g_NESmapper.Mapper132.regs[2]>>2)&1);
	g_NESmapper.SetVROM_8K_Bank(g_NESmapper.Mapper132.regs[2]&3);
}

void NES_mapper132_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper132.cmd=data;
	
	g_NESmapper.SetPROM_32K_Bank((g_NESmapper.Mapper132.regs[2]>>2)&1);
	if(g_NESmapper.Mapper132.is172)
		g_NESmapper.SetVROM_8K_Bank((((g_NESmapper.Mapper132.cmd^g_NESmapper.Mapper132.regs[2])>>3)&2)|(((g_NESmapper.Mapper132.cmd^g_NESmapper.Mapper132.regs[2])>>5)&1)); 
	else
		g_NESmapper.SetVROM_8K_Bank(g_NESmapper.Mapper132.regs[2]&3);
	
}
/////////////////////////////////////////////////////////////////////

