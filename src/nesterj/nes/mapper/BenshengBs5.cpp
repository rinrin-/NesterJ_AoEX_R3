/////////////////////////////////////////////////////////////////////
// BenshengBs5
void BenshengBs5_Init()
{
	g_NESmapper.Reset = BenshengBs5_Reset;
	g_NESmapper.MemoryWrite = BenshengBs5_MemoryWrite;
	g_NESmapper.BenshengBs5.mode = 3;
}

void BenshengBs5_Reset()
{
	g_NESmapper.SetPROM_8K_Bank(4,~0);
	g_NESmapper.SetPROM_8K_Bank(5,~0);
	g_NESmapper.SetPROM_8K_Bank(6,~0);
	g_NESmapper.SetPROM_8K_Bank(7,~0);
	g_NESmapper.BenshengBs5.mode = (g_NESmapper.BenshengBs5.mode+1)&3;
}

void BenshengBs5_MemoryWrite(uint32 A, uint8 V)
{
	if( A<=0x8fff )//8000~8fff
	{
		g_NESmapper.SetVROM_2K_Bank( (A << 1 & 0x1800)>>10, A & 0x1F );
	}else if( (A>=0xA000)&&(A<=0xafff) )//A000~Afff
	{
		if (A & (0x10U << (1 ? g_NESmapper.BenshengBs5.mode : 0)))
			g_NESmapper.SetPROM_8K_Bank(((A << 3 & 0x6000)>>13)+4,A & 0xF);
	}
}
/////////////////////////////////////////////////////////////////////
