/////////////////////////////////////////////////////////////////////
// GeniusMerioBros
void GeniusMerioBros_Init()
{
	g_NESmapper.Reset = GeniusMerioBros_Reset;
	g_NESmapper.MemoryWriteSaveRAM = GeniusMerioBros_MemoryWriteSaveRAM;
	g_NESmapper.MemoryReadSaveRAM = GeniusMerioBros_MemoryReadSaveRAM;
}

void GeniusMerioBros_Reset()
{
	g_NESmapper.SetPROM_32K_Bank(0);
	g_NESmapper.SetVROM_8K_Bank(0);
	
	g_NESmapper.Mapper.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;	
	_memcpy(g_NESmapper.Mapper.wram,g_NESmapper.ROM_banks+0x2000*4,0x800);
	g_NESmapper.SetPROM_Bank( 3, g_NESmapper.Mapper.wram);	
}

void GeniusMerioBros_MemoryWriteSaveRAM(uint32 A, uint8 V)
{	
	if( A >= 0x7000 && A <= 0x7FFF ) {
		g_NESmapper.Mapper.xram[A&0x7FF] = V;
	}
}

void GeniusMerioBros_MemoryReadSaveRAM(uint32 A)
{	
	nes6502_context cpu;
	NES6502_GetContext(&cpu);	
	
	if( A >= 0x6000 && A <= 0x6FFF )
	{
		cpu.mem_page[A>>13][A&NES6502_BANKMASK]=cpu.mem_page[4][A&0x7FF];
	}else	
	if( A >= 0x7000 && A <= 0x7FFF ) {
		cpu.mem_page[A>>13][A&NES6502_BANKMASK]=g_NESmapper.Mapper.xram[A&0x7FF];
	}NES6502_SetContext(&cpu);
}