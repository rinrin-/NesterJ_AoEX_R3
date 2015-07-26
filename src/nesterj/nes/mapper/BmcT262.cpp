
/////////////////////////////////////////////////////////////////////
// void BmcT262_Reset();

void BmcT262_Init()
{
	g_NESmapper.Reset = BmcT262_Reset;
	g_NESmapper.MemoryWrite = BmcT262_MemoryWrite;
}

void BmcT262_Reset()
{
	g_NESmapper.SetVROM_8K_Bank(0);
	g_NESmapper.BmcT262.busy=0;
	g_NESmapper.BmcT262.addrreg=0;
	g_NESmapper.BmcT262.datareg=0;
	BmcT262_Sync();  
}

void BmcT262_MemoryWrite(uint32 A, uint8 V)
{
  if(g_NESmapper.BmcT262.busy||(A==0x8000))
    g_NESmapper.BmcT262.datareg=V;
  else
  {
    g_NESmapper.BmcT262.addrreg=A;
    g_NESmapper.BmcT262.busy=1;
  }
  BmcT262_Sync();
}

void BmcT262_Sync()
{
  uint16 base=((g_NESmapper.BmcT262.addrreg&0x60)>>2)|((g_NESmapper.BmcT262.addrreg&0x100)>>3);  
  g_NESmapper.SetPROM_16K_Bank(4,(g_NESmapper.BmcT262.datareg&7)|base);
  g_NESmapper.SetPROM_16K_Bank(6,7|base);
  g_NESmapper.set_mirroring2(((g_NESmapper.BmcT262.addrreg&2)>>1)^1);
}
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// void Bmc8157_Reset();

void Bmc8157_Init()
{
	g_NESmapper.Reset = Bmc8157_Reset;
	g_NESmapper.MemoryWrite = Bmc8157_MemoryWrite;
	g_NESmapper.MemoryReadSaveRAM = Bmc8157_MemoryReadSaveRAM;
	g_NESmapper.Bmc8157.mode = 0x00;
}

void Bmc8157_Reset()
{

	g_NESmapper.Bmc8157.trash = 0x00;
	if(g_NESmapper.Bmc8157.mode)
		g_NESmapper.Bmc8157.mode = 0;
	else
		g_NESmapper.Bmc8157.mode = 0x100;
					
	Bmc8157_MemoryWrite(0x8000,0);
}

void Bmc8157_MemoryWrite(uint32 A, uint8 V)
{
	g_NESmapper.Bmc8157.trash = 
	(A & g_NESmapper.Bmc8157.mode ) ? 0xFF : 0x00;
	
	g_NESmapper.SetPROM_16K_Bank(4,(A >> 2 & 0x18) | (A >> 2 & 0x7));
	g_NESmapper.SetPROM_16K_Bank(6,(A >> 2 & 0x18) | ((A & 0x200) ? 0x7 : 0x0));
	
	if(A & 0x2)
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_HORIZ );
	else
		g_NESmapper.set_mirroring2( NES_PPU_MIRROR_VERT);
}

void Bmc8157_MemoryReadSaveRAM(uint32 A)
{
	if(A>=0x8000)
	{
		//return 0xFF;
		nes6502_context cpu;
		NES6502_GetContext(&cpu);
		cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (A-0x8000) | g_NESmapper.Bmc8157.trash;
		NES6502_SetContext(&cpu);
	}			
}
/////////////////////////////////////////////////////////////////////

