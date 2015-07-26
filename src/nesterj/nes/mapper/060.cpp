#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 60
void NES_mapper60_Init()
{
	g_NESmapper.Reset = NES_mapper60_Reset;
	g_NESmapper.MemoryWrite = NES_mapper60_MemoryWrite;
	g_NESmapper.Mapper60.game_sel = 0;
}

void NES_mapper60_Reset()
{
	g_NESmapper.Mapper60.patch = 0;

	if( NES_crc32() == 0xf9c484a0 ) {	// Reset Based 4-in-1(Unl)
		g_NESmapper.SetPROM_16K_Bank( 4, g_NESmapper.Mapper60.game_sel );
		g_NESmapper.SetPROM_16K_Bank( 6, g_NESmapper.Mapper60.game_sel );
		g_NESmapper.SetVROM_8K_Bank( g_NESmapper.Mapper60.game_sel );
		g_NESmapper.Mapper60.game_sel++;
		g_NESmapper.Mapper60.game_sel &= 3;
	} else {
		g_NESmapper.Mapper60.patch = 1;
		g_NESmapper.SetPROM_32K_Bank( 0 );
		g_NESmapper.SetVROM_8K_Bank( 0 );
	}
}

void NES_mapper60_MemoryWrite(uint32 addr, uint8 data)
{
	if( g_NESmapper.Mapper60.patch ) {
		if( addr & 0x80 ) {
			g_NESmapper.SetPROM_16K_Bank( 4, (addr&0x70)>>4 );
			g_NESmapper.SetPROM_16K_Bank( 6, (addr&0x70)>>4 );
		} else {
			g_NESmapper.SetPROM_32K_Bank( (addr&0x70)>>5 );
		}

		g_NESmapper.SetVROM_8K_Bank( addr&0x07 );

		if(data & 0x08)
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		}
		else
		{
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
	}
	
}
/////////////////////////////////////////////////////////////////////

#endif
