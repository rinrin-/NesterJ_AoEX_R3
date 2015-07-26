#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 244
void NES_mapper244_Init()
{
	g_NESmapper.Reset = NES_mapper244_Reset;
	g_NESmapper.MemoryWrite = NES_mapper244_MemoryWrite;
}

void NES_mapper244_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0, 1, 2, 3);
	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}


void NES_mapper244_MemoryWrite(uint32 addr, uint8 data)
{
/*
	switch(addr&0xE001){
		case 0x8000:
			data<<=2;
			set_CPU_banks(data, data+1, data+2, data+3);
			break;
		case 0x8001:
//			data-=8;
			data<<=3;
			set_PPU_banks(data,data+1,data+2,data+3,data+4,data+5,data+6,data+7);
			break;
	}
*/
//	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) <<  endl);

	if(data<4){
		data<<=2;
		g_NESmapper.set_CPU_banks4(data, data+1, data+2, data+3);
	}
	else if(data>=8 && data<=0x0f){
		data-=8;
		data<<=3;
		g_NESmapper.set_PPU_banks8(data,data+1,data+2,data+3,data+4,data+5,data+6,data+7);
	}

}

/////////////////////////////////////////////////////////////////////

#endif
