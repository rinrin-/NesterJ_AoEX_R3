
void BmcSV16in1_MemoryWrite(uint32 addr, uint8 data)
{
	g_NESmapper.BmcSV16in1.regs[1] = data;
	BmcSV16in1_UpdatePrg();
}

void BmcSV16in1_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	g_NESmapper.BmcSV16in1.regs[0] = data;
	BmcSV16in1_UpdatePrg();
	
	if(data & 0x20)
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_VERT);		
}

void BmcSV16in1_MemoryReadSaveRAM(uint32 A)
{
	if(A==0x6000)
	{
		nes6502_context cpu;
		NES6502_GetContext(&cpu);
		cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)g_NESmapper.BmcSV16in1.wram[A-0x6000];
		NES6502_SetContext(&cpu);
	}
	//return wrk[0][address - 0x6000];
}

void BmcSV16in1_UpdatePrg()
{
	const unsigned int r = g_NESmapper.BmcSV16in1.regs[0] << 3 & 0x78;
	
	g_NESmapper.SetPROM_8K_Bank(3,(r << 1 | 0xF) + (g_NESmapper.BmcSV16in1.epromFirst ? 0x4 : 0x0));
	
    g_NESmapper.SetPROM_16K_Bank(4,
		(g_NESmapper.BmcSV16in1.regs[0] & 0x10) ? (r | (g_NESmapper.BmcSV16in1.regs[1] & 0x7)) + (g_NESmapper.BmcSV16in1.epromFirst ? 0x2 : 0x0) : g_NESmapper.BmcSV16in1.epromFirst ? 0x00 : 0x80);
    g_NESmapper.SetPROM_16K_Bank(6,
		(g_NESmapper.BmcSV16in1.regs[0] & 0x10) ? (r | (0xFF    & 0x7)) + (g_NESmapper.BmcSV16in1.epromFirst ? 0x2 : 0x0) : g_NESmapper.BmcSV16in1.epromFirst ? 0x01 : 0x81);
}

void BmcSV16in1_Reset()
{
	g_NESmapper.BmcSV16in1.regs[0] = 0;
	g_NESmapper.BmcSV16in1.regs[1] = 0;
	if( NES_crc32() == 0x7e449555 )
	g_NESmapper.BmcSV16in1.epromFirst =1;
	else
	g_NESmapper.BmcSV16in1.epromFirst =0;

	BmcSV16in1_UpdatePrg(); 
	//g_NESmapper.BmcSV16in1.wram = g_NES.mapper_extram;
	//g_NES.mapper_extramsize = 0x2000;		
	//g_NESmapper.SetPROM_Bank( 2, g_NESmapper.BmcSV16in1.wram);	
}

void BmcSV16in1_Init()
{	
	g_NESmapper.Reset = BmcSV16in1_Reset;
	g_NESmapper.MemoryWriteSaveRAM = BmcSV16in1_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = BmcSV16in1_MemoryWrite;
}