
void BmcCtc65_Init()
{
	g_NESmapper.Reset = BmcCtc65_Reset;
	g_NESmapper.MemoryReadSaveRAM = BmcCtc65_MemoryReadSaveRAM;
	g_NESmapper.MemoryWrite = BmcCtc65_MemoryWrite;
}

void BmcCtc65_Reset()
{
	g_NESmapper.BmcCtc65.reg[0] = 0;
	g_NESmapper.BmcCtc65.reg[1] = 0;
	BmcCtc65_UpdateChrPrg();
}


void BmcCtc65_UpdateChrPrg()
{
	//static const uint8 banks[4] = {0, 0, 1, 2};
	const unsigned int chip = (g_NESmapper.BmcCtc65.reg[1] << 5 & 0x20) << (g_NESmapper.BmcCtc65.reg[0] >> 7);
	/*
  if(g_NESmapper.BmcCtc65.reg[0]&0x20)
  {
    //setprg16r(banks[bank],0x8000,g_NESmapper.BmcCtc65.reg[0]&0x1F);
    //setprg16r(banks[bank],0xC000,g_NESmapper.BmcCtc65.reg[0]&0x1F);
    g_NESmapper.SetPROM_16K_Bank(4,banks[g_NESmapper.BmcCtc65.bank]|g_NESmapper.BmcCtc65.reg[0]&0x1F);
    g_NESmapper.SetPROM_16K_Bank(6,banks[g_NESmapper.BmcCtc65.bank]|g_NESmapper.BmcCtc65.reg[0]&0x1F);
  }
  else
    //setprg32r(banks[bank],0x8000,(g_NESmapper.BmcCtc65.reg[0]>>1)&0x0F);
    g_NESmapper.SetPROM_16K_Bank(4,banks[g_NESmapper.BmcCtc65.bank]|(g_NESmapper.BmcCtc65.reg[0]>>1)&0x0F);
	*/
	g_NESmapper.SetPROM_16K_Bank(4,chip | (g_NESmapper.BmcCtc65.reg[0] & 0x1E) | (g_NESmapper.BmcCtc65.reg[0] >> 5 & g_NESmapper.BmcCtc65.reg[0]));
	g_NESmapper.SetPROM_16K_Bank(6,chip | (g_NESmapper.BmcCtc65.reg[0] & 0x1F) | (~g_NESmapper.BmcCtc65.reg[0] >> 5 & 0x01));


					
  if(g_NESmapper.BmcCtc65.reg[1]&2)
    //setchr8r(0x10,0);
		g_NESmapper.SetCRAM_8K_Bank(0);
  else
	g_NESmapper.SetVROM_8K_Bank(0);
	if(g_NESmapper.BmcCtc65.reg[0] & 0x40)
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_VERT );
	else
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_HORIZ);
  }


void BmcCtc65_MemoryWrite(uint32 A, uint8 V)
{
  g_NESmapper.BmcCtc65.reg[A&1]=V;
  g_NESmapper.BmcCtc65.bank=((g_NESmapper.BmcCtc65.reg[0]&0x80)>>7)|((g_NESmapper.BmcCtc65.reg[1]&1)<<1);
  BmcCtc65_UpdateChrPrg();  
}
void BmcCtc65_MemoryReadSaveRAM(uint32 addr)
{
	if(addr>=0x8000)
	{
		//return !openBus ? prg.Peek( address - 0x8000 ) : (address >> 8);
		nes6502_context cpu;
		NES6502_GetContext(&cpu);
		if(g_NESmapper.BmcCtc65.bank==1)
		cpu.mem_page[addr>>13][addr&NES6502_BANKMASK] = (uint8)(addr >> 8);
		NES6502_SetContext(&cpu);
	}
}




