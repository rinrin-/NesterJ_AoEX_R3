
/////////////////////////////////////////////////////////////////////
// Mapper 91
void NES_mapper91_Init()
{
	g_NESmapper.Reset = NES_mapper91_Reset;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper91_MemoryWriteSaveRAM;
	g_NESmapper.HSync = NES_mapper91_HSync;
}

void NES_mapper91_Reset()
{
	g_NESmapper.set_CPU_banks4(g_NESmapper.num_8k_ROM_banks-2,
	                           g_NESmapper.num_8k_ROM_banks-1,
	                           g_NESmapper.num_8k_ROM_banks-2,
	                           g_NESmapper.num_8k_ROM_banks-1);
                            
	// set PPU bank pointers
	//if(NES_ROM_get_num_8k_VROM_banks())
	if( g_NESmapper.num_1k_VROM_banks )
	{
		g_NESmapper.set_PPU_banks8(0,0,0,0,0,0,0,0);
	}	
	
	g_NESmapper.Mapper91.irq_counter = 0;
	g_NESmapper.Mapper91.irq_enabled = 0;
}

void NES_mapper91_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	LOG("W " << HEX(addr,4) << "  " << HEX(data,2) << endl);
	switch(addr & 0xF003)
	{
	case 0x6000:
		{
			g_NESmapper.set_PPU_bank0(data*2+0);
			g_NESmapper.set_PPU_bank1(data*2+1);
		}
		break;

	case 0x6001:
		{
			g_NESmapper.set_PPU_bank2(data*2+0);
			g_NESmapper.set_PPU_bank3(data*2+1);
		}
		break;

	case 0x6002:
		{
			g_NESmapper.set_PPU_bank4(data*2+0);
			g_NESmapper.set_PPU_bank5(data*2+1);
		}
		break;

	case 0x6003:
		{
			g_NESmapper.set_PPU_bank6(data*2+0);
			g_NESmapper.set_PPU_bank7(data*2+1);
		}
		break;

	case 0x7000:
		{
			g_NESmapper.set_CPU_bank4(data);
		}
		break;

	case 0x7001:
		{
			g_NESmapper.set_CPU_bank5(data);
		}
		break;

	case 0x7002:
			{
			g_NESmapper.Mapper91.irq_counter = 0;
			g_NESmapper.Mapper91.irq_enabled = 0;
		}
			nes6502_context context;
			NES6502_GetContext(&context);
			context.int_pending &= ~0x10;
			NES6502_SetContext(&context);	
		break;

	case 0x7003:
		g_NESmapper.Mapper91.irq_enabled = 1;
		break;
	}
}

void NES_mapper91_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline < 240) && (NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) ) 
	{
		if( g_NESmapper.Mapper91.irq_enabled ) {
			g_NESmapper.Mapper91.irq_counter++;
		}
		if( g_NESmapper.Mapper91.irq_counter >= 8 ) {
					NES6502_DoIRQ();
		}
	}
}
/////////////////////////////////////////////////////////////////////

