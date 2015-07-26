
/////////////////////////////////////////////////////////////////////
// Mapper 163

void NES_mapper162_Init()
{
	g_NESmapper.Reset = NES_mapper162_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper162_MemoryWriteLow;
	g_NESmapper.HSync = NES_mapper162_HSync;
}

void NES_mapper162_Reset()
{
	g_NESmapper.Mapper162.reg5000 = 0;
	g_NESmapper.Mapper162.reg5100 = 0;
	g_NESmapper.Mapper162.reg5200 = 0;
	g_NESmapper.Mapper162.reg5300 = 7;
	NES_mapper162_MMC3_set_CPU_banks();
	NES_mapper162_MMC3_set_PPU_banks();
		
	g_PPU.vram_write_protect = 0;
}

void NES_mapper162_MemoryWriteLow(uint32 addr, uint8 data)
{
	if(addr==0x5000){
		g_NESmapper.Mapper162.reg5000 = data;
		NES_mapper162_MMC3_set_CPU_banks();
		NES_mapper162_MMC3_set_PPU_banks();
	}else if(addr==0x5100){
		g_NESmapper.Mapper162.reg5100 = data;
		NES_mapper162_MMC3_set_CPU_banks();
		NES_mapper162_MMC3_set_PPU_banks();
	}else if(addr==0x5200){
		g_NESmapper.Mapper162.reg5200 = data;
		NES_mapper162_MMC3_set_CPU_banks();
		NES_mapper162_MMC3_set_PPU_banks();
	}else if(addr==0x5300){
		g_NESmapper.Mapper162.reg5300 = data;
	}else if(addr>=0x6000){
		;//CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}

void NES_mapper162_MMC3_set_CPU_banks()
{
	uint8 bank=0;
	switch (g_NESmapper.Mapper162.reg5300)
	{
		case 4:
			bank = (( (g_NESmapper.Mapper162.reg5000 & 0xF) + ((g_NESmapper.Mapper162.reg5100 & 3) >> 1) ) | ((g_NESmapper.Mapper162.reg5200 & 1) << 4));
			break;
		case 7:
			bank = ((g_NESmapper.Mapper162.reg5000 & 0xF) | ((g_NESmapper.Mapper162.reg5200 & 1) << 4));
			break;
	}
	//SetPROM_32K_Bank(bank);
	g_NESmapper.set_CPU_banks4(bank*4,bank*4+1,bank*4+2,bank*4+3);
}
void NES_mapper162_MMC3_set_PPU_banks()
{
	//SetCRAM_8K_Bank(0);			
	g_NESmapper.set_VRAM_bank(0,0);
	g_NESmapper.set_VRAM_bank(1,1);
	g_NESmapper.set_VRAM_bank(2,2);
	g_NESmapper.set_VRAM_bank(3,3);
	g_NESmapper.set_VRAM_bank(4,4);
	g_NESmapper.set_VRAM_bank(5,5);
	g_NESmapper.set_VRAM_bank(6,6);
	g_NESmapper.set_VRAM_bank(7,7);
}


void NES_mapper162_HSync(uint32 scanline)
{
	if( (g_NESmapper.Mapper162.reg5000&0x80) && (NES_PPU_spr_enabled() || NES_PPU_bg_enabled())) {
		if(scanline<127){
			//SetCRAM_4K_Bank(4, 0);
			g_NESmapper.set_VRAM_bank(4,0);
			g_NESmapper.set_VRAM_bank(5,1);
			g_NESmapper.set_VRAM_bank(6,2);
			g_NESmapper.set_VRAM_bank(7,3);
		}else if(scanline<240){
			//SetCRAM_4K_Bank(4, 1);
			g_NESmapper.set_VRAM_bank(4,4);
			g_NESmapper.set_VRAM_bank(5,5);
			g_NESmapper.set_VRAM_bank(6,6);
			g_NESmapper.set_VRAM_bank(7,7);
		}
	}
}
