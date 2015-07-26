
/////////////////////////////////////////////////////////////////////
// Mapper 217


void NES_mapper217_Init()
{
	g_NESmapper.Reset = NES_mapper217_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper217_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper217_MemoryWrite;
	g_NESmapper.HSync = NES_mapper217_HSync;
}

void NES_mapper217_Reset()
{
	int x;
	for(x=0;x<8;x++)	
		g_NESmapper.Mapper217.regs[x]=0;
		
	g_NESmapper.Mapper217.master   =0;
	g_NESmapper.Mapper217.chrm     =0;
	g_NESmapper.Mapper217.cmd      =0;
	g_NESmapper.Mapper217.cmdin    =0;
	g_NESmapper.Mapper217.IRQCount =0;
	//GameHBIRQHook=hooko217;
	//SetReadHandler(0x8000,0xFFFF,CartBR);
	//SetWriteHandler(0x5000,0xFFFF,Write217_write);
	NES_mapper217_MMC3_set_CPU_banks();
	NES_mapper217_MMC3_set_PPU_banks();
}
void NES_mapper217_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch(addr&0xE001)
	{
		case 0x4000:
				g_NESmapper.Mapper217.master=data;NES_mapper217_MMC3_set_CPU_banks();break;
		case 0x8000:
				g_NESmapper.Mapper217.IRQCount=data;break;
		case 0xc001:break;
		case 0xe000:
				nes6502_ClrIRQ( IRQ_MAPPER );break;
		case 0xe001:break;
		case 0xa001://setmirror((data&1)^1);
					break;
		case 0x8001:
				g_NESmapper.Mapper217.cmd=data;
				g_NESmapper.Mapper217.cmdin=1;
				NES_mapper217_MMC3_set_CPU_banks();NES_mapper217_MMC3_set_PPU_banks();break;
		case 0xa000:if(!g_NESmapper.Mapper217.cmdin) break;
				g_NESmapper.Mapper217.regs[g_NESmapper.Mapper217.cmd&7]=data;
				NES_mapper217_MMC3_set_CPU_banks();
				NES_mapper217_MMC3_set_PPU_banks();
				g_NESmapper.Mapper217.cmdin=0;
				break;
	}
 
}

void NES_mapper217_MemoryWrite(uint32 addr, uint8 data)
{
	switch(addr&0xE001)
	{
		case 0x4000:
				g_NESmapper.Mapper217.master=data;NES_mapper217_MMC3_set_CPU_banks();break;
		case 0x8000:
				g_NESmapper.Mapper217.IRQCount=data;break;
		case 0xc001:break;
		case 0xe000:
				nes6502_ClrIRQ( IRQ_MAPPER );break;
		case 0xe001:break;
		case 0xa001://setmirror((data&1)^1);
					g_NESmapper.set_mirroring2((data&1)^1);
					break;
		case 0x8001:
				g_NESmapper.Mapper217.cmd=data;
				g_NESmapper.Mapper217.cmdin=1;
				NES_mapper217_MMC3_set_CPU_banks();NES_mapper217_MMC3_set_PPU_banks();break;
		case 0xa000:if(!g_NESmapper.Mapper217.cmdin) break;
				g_NESmapper.Mapper217.regs[g_NESmapper.Mapper217.cmd&7]=data;
				NES_mapper217_MMC3_set_CPU_banks();
				NES_mapper217_MMC3_set_PPU_banks();
				g_NESmapper.Mapper217.cmdin=0;
				break;
	}
 
}

void NES_mapper217_MMC3_set_PPU_banks(void)
{
	unsigned long base=(g_NESmapper.Mapper217.cmd&0x80)<<5;
	int orie=(g_NESmapper.Mapper217.chrm&0x4)<<6;

	unsigned long temp = (orie|g_NESmapper.Mapper217.regs[0])>>1;

	g_NESmapper.SetVROM_2K_Bank((0x0000^base)/0x400,temp);

	temp = (orie|g_NESmapper.Mapper217.regs[7])>>1;
	g_NESmapper.SetVROM_2K_Bank((0x0800^base)/0x400,temp);
	
	g_NESmapper.set_PPU_bank(4,(orie|g_NESmapper.Mapper217.regs[5])>>1 );
	g_NESmapper.set_PPU_bank(5,(orie|g_NESmapper.Mapper217.regs[2])>>1 );
	g_NESmapper.set_PPU_bank(6,(orie|g_NESmapper.Mapper217.regs[6])>>1 );
	g_NESmapper.set_PPU_bank(7,(orie|g_NESmapper.Mapper217.regs[4])>>1 );
}

void NES_mapper217_MMC3_set_CPU_banks(void)
{
	if(g_NESmapper.Mapper217.master&0x80)
	{
		g_NESmapper.SetPROM_16K_Bank(4,g_NESmapper.Mapper217.master&0x1F);
		g_NESmapper.SetPROM_16K_Bank(6,g_NESmapper.Mapper217.master&0x1F);
	}
	else
	{
		g_NESmapper.SetPROM_8K_Bank(4,g_NESmapper.Mapper217.regs[1]);
		g_NESmapper.SetPROM_8K_Bank(5,g_NESmapper.Mapper217.regs[3]);
		g_NESmapper.SetPROM_8K_Bank(6,~1);
		g_NESmapper.SetPROM_8K_Bank(7,~0);
	}
}

void NES_mapper217_HSync(uint32 scanline)
{
	if(g_NESmapper.Mapper217.IRQCount)
	{
		g_NESmapper.Mapper217.IRQCount--;
		if(!g_NESmapper.Mapper217.IRQCount)
		{
			//X6502_IRQBegin(FCEU_IQEXT);
			nes6502_SetIRQ( IRQ_MAPPER );
		}
	}
}