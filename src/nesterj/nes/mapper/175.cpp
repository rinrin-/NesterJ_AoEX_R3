
/////////////////////////////////////////////////////////////////////
// Mapper 175

void NES_mapper175_Init()
{
	g_NESmapper.Reset = NES_mapper175_Reset;
	g_NESmapper.MemoryWrite = NES_mapper175_MemoryWrite;
	g_NESmapper.MemoryReadLow = NES_mapper175_MemoryReadLow;
}

void NES_mapper175_Reset()
{
	g_NESmapper.SetPROM_16K_Bank( 4, 0 );
	g_NESmapper.SetPROM_16K_Bank( 6, 0 );
	g_NESmapper.Mapper175.reg_dat = 0;

	if(g_NESmapper.num_1k_VROM_banks) {
		g_NESmapper.SetVROM_8K_Bank( 0 );
	}
}


uint8 NES_mapper175_MemoryReadLow(uint32 addr)
{
	if( addr == 0xFFFC ) {
		g_NESmapper.SetPROM_16K_Bank( 4, g_NESmapper.Mapper175.reg_dat & 0x0F );
		g_NESmapper.SetPROM_8K_Bank( 6, (g_NESmapper.Mapper175.reg_dat & 0x0F)*2 );
	}
	return 0;
}

void NES_mapper175_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr ) {
		case	0x8000:
			if( data & 0x04 ) {
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			} else {
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			break;
		case	0xA000:
			g_NESmapper.Mapper175.reg_dat = data;
			g_NESmapper.SetPROM_8K_Bank( 7, (g_NESmapper.Mapper175.reg_dat & 0x0F)*2+1 );
			g_NESmapper.SetVROM_8K_Bank( g_NESmapper.Mapper175.reg_dat & 0x0F );
			break;
	}
}
/////////////////////////////////////////////////////////////////////

