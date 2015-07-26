/////////////////////////////////////////////////////////////////////
// Mapper 250
void NES_mapper250_Init()
{
	Mmc3_Init();
	
	g_NESmapper.Reset = NES_mapper250_Reset;
	g_NESmapper.MemoryWrite = NES_mapper250_MemoryWrite;
}

void NES_mapper250_Reset()
{
	Mmc3_Reset();	
	//g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}

void NES_mapper250_MemoryWrite(uint32 addr, uint8 data)
{
	data = addr & 0xFF;
	addr = (addr & 0xE000) | (addr >> 10 & 0x1);

	switch (addr)
	{
		case 0x8000: Poke_8000(addr, data ); break;
		case 0x8001: Poke_8001(addr, data ); break;
		case 0xA000: /*SetMirroringHV( data );*/Poke_A000(0,data);break;
		case 0xA001: Poke_A001(addr, data ); break;
		case 0xC000: Poke_C000(addr, data ); break;
		case 0xC001: Poke_C001(addr, data ); break;
		case 0xE000: Poke_E000(addr, data ); break;
		case 0xE001: Poke_E001(addr, data ); break;
	}
}
/////////////////////////////////////////////////////////////////////
