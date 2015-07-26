/////////////////////////////////////////////////////////////////////
// Mapper 61
void NES_mapper61_Init()
{
	Mmc3_Init();
	
	g_NESmapper.Reset = NES_mapper61_Reset;
	g_NESmapper.MemoryWrite = NES_mapper61_MemoryWrite;
}

void NES_mapper61_Reset()
{
	NES_mapper61_MemoryWrite(0x8000,0);
}

void NES_mapper61_MemoryWrite(uint32 A, uint8 V)
{
	 switch(A&0x30)
	 {
	  case 0x00:
	  case 0x30:
			  g_NESmapper.SetPROM_32K_Bank(A&0xF);
			  break;
	  case 0x20:
	  case 0x10:
			  g_NESmapper.SetPROM_16K_Bank(4,((A&0xF)<<1)| (((A&0x20)>>4)) );
			  g_NESmapper.SetPROM_16K_Bank(6,((A&0xF)<<1)| (((A&0x20)>>4)) );
			  break;
	 }	
	 
	 if(A&0x80)
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}
/////////////////////////////////////////////////////////////////////
