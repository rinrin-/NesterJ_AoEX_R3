
/////////////////////////////////////////////////////////////////////
// Mapper 177
void NES_mapper177_Init()
{
	g_NESmapper.Reset = NES_mapper177_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper177_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper177_MemoryWrite;
}

void NES_mapper177_Reset()
{
	//SetPROM_32K_Bank( 0 );
	g_NESmapper.set_CPU_banks4(0,1,2,3);
	g_NESmapper.Mapper177.HengGe_TYPE = 0;
	if(  NES_crc32() == 0x02c41438 ) {	// Xing He Zhan Shi (C)
		g_NESmapper.Mapper177.HengGe_TYPE = 1;
	}		                      
}

void NES_mapper177_MemoryWriteLow(uint32 addr, uint8 data)
{
		if( g_NESmapper.Mapper177.HengGe_TYPE == 1 ) {
			if(addr==0x5ff1){
				//SetPROM_32K_Bank( (data>>1));
				g_NESmapper.set_CPU_banks4((data>>1)*4,(data>>1)*4+1,(data>>1)*4+2,(data>>1)*4+3);
			}
		}
		if( addr>=0x6000 ) {
			;//CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
		}		
}


void NES_mapper177_MemoryWrite(uint32 addr, uint8 data)
{
		if( g_NESmapper.Mapper177.HengGe_TYPE == 0 ) {
			//SetPROM_32K_Bank( data );
			g_NESmapper.set_CPU_banks4(data*4,data*4+1,data*4+2,data*4+3);
			if( data&0x20 ) //SetVRAM_Mirror( VRAM_HMIRROR );			
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			else			//SetVRAM_Mirror( VRAM_VMIRROR );			
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		}
		if( g_NESmapper.Mapper177.HengGe_TYPE == 1) {
			if(addr==0xa000){
				if( data & 0x01 ) //SetVRAM_Mirror( VRAM_HMIRROR );				
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
					else		  //SetVRAM_Mirror( VRAM_VMIRROR );
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			if(addr==0xa001){
				//unknown
			}
		}
}


