
/////////////////////////////////////////////////////////////////////
// Mapper 113
void NES_mapper113_Init()
{
	g_NESmapper.Reset = NES_mapper113_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper113_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper113_MemoryWrite;
}

void NES_mapper113_Reset()
{
	g_NESmapper.SetPROM_32K_Bank( 0 );
	g_NESmapper.SetVROM_8K_Bank( 0 );
}

void NES_mapper113_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr)
	{
	case 0x4100:
	case 0x4111:
	case 0x4120:
	case	0x4194:
	case	0x4195:
	case 0x4900:
			if( NES_crc32() == 0xA75AEDE5 ) 
			{ // HES 6-in-1
				if( data & 0x80 ) {
					g_NESmapper.set_mirroring(0,1,0,1);
					//SetVRAM_Mirror( VRAM_VMIRROR );
				} else {
					g_NESmapper.set_mirroring(0,0,1,1);
					//SetVRAM_Mirror( VRAM_HMIRROR );
				}
			}
			g_NESmapper.SetPROM_32K_Bank( data >> 3 );
			g_NESmapper.SetVROM_8K_Bank( ((data>>3)&0x08)+(data&0x07) );
			break;
	}
}

void NES_mapper113_MemoryWrite(uint32 addr, uint8 data)
{
	switch( addr ) {
		case	0x8008:
		case	0x8009:
			g_NESmapper.SetPROM_32K_Bank( data >> 3 );
			g_NESmapper.SetVROM_8K_Bank( ((data>>3)&0x08)+(data&0x07) );
			break;
		case	0x8E66:
		case	0x8E67:
			g_NESmapper.SetVROM_8K_Bank( (data&0x07)?0:1 );
			break;
		case	0xE00A:
			//SetVRAM_Mirror( VRAM_MIRROR4L );
			g_NESmapper.set_mirroring(0,0,0,0);
			break;
	}
}
/////////////////////////////////////////////////////////////////////

