//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////

void NES_mapper116_Init()
{
	g_NESmapper.Reset = NES_mapper116_Reset;
	g_NESmapper.MemoryWriteLow = NES_mapper116_MemoryWriteLow;
	g_NESmapper.MemoryWrite = NES_mapper116_MemoryWrite;
	g_NESmapper.HSync = NES_mapper116_HSync;
}


void	NES_mapper116_Reset()
{
	int i;
	g_NESmapper.Mapper116.mode = 0;

	g_NESmapper.Mapper116.vrc2_prg[0] = 0x0;
	g_NESmapper.Mapper116.vrc2_prg[1] = 0x1;
	g_NESmapper.Mapper116.vrc2_nmt = 0;

	
	for (i=0; i < 8; ++i)
		g_NESmapper.Mapper116.vrc2_chr[i] = i;

	g_NESmapper.Mapper116.mmc3_ctrl = 0;
	g_NESmapper.Mapper116.mmc3_nmt = 0;

	g_NESmapper.Mapper116.mmc3_banks[0] = 0x0;
	g_NESmapper.Mapper116.mmc3_banks[1] = 0x1;
	g_NESmapper.Mapper116.mmc3_banks[2] = 0x4;
	g_NESmapper.Mapper116.mmc3_banks[3] = 0x5;
	g_NESmapper.Mapper116.mmc3_banks[4] = 0x6;
	g_NESmapper.Mapper116.mmc3_banks[5] = 0x7;

	g_NESmapper.Mapper116.mmc3_banks[6] = 0x3C;
	g_NESmapper.Mapper116.mmc3_banks[7] = 0x3D;
	g_NESmapper.Mapper116.mmc3_banks[8] = 0xFE;
	g_NESmapper.Mapper116.mmc3_banks[9] = 0xFF;

	g_NESmapper.Mapper116.mmc1_buffer = 0;
	g_NESmapper.Mapper116.mmc1_shifter = 0;

	g_NESmapper.Mapper116.mmc1_regs[0] = 0x4U|0x8U;
	g_NESmapper.Mapper116.mmc1_regs[1] = 0;
	g_NESmapper.Mapper116.mmc1_regs[2] = 0;
	g_NESmapper.Mapper116.mmc1_regs[3] = 0;

	g_NESmapper.Mapper116.irq_enable = 0;	// Disable
	g_NESmapper.Mapper116.irq_counter = 0;
	g_NESmapper.Mapper116.irq_latch = 0;
	
	NES_mapper116_MMC3_set_CPU_banks();
	NES_mapper116_MMC3_set_Nmt();
	NES_mapper116_MMC3_set_PPU_banks();
	
}

void	NES_mapper116_MMC3_set_CPU_banks()
{
	switch (g_NESmapper.Mapper116.mode & 0x3)
	{
		case 0x0:
			//prg.SwapBanks<SIZE_8K,0x0000>( vrc2.prg[0], vrc2.prg[1], 0x1E, 0x1F );
			g_NESmapper.SetPROM_32K_Banks(
				g_NESmapper.Mapper116.vrc2_prg[0], 
				g_NESmapper.Mapper116.vrc2_prg[1], 0x1E, 0x1F);
			break;

		case 0x1:
		{
			const uint32 i = g_NESmapper.Mapper116.mmc3_ctrl >> 5 & 0x2U;
			//prg.SwapBanks<SIZE_8K,0x0000>( mmc3.banks[6+i], mmc3.banks[6+1], mmc3.banks[6+(i^2)], mmc3.banks[6+3] );
			g_NESmapper.SetPROM_32K_Banks(
				g_NESmapper.Mapper116.mmc3_banks[6+i],
				g_NESmapper.Mapper116.mmc3_banks[6+1],
				g_NESmapper.Mapper116.mmc3_banks[6+(i^2)],
				g_NESmapper.Mapper116.mmc3_banks[6+3] );		
			break;
		}

		case 0x2:
		{
			const uint32 bank = g_NESmapper.Mapper116.mmc1_regs[3] & 0xFU;
			if (g_NESmapper.Mapper116.mmc1_regs[0] & 0x8U)
			{
				//prg.SwapBanks<SIZE_16K,0x0000>( (mmc1.regs[0] & 0x4U) ? bank : 0x0, (mmc1.regs[0] & 0x4U) ? 0xF : bank );
				g_NESmapper.SetPROM_16K_Bank(4,(g_NESmapper.Mapper116.mmc1_regs[0] & 0x4U) ? bank : 0x0);
				g_NESmapper.SetPROM_16K_Bank(6,(g_NESmapper.Mapper116.mmc1_regs[0] & 0x4U) ? 0xF : bank );				
			}
			else{
				//prg.SwapBank<SIZE_32K,0x0000>( bank >> 1 );
				g_NESmapper.SetPROM_32K_Bank( bank >> 1 );
			}
			break;
		}
	}
}


void	NES_mapper116_MMC3_set_PPU_banks()
{	
	const uint32 base = (g_NESmapper.Mapper116.mode & 0x4) << 6;

	switch (g_NESmapper.Mapper116.mode & 0x3)
	{
		case 0x0:
			//chr.SwapBanks<SIZE_1K,0x0000>( base|vrc2.chr[0], base|vrc2.chr[1], base|vrc2.chr[2], base|vrc2.chr[3], base|vrc2.chr[4], base|vrc2.chr[5], base|vrc2.chr[6], base|vrc2.chr[7] );
			g_NESmapper.SetVROM_8K_Banks(
				base|g_NESmapper.Mapper116.vrc2_chr[0],
				base|g_NESmapper.Mapper116.vrc2_chr[1],
				base|g_NESmapper.Mapper116.vrc2_chr[2],
				base|g_NESmapper.Mapper116.vrc2_chr[3],
				base|g_NESmapper.Mapper116.vrc2_chr[4],
				base|g_NESmapper.Mapper116.vrc2_chr[5],
				base|g_NESmapper.Mapper116.vrc2_chr[6],
				base|g_NESmapper.Mapper116.vrc2_chr[7] );			
			break;

		case 0x1:
		{
			const uint32 swap = (g_NESmapper.Mapper116.mmc3_ctrl & 0x80U) << 5;
			//chr.SwapBanks<SIZE_2K>( 0x0000 ^ swap, base >> 1 | mmc3.banks[0], base >> 1 | mmc3.banks[1] );
			g_NESmapper.SetVROM_2K_Bank( (0x0000 ^ swap)/0x400+0,base >> 1 | g_NESmapper.Mapper116.mmc3_banks[0]);
			g_NESmapper.SetVROM_2K_Bank( (0x0000 ^ swap)/0x400+2,base >> 1 | g_NESmapper.Mapper116.mmc3_banks[1]);
			//chr.SwapBanks<SIZE_1K>( 0x1000 ^ swap, base|mmc3.banks[2], base|mmc3.banks[3], base|mmc3.banks[4], base|mmc3.banks[5] );
			g_NESmapper.SetVROM_1K_Bank((0x1000 ^ swap)/0x400+0,base|g_NESmapper.Mapper116.mmc3_banks[2]);
			g_NESmapper.SetVROM_1K_Bank((0x1000 ^ swap)/0x400+1,base|g_NESmapper.Mapper116.mmc3_banks[3]);
			g_NESmapper.SetVROM_1K_Bank((0x1000 ^ swap)/0x400+2,base|g_NESmapper.Mapper116.mmc3_banks[4]);
			g_NESmapper.SetVROM_1K_Bank((0x1000 ^ swap)/0x400+3,base|g_NESmapper.Mapper116.mmc3_banks[5]);
			break;
		}

		case 0x2:
			//chr.SwapBanks<SIZE_4K,0x0000>( (mmc1.regs[0] & 0x10U) ? mmc1.regs[1] : mmc1.regs[1] & 0x1EU, (mmc1.regs[0] & 0x10U) ? mmc1.regs[2] : mmc1.regs[1] | 0x01U );
			g_NESmapper.SetVROM_4K_Bank(0,(g_NESmapper.Mapper116.mmc1_regs[0] & 0x10U) ? g_NESmapper.Mapper116.mmc1_regs[1] : g_NESmapper.Mapper116.mmc1_regs[1] & 0x1EU );
			g_NESmapper.SetVROM_4K_Bank(4,(g_NESmapper.Mapper116.mmc1_regs[0] & 0x10U) ? g_NESmapper.Mapper116.mmc1_regs[2] : g_NESmapper.Mapper116.mmc1_regs[1] | 0x01U );
			break;
	}	
}


void	NES_mapper116_MMC3_set_Nmt()
{
	uint8 nmtCtrl = 0XFF;
	switch (g_NESmapper.Mapper116.mode & 0x3)
	{
		case 0x0:

			nmtCtrl = (g_NESmapper.Mapper116.vrc2_nmt & 0x1U) ? NES_PPU_MIRROR_HORIZ   : NES_PPU_MIRROR_VERT   ;
			break;

		case 0x1:

			nmtCtrl = (g_NESmapper.Mapper116.mmc3_nmt & 0x1U) ? NES_PPU_MIRROR_HORIZ   : NES_PPU_MIRROR_VERT   ;
			break;

		case 0x2:

			switch (g_NESmapper.Mapper116.mmc1_regs[0] & 0x3U)
			{
				case 0x0: NES_PPU_set_mirroring(0,0,0,0);       break;
				case 0x1: NES_PPU_set_mirroring(1,1,1,1);        break;
				case 0x2: nmtCtrl =NES_PPU_MIRROR_VERT;   break;
				default:  nmtCtrl = NES_PPU_MIRROR_HORIZ ; break;
			}
			break;

		default: return;
	}
	if(nmtCtrl!=0xff)
	NES_PPU_set_mirroring2(nmtCtrl);
}


void Mapper116_Poke_Vrc2_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 0 );

	data &= 0x1F;
	address = address >> 13 & 0x1;

	if (g_NESmapper.Mapper116.vrc2_prg[address] != data)
	{
		g_NESmapper.Mapper116.vrc2_prg[address] = data;
		NES_mapper116_MMC3_set_CPU_banks();
	}
}

void Mapper116_Poke_Vrc2_9000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 0 );

	data &= 0x1;

	if (g_NESmapper.Mapper116.vrc2_nmt != data)
	{
		g_NESmapper.Mapper116.vrc2_nmt = data;
		NES_mapper116_MMC3_set_Nmt();
	}
}

void Mapper116_Poke_Vrc2_B000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 0 );

	data = (data & 0xF) << (address << 1 & 0x4);
	address = ((address - 0xB000) >> 11 & 0x6) | (address & 0x1);

	if (g_NESmapper.Mapper116.vrc2_chr[address] != data)
	{
		g_NESmapper.Mapper116.vrc2_chr[address] = data;
		;//ppu.Update();
		NES_mapper116_MMC3_set_PPU_banks();
	}
}

void Mapper116_Poke_Mmc3_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 1 );

	if (address & 0x1)
	{
		address = g_NESmapper.Mapper116.mmc3_ctrl & 0x7U;

		if (address < 2)
			data >>= 1;

		if (g_NESmapper.Mapper116.mmc3_banks[address] != data)
		{
			g_NESmapper.Mapper116.mmc3_banks[address] = data;

			if (address < 6)
			{
				;//ppu.Update();
				NES_mapper116_MMC3_set_PPU_banks();
			}
			else
			{
				NES_mapper116_MMC3_set_CPU_banks();
			}
		}
	}
	else
	{
		address = g_NESmapper.Mapper116.mmc3_ctrl ^ data;
		g_NESmapper.Mapper116.mmc3_ctrl = data;

		if (address & 0x40)
			NES_mapper116_MMC3_set_CPU_banks();

		if (address & (0x80U|0x07U))
		{
			//ppu.Update();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}

void Mapper116_Poke_Mmc3_A000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 1 );

	if (!(address & 0x1))
	{
		if (g_NESmapper.Mapper116.mmc3_nmt != data)
		{
			g_NESmapper.Mapper116.mmc3_nmt = data;
			NES_mapper116_MMC3_set_Nmt();
		}
	}
}

void Mapper116_Poke_Mmc3_C000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 1 );

	;//irq.Update();

	if (address & 0x1)
		g_NESmapper.Mapper116.irq_latch = data;//irq.unit.Reload();
	else
		g_NESmapper.Mapper116.irq_counter = data;//irq.unit.SetLatch( data );
}

void Mapper116_Poke_Mmc3_E000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 1 );

	;//irq.Update();

	if (address & 0x1)
		g_NESmapper.Mapper116.irq_enable = 0xFF;//Enable
	else
	{
		g_NESmapper.Mapper116.irq_counter = g_NESmapper.Mapper116.irq_latch;
		g_NESmapper.Mapper116.irq_enable = 0;
		nes6502_ClrIRQ( IRQ_MAPPER );//
	}
}

void Mapper116_Poke_Mmc1_8000(uint32 address,uint32 data)
{
	//NST_ASSERT( (g_NESmapper.Mapper116.mode & 0x3) == 2 );

	if (!(data & 0x80))
	{
		g_NESmapper.Mapper116.mmc1_buffer |= 
		(data & 0x1) << g_NESmapper.Mapper116.mmc1_shifter++;

		if (g_NESmapper.Mapper116.mmc1_shifter != 5)
			return;

		g_NESmapper.Mapper116.mmc1_shifter = 0;
		data = g_NESmapper.Mapper116.mmc1_buffer;
		g_NESmapper.Mapper116.mmc1_buffer = 0;

		address = address >> 13 & 0x3;

		if (g_NESmapper.Mapper116.mmc1_regs[address] != data)
		{
			g_NESmapper.Mapper116.mmc1_regs[address] = data;

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
	else
	{
		g_NESmapper.Mapper116.mmc1_buffer = 0;
		g_NESmapper.Mapper116.mmc1_shifter = 0;

		if ((g_NESmapper.Mapper116.mmc1_regs[0] & (0x4U|0x8U)) != (0x4U|0x8U))
		{
			g_NESmapper.Mapper116.mmc1_regs[0] |= (0x4U|0x8U);

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}

void NES_mapper116_MemoryWriteLow(uint32 addr, uint8 data)
{
		
	if( (addr & 0x4100) == 0x4100 ) 
	{
		if (g_NESmapper.Mapper116.mode != data)
		{
			g_NESmapper.Mapper116.mode = data;

			if ((data & 0x3) != 1)
			{
				//nes6502_ClrIRQ( IRQ_MAPPER );//irq.unit.Disable( cpu );
				g_NESmapper.Mapper116.irq_counter = g_NESmapper.Mapper116.irq_latch;
				g_NESmapper.Mapper116.irq_enable = 0;
				nes6502_ClrIRQ( IRQ_MAPPER );//
			}

			NES_mapper116_MMC3_set_CPU_banks();
			NES_mapper116_MMC3_set_Nmt();
			NES_mapper116_MMC3_set_PPU_banks();
		}
	}
}

void NES_mapper116_MemoryWrite(uint32 address, uint8 data)
{
	switch( address & 0xF000 ) {
		case	0x8000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_8000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_8000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;	
			
		case	0x9000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_9000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_8000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
			
		case	0xA000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_8000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_A000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xB000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_A000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xC000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_C000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xD000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_C000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xE000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: Mapper116_Poke_Vrc2_B000( address, data ); break;
				case 0x1: Mapper116_Poke_Mmc3_E000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
		case	0xF000:
			switch (g_NESmapper.Mapper116.mode & 0x3)
			{
				case 0x0: break;
				case 0x1: Mapper116_Poke_Mmc3_E000( address, data ); break;
				case 0x2: Mapper116_Poke_Mmc1_8000( address, data ); break;
			}
			break;
	}
}

void NES_mapper116_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( g_NESmapper.Mapper116.irq_counter <= 0 ) {
			if( g_NESmapper.Mapper116.irq_enable ) {
				nes6502_SetIRQ( IRQ_MAPPER );
				return;
			}
		}

		if( (NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) ) {
			g_NESmapper.Mapper116.irq_counter--;
		}
	}
}

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// mapper 116b base on mmc3
void Mapper116B_MemoryWriteLow(uint32 A, uint8 V)
{
	if(A==0x4100){
		g_NESmapper.Mapper116.exPreg=V;
		Mmc3_UpdateChr();
	}
}
void Mapper116B_UpdateChr(unsigned int A,unsigned int V)
{
// setchr1(A,V|((exPreg[0]&0x4)<<6));
 if(g_NESmapper.Mapper116.exPreg&2)
 {
    //g_NESmapper.SetVROM_8K_Bank(0);//0x10
    g_NESmapper.SetCRAM_8K_Bank(0);//0x10
    /*g_NESmapper.SetVROM_1K_BankR(0x10,0,0);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,1,1);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,2,2);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,3,3);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,4,4);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,5,5);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,6,6);//0x10
    g_NESmapper.SetVROM_1K_BankR(0x10,7,7);//0x10*/
	}
 else
    g_NESmapper.SetVROM_1K_Bank(A>>10,V);
}
void Mapper116B_Reset()
{
	g_NESmapper.Mapper116.exPreg=0;
	Mmc3_Reset();
}
void Mapper116B_Init()
{
	Mmc3_Init();
	UpdateChr=Mapper116B_UpdateChr;
	g_NESmapper.MemoryWriteLow = Mapper116B_MemoryWriteLow;
	g_NESmapper.Reset = Mapper116B_Reset;
}




