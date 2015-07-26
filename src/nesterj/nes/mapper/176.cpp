
/////////////////////////////////////////////////////////////////////
// Mapper 176

void NES_mapper176_Init()
{
	g_NESmapper.Reset = NES_mapper176_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper176_MemoryWriteLow;
}

void NES_mapper176_Reset()
{
	g_NESmapper.set_CPU_banks4( 0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1 );
	if( g_NESmapper.num_1k_VROM_banks ) {
		//SetVROM_8K_Bank( 0 );		
		g_NESmapper.set_PPU_bank(0, 0 );
		g_NESmapper.set_PPU_bank(1, 1 );
		g_NESmapper.set_PPU_bank(2, 2 );
		g_NESmapper.set_PPU_bank(3, 3 );
		g_NESmapper.set_PPU_bank(4, 4 );
		g_NESmapper.set_PPU_bank(5, 5 );
		g_NESmapper.set_PPU_bank(6, 6 );
		g_NESmapper.set_PPU_bank(7, 7 );
	}		                      
}

void NES_mapper176_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch( addr ) {
		case 0x5ff1:
			 g_NESmapper.set_CPU_banks4( (data>>1)*4,(data>>1)*4+1,(data>>1)*4+2,(data>>1)*4+3 );
			 break;
		case 0x5ff2:
			 //SetVROM_8K_Bank( data );				 
			g_NESmapper.set_PPU_bank(0, data*8+0 );
			g_NESmapper.set_PPU_bank(1, data*8+1 );
			g_NESmapper.set_PPU_bank(2, data*8+2 );
			g_NESmapper.set_PPU_bank(3, data*8+3 );
			g_NESmapper.set_PPU_bank(4, data*8+4 );
			g_NESmapper.set_PPU_bank(5, data*8+5 );
			g_NESmapper.set_PPU_bank(6, data*8+6 );
			g_NESmapper.set_PPU_bank(7, data*8+7 );
			 break;
		default:
			 break;
			 }
	if( addr>=0x6000 ) {
		;//CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}	
}

