#ifdef _NES_MAPPER_CPP_

/////////////////////////////////////////////////////////////////////
// Mapper 254
void NES_mapper254_Init()
{
	g_NESmapper.Reset = NES_mapper254_Reset;
	g_NESmapper.MemoryReadSaveRAM = NES_mapper254_MemoryReadSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper254_MemoryWrite;
	g_NESmapper.HSync = NES_mapper254_HSync;
}

void NES_mapper254_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_bank3(0);
	g_NESmapper.set_CPU_bank4(g_NESmapper.num_8k_ROM_banks-4);
	g_NESmapper.set_CPU_bank5(g_NESmapper.num_8k_ROM_banks-3);
	g_NESmapper.set_CPU_bank6(g_NESmapper.num_8k_ROM_banks-2);
	g_NESmapper.set_CPU_bank7(g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.Mapper254.regs[0]=0;
	// set PPU bank pointers
//	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
	g_NESmapper.Mapper254.irq_latch=g_NESmapper.Mapper254.irq_counter=g_NESmapper.Mapper254.irq_enabled=0;
}


void NES_mapper254_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	//	if(addr>=0x6000 && addr < 0x8000){
//	NES_WriteSaveRAM(0, 0);
	//    }
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2)  << endl);
}


void NES_mapper254_MemoryReadSaveRAM(uint32 addr)
{
	LOG("R " << HEX(addr,4)  << endl);
}


void NES_mapper254_MemoryWrite(uint32 addr, uint8 data)
{
	//	char f=0;
	switch(addr & 0xE003){
	case 0x8000:
		{
			int pb = (data&0x1f)<<3;
			g_NESmapper.set_PPU_banks8(pb,pb+1,pb+2,pb+3,pb+4,pb+5,pb+6,pb+7);
		}
		break;
	case 0xE000:
//		LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << "  "  << endl);
		g_NESmapper.set_CPU_bank3(data&0x0f);

//		g_NESmapper.Mapper254.irq_counter= (g_NESmapper.Mapper254.irq_counter&0xff00)|data;
		//			set_CPU_bank4(data);
		break;
	case 0xE001:
		{
			if(data & 0x08)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
		}
//		g_NESmapper.Mapper254.irq_counter=(g_NESmapper.Mapper254.irq_counter&0xff)|(data<<8);
		//			set_CPU_bank5(data);
		break;
//		LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << "  " << HEX(g_NESmapper.Mapper254.regs[0],2) << endl);
	}
}

void NES_mapper254_HSync(uint32 scanline)
{
#if 0
	if(g_NESmapper.Mapper254.irq_enabled)
	{
		if(scanline<241 /*&& (NES_PPU_spr_enabled() || NES_PPU_bg_enabled())*/){
			g_NESmapper.Mapper254.irq_counter-=114;
			if(g_NESmapper.Mapper254.irq_counter<=0){
				NES6502_DoIRQ();
				g_NESmapper.Mapper254.irq_enabled = 0;
				//			  g_NESmapper.Mapper254.irq_counter = g_NESmapper.Mapper254.irq_latch;
			}
		}
	}
#endif
}


/////////////////////////////////////////////////////////////////////

#endif
