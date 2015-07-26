void Unlcc21_Init()
{
	g_NESmapper.Reset = Unlcc21_Reset;
	g_NESmapper.MemoryWrite = Unlcc21_MemoryWrite;
}

void Unlcc21_Reset()
{
	g_NESmapper.SetPROM_32K_Bank(0);
}


void Unlcc21_MemoryWrite(uint32 A, uint8 data)
{
	//ppu.SetMirroring( (A & 0x2) ? Ppu::NMT_1 : Ppu::NMT_0 );
	if(A&0x2)
		g_NESmapper.set_mirroring(1,1,1,1);
	else
		g_NESmapper.set_mirroring(0,0,0,0);
	//chr.SwapBank<SIZE_8K,0x0000>( A );
	g_NESmapper.SetVROM_8K_Bank(A);
}
/////////////////////////////////////////////////////////////////////




void BmcD1038_Init()
{
	g_NESmapper.Reset = BmcD1038_Reset;
	g_NESmapper.MemoryWrite = BmcD1038_MemoryWrite;
	g_NESmapper.MemoryReadSaveRAM = BmcD1038_MemoryReadSaveRAM;
	g_NESmapper.BmcD1038.dipswitch = 3;
}
void BmcD1038_Reset()
{
	g_NESmapper.BmcD1038.dipswitch++;
	g_NESmapper.BmcD1038.dipswitch&=3; 
	BmcD1038_MemoryWrite(0x8000,0);
}
void BmcD1038_MemoryWrite(uint32 A,uint8 V)
{
	g_NESmapper.BmcD1038.cartMode = A & 0x100;	
	if(!(V & 0x8))
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			
	g_NESmapper.SetPROM_16K_Bank(4,(A >> 4) & ~(~A >> 7 & 0x1));
	g_NESmapper.SetPROM_16K_Bank(6,(A >> 4) | (~A >> 7 & 0x1) );
	g_NESmapper.SetVROM_8K_Bank(A);
}

void BmcD1038_MemoryReadSaveRAM(uint32 A)
{
	nes6502_context cpu;
	NES6502_GetContext(&cpu);
	if(!g_NESmapper.BmcD1038.cartMode)
	{
		cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)(A>>8);
	}
	else
		//return CartBR(A);
		cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)g_NESmapper.BmcD1038.dipswitch;
	NES6502_SetContext(&cpu);
}