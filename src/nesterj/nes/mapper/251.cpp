#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 251
void NES_mapper251_Init()
{
	g_NESmapper.Reset = NES_mapper251_Reset;
	g_NESmapper.MemoryWrite = NES_mapper251_MemoryWrite;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper251_MemoryWriteSaveRAM;
}

void NES_mapper251_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);
//	set_CPU_banks(0x30, 0x31, 0x3E, 0x3F);

	// set PPU bank pointers
//	set_PPU_banks(0,1,2,3,4,5,6,7);

	g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);

	_memset(g_NESmapper.Mapper251.regs, 0, 10);
	_memset(g_NESmapper.Mapper251.bregs, 0, 4);
	//g_NESmapper.Mapper251.regs[0] = g_NESmapper.Mapper251.regs[2] = 0x0F;
	//g_NESmapper.Mapper251.regs[1] = g_NESmapper.Mapper251.regs[3] = 0x00;
}

void NES_mapper251_banksync()
{
	uint32 chr[6], prg[4];
	int i;

	for(i=0; i<6; i++){
		chr[i] = (g_NESmapper.Mapper251.regs[i] | (g_NESmapper.Mapper251.bregs[1] << 4)) & ((g_NESmapper.Mapper251.bregs[2] << 4) | 0x0F);
	}

	if(g_NESmapper.Mapper251.regs[8]&0x80){
		g_NESmapper.set_PPU_banks8(chr[2],chr[3],chr[4],chr[5],chr[0],chr[0]+1,chr[1],chr[1]+1);
	}else{
		g_NESmapper.set_PPU_banks8(chr[0],chr[0]+1,chr[1],chr[1]+1,chr[2],chr[3],chr[4],chr[5]);
	}

	prg[0] = (g_NESmapper.Mapper251.regs[6] & ((g_NESmapper.Mapper251.bregs[3] & 0x3F) ^ 0x3F)) | (g_NESmapper.Mapper251.bregs[1]);
	prg[1] = (g_NESmapper.Mapper251.regs[7] & ((g_NESmapper.Mapper251.bregs[3] & 0x3F) ^ 0x3F)) | (g_NESmapper.Mapper251.bregs[1]);
	prg[2] = prg[3] = ((g_NESmapper.Mapper251.bregs[3] & 0x3F) ^ 0x3F) | (g_NESmapper.Mapper251.bregs[1]);
	prg[2] &= (g_NESmapper.num_8k_ROM_banks-1);

	if(g_NESmapper.Mapper251.regs[8]&0x40){
		g_NESmapper.set_CPU_banks4(prg[2],prg[1],prg[0],prg[3]);
	}else{
		g_NESmapper.set_CPU_banks4(prg[0],prg[1],prg[2],prg[3]);
	}
}


void NES_mapper251_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	switch(addr & 0xE001){
		case 0x6000:
			if(g_NESmapper.Mapper251.regs[9]){
				g_NESmapper.Mapper251.bregs[g_NESmapper.Mapper251.regs[10]++] = data;
				if(g_NESmapper.Mapper251.regs[10] == 4){
					g_NESmapper.Mapper251.regs[10] = 0;
					NES_mapper251_banksync();
				}
			}
			break;
	}
}

void NES_mapper251_MemoryWrite(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) <<  endl);
	switch(addr & 0xE001){
#if 1
	case 0x8000:
		g_NESmapper.Mapper251.regs[8]=data;
		NES_mapper251_banksync();
		break;
	case 0x8001:
		g_NESmapper.Mapper251.regs[g_NESmapper.Mapper251.regs[8]&7] = data;
		NES_mapper251_banksync();
		break;
	case 0xA001:
		if(data & 0x80){
			g_NESmapper.Mapper251.regs[9] = 1;
			g_NESmapper.Mapper251.regs[10] = 0;
		}else{
			g_NESmapper.Mapper251.regs[9] = 0;
		}
		break;
#endif
	}

}
/////////////////////////////////////////////////////////////////////

#endif
