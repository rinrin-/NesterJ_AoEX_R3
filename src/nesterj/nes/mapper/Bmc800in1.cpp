void Game800in1_Init()
{
	g_NESmapper.Reset = Game800in1_Reset;
	g_NESmapper.MemoryWrite = Game800in1_MemoryWrite;//8000~ffff
	g_NESmapper.MemoryReadSaveRAM = Game800in1_MemoryReadSaveRAM;//8000~ffff	
}


void Game800in1_Reset()
{
	g_NESmapper.Game800in1.mode = 0;
	g_NESmapper.Game800in1.prg_bank4 = 0;
	g_NESmapper.Game800in1.prg_bank6 = 0;
	Game800in1_MemoryWrite(0x8000,0);
	Game800in1_MemoryWrite(0xc000,0);
}

void Game800in1_MemoryWrite(uint32 A, uint8 V)
{
	unsigned int banks[2] =
	{	
		g_NESmapper.Game800in1.prg_bank4,//prg.GetBank<SIZE_16K,0x0000>(),
		g_NESmapper.Game800in1.prg_bank6//prg.GetBank<SIZE_16K,0x4000>()
	};

	if (A < 0xC000)
	{	
		if(A & 0x20) 
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else		    
			g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);

		//if (chr.Source().GetType() == Ram::RAM)
		//{
		//	banks[0] = (banks[0] & 0x7) | (A << 3 & 0x38);
		//	banks[1] = (banks[1] & 0x7) | (A << 3 & 0x38);
		//}
		//else
		//{
			g_NESmapper.SetVROM_8K_Bank( A & 0x7 );
			return;
		//}
	}
	else switch (A & 0x30)
	{
		case 0x00: g_NESmapper.Game800in1.mode = 0x0; banks[0] = (banks[0] & 0x38) | (A & 0x7); banks[1] = banks[0] | 0x7; break;
		case 0x10: g_NESmapper.Game800in1.mode = 0x1; banks[0] = (banks[0] & 0x38) | (A & 0x7); banks[1] = banks[0] | 0x7; break;
		case 0x20: g_NESmapper.Game800in1.mode = 0x0; banks[0] = (banks[0] & 0x38) | (A & 0x6); banks[1] = banks[0] | 0x1; break;
		case 0x30: g_NESmapper.Game800in1.mode = 0x0; banks[0] = (banks[0] & 0x38) | (A & 0x7); banks[1] = banks[0] | 0x0; break;
	}

	g_NESmapper.Game800in1.prg_bank4 = banks[0];
	g_NESmapper.Game800in1.prg_bank6 = banks[1];
	g_NESmapper.SetPROM_16K_Bank(4,banks[0]);
	g_NESmapper.SetPROM_16K_Bank(6,banks[1]);
}
void Game800in1_MemoryReadSaveRAM(uint32 A)
{
	nes6502_context cpu;
	NES6502_GetContext(&cpu);
	if(A>=0x8000)
	cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)g_NESmapper.Game800in1.mode ? (A & 0x7FF0) : A>>8 ;
	else
	cpu.mem_page[A>>13][A&NES6502_BANKMASK] = (uint8)A>>8 ;
	NES6502_SetContext(&cpu);
}


