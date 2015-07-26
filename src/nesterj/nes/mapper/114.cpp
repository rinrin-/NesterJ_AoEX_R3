#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 114
void NES_mapper114_Init()
{
	g_NESmapper.Reset = NES_mapper114_Reset;
	g_NESmapper.MemoryWrite = NES_mapper114_MemoryWrite;
}

void NES_mapper114_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.Mapper114.regs[0]=0;
	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}


void NES_mapper114_MemoryWrite(uint32 addr, uint8 data)
{
	//	char f=0;
	switch(addr&0xf000){
	case 0xA000:
		g_NESmapper.set_CPU_bank4(data);
		break;
	case 0xC000:
		g_NESmapper.set_CPU_bank5(data);
		break;
		/*		case 0x8000:
					set_CPU_bank4(data&0x0f);
					break;
				case 0x8000:
					set_CPU_bank4(data&0x0f);
					break;*/
		//		case 0xC004:
		//			set_PPU_bank1(data);
		//			break;
	}
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << "  " << HEX(g_NESmapper.Mapper114.regs[0],2) << endl);

}

#if 0
void NES_mapper114_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper114.irq_enabled)
	{
		if(scanline<241){
			g_NESmapper.Mapper114.irq_counter--;
			if(g_NESmapper.Mapper114.irq_counter<=0){
				NES6502_DoIRQ();
				g_NESmapper.Mapper114.irq_enabled = 0;
				g_NESmapper.Mapper114.irq_counter = g_NESmapper.Mapper114.irq_latch;
			}
		}
	}
}
#endif

/////////////////////////////////////////////////////////////////////

#endif
