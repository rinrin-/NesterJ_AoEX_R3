//Rcm
void Gs2015_Init()
{
	g_NESmapper.Reset = Gs2015_Reset;
	g_NESmapper.MemoryWrite = Gs2015_MemoryWrite;
}
void Gs2015_Reset()
{
	g_NESmapper.SetPROM_32K_Bank(0);
}
void Gs2015_MemoryWrite(uint32 A, uint8 data)
{
	g_NESmapper.SetPROM_32K_Bank(A);
	g_NESmapper.SetVROM_8K_Bank(A>>1);
}



void Gs2013_Reset()
{
	//wrk.SwapBank<SIZE_8K,0x0000>( 0x1F );
	g_NESmapper.SetPROM_8K_Bank(3,0x1F);	
	g_NESmapper.SetPROM_32K_Bank(~0U);
}
void Gs2013_MemoryWrite(uint32 addr, uint8 V)
{	
	g_NESmapper.SetPROM_32K_Bank( (V & 0x8) ? (V & 0x9) : (V & 0x7) );
}
void Gs2013_MemoryReadSaveRAM(uint32 addr)
{
	//if(A<0x8000)
	//return wrk[0][address - 0x6000];
}
void Gs2013_Init()
{
	g_NESmapper.Reset = Gs2013_Reset;
	g_NESmapper.MemoryWrite = Gs2013_MemoryWrite;
	//g_NESmapper.MemoryReadSaveRAM = Gs2013_MemoryReadSaveRAM;
}



void Gs2004_Reset()
{
	g_NESmapper.SetPROM_8K_Bank(3,0x1F);	
	g_NESmapper.SetPROM_32K_Bank(NES_ROM_get_num_16k_ROM_banks() / 2 - 1);
}
void Gs2004_MemoryWrite(uint32 addr, uint8 V)
{
	g_NESmapper.SetPROM_32K_Bank(V);//?PRG_SWAP_32K       
}
void Gs2004_MemoryReadSaveRAM(uint32 A)
{
	//if(A<0x8000)
	//return wrk[0][address - 0x6000];
}
void Gs2004_Init()
{
	g_NESmapper.Reset = Gs2004_Reset;
	g_NESmapper.MemoryWrite = Gs2004_MemoryWrite;
	//g_NESmapper.MemoryReadSaveRAM = Gs2004_MemoryReadSaveRAM;
}



void TetrisFamily_Reset(){
	g_NESmapper.SetPROM_32K_Bank(0);}
void TetrisFamily_MemoryWrite(uint32 A, uint8 V)
{
	if(A & 0x80)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);			

	switch (A & 0x30)
	{
		case 0x00:
		case 0x30:
			g_NESmapper.SetPROM_32K_Bank(A & 0xF);
			break;

		case 0x20:
		case 0x10:

			A = (A << 1 & 0x1E) | (A >> 4 & 0x02);
			g_NESmapper.SetPROM_16K_Bank(4,A);
			g_NESmapper.SetPROM_16K_Bank(6,A);
			break;
	}
}
void TetrisFamily_Init()
{
	g_NESmapper.Reset = TetrisFamily_Reset;
	g_NESmapper.MemoryWrite = TetrisFamily_MemoryWrite;
}