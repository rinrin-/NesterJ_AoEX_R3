
/////////////////////////////////////////////////////////////////////
// Mapper 204


void NES_mapper204_Init()
{
	g_NESmapper.Reset = NES_mapper204_Reset;
	g_NESmapper.MemoryWrite = NES_mapper204_MemoryWrite;
}



void NES_mapper204_Reset()
{	
	//ROM_BANK32(~0);
	g_NESmapper.SetPROM_32K_Bank(0);
	//VROM_BANK8(~0);
	g_NESmapper.SetVROM_8K_Bank(0);
}

void NES_mapper204_MemoryWrite(uint32 addr, uint8 data)
{	
	//int tmp2=A&0x6;
	//int tmp1=tmp2+((tmp2==0x6)?0:(A&1));
	//MIRROR_SET((A>>4)&1);
	//ROM_BANK16(0x8000,tmp1);
	//ROM_BANK16(0xc000,tmp2+((tmp2==0x6)?1:(A&1)));
	//VROM_BANK8(tmp1);

	int tmp2=addr&0x6;
	int tmp1=tmp2+((tmp2==0x6)?0:(addr&1));
	//MIRROR_SET((addr>>4)&1);
	//ROM_BANK16(0x8000,tmp1);
	//ROM_BANK16(0xc000,tmp2+((tmp2==0x6)?1:(addr&1)));
	//VROM_BANK8(tmp1);
	g_NESmapper.SetPROM_16K_Bank(4,tmp1);
	g_NESmapper.SetPROM_16K_Bank(6,tmp2+((tmp2==0x6)?1:(addr&1)));
	g_NESmapper.SetVROM_8K_Bank(tmp1);
}
