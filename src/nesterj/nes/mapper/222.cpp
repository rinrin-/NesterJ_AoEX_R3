#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 222
void NES_mapper222_Init()
{
	g_NESmapper.Reset = NES_mapper222_Reset;
	g_NESmapper.MemoryWrite = NES_mapper222_MemoryWrite;
}

void NES_mapper222_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1);
	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
//	set_mirroring(NES_PPU_MIRROR_HORIZ);
	// Dragon Ninja Pirates
	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}



void NES_mapper222_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr&0xf003){
	case 0x8000:
		g_NESmapper.set_CPU_bank4(data);
		break;
	case 0xA000:
		g_NESmapper.set_CPU_bank5(data);
		break;

	case 0xB000:
		g_NESmapper.set_PPU_bank0(data);
		break;
	case 0xB002:
		g_NESmapper.set_PPU_bank1(data);
		break;
	case 0xC000:
		g_NESmapper.set_PPU_bank2(data);
		break;
	case 0xC002:
		g_NESmapper.set_PPU_bank3(data);
		break;
	case 0xD000:
		g_NESmapper.set_PPU_bank4(data);
		break;
	case 0xD002:
		g_NESmapper.set_PPU_bank5(data);
		break;
	case 0xE000:
		g_NESmapper.set_PPU_bank6(data);
		break;
	case 0xE002:
		g_NESmapper.set_PPU_bank7(data);
		break;
	}
}


/////////////////////////////////////////////////////////////////////

#endif
