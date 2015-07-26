
/////////////////////////////////////////////////////////////////////
// Mapper 167


void NES_mapper167_Init()
{
	g_NESmapper.Reset = NES_mapper167_Reset;
	g_NESmapper.MemoryWrite = NES_mapper167_MemoryWrite;
}

void NES_mapper167_Reset()
{
	g_NESmapper.Mapper167.regs[0] = 0;
	g_NESmapper.Mapper167.regs[1] = 0;
	g_NESmapper.Mapper167.regs[2] = 0;
	g_NESmapper.Mapper167.regs[3] = 0;

	if(NES_crc32()==0x82F1Fb96){
		// Subor Computer(Russia)
		g_NESmapper.Mapper167.rom_type = 1;
	}else{
		// Subor Computer(Chinese)
		g_NESmapper.Mapper167.rom_type = 0;
	}

	NES_mapper167_MMC3_set_CPU_banks();
	NES_mapper167_MMC3_set_PPU_banks();
}

void NES_mapper167_MemoryWrite(uint32 addr, uint8 data)
{
	int idx;

	idx = (addr>>13)&0x03;
	g_NESmapper.Mapper167.regs[idx]=data;
	NES_mapper167_MMC3_set_CPU_banks();
	NES_mapper167_MMC3_set_PPU_banks();	
}



void NES_mapper167_MMC3_set_CPU_banks()
{
	int base, bank;

	base = ((g_NESmapper.Mapper167.regs[0]^g_NESmapper.Mapper167.regs[1])&0x10)<<1;
	bank = (g_NESmapper.Mapper167.regs[2]^g_NESmapper.Mapper167.regs[3])&0x1f;
	
	if(g_NESmapper.Mapper167.regs[1]&0x08){
		bank &= 0xfe;
		if(g_NESmapper.Mapper167.rom_type==0){
			g_NESmapper.SetPROM_16K_Bank(4, base+bank+1);
			g_NESmapper.SetPROM_16K_Bank(6, base+bank+0);
		}else{
			g_NESmapper.SetPROM_16K_Bank(6, base+bank+1);
			g_NESmapper.SetPROM_16K_Bank(4, base+bank+0);
		}
	}else{
		if(g_NESmapper.Mapper167.regs[1]&0x04){
			g_NESmapper.SetPROM_16K_Bank(4, 0x1f);
			g_NESmapper.SetPROM_16K_Bank(6, base+bank);
		}else{
			g_NESmapper.SetPROM_16K_Bank(4, base+bank);
			if(g_NESmapper.Mapper167.rom_type==0){
				g_NESmapper.SetPROM_16K_Bank(6, 0x20);
			}else{
				g_NESmapper.SetPROM_16K_Bank(6, 0x07);
			}
		}
	}
}                            

void NES_mapper167_MMC3_set_PPU_banks()
{
	g_NESmapper.SetCRAM_8K_Bank(0);
}
/////////////////////////////////////////////////////////////////////

