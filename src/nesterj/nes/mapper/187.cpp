
/////////////////////////////////////////////////////////////////////
// Mapper 187

void NES_mapper187_Init()
{
	Mmc3_Init();
	
	UpdatePrg = NES_mapper187_UpdatePrg;
	UpdateChr = NES_mapper187_UpdateChr;
	
	g_NESmapper.Reset = NES_mapper187_Reset;
	g_NESmapper.MemoryWrite = NES_mapper187_MemoryWrite;
	g_NESmapper.MemoryWriteLow = NES_mapper187_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = NES_mapper187_MemoryReadLow;
}

void NES_mapper187_Reset()
{	
	uint8 i;
	for (i=0; i < 4; ++i)
		g_NESmapper.Mapper187.exRegs[i] = 0;
							
	Mmc3_Reset();		
	g_NESmapper.Mapper187.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x2000;		
	g_NESmapper.SetPROM_Bank( 2, g_NESmapper.Mapper187.wram);	
}


void  NES_mapper187_UpdatePrg(unsigned int addr,unsigned int bank)
{
	if (!(g_NESmapper.Mapper187.exRegs[0] & 0x80U))
		g_NESmapper.SetPROM_8K_Bank( (addr>>13)+4, bank);
}

void  NES_mapper187_UpdateChr(unsigned int addr,unsigned int bank)
{
	if ((addr & 0x1000) == (g_NESmapper.Mmc3.ctrl0 << 5 & 0x1000))
		bank |= 0x100;
	Mmc3_SwapChr1K(addr>>10,bank);
}

uint8 NES_mapper187_MemoryReadLow(uint32 addr)
{
	if( (addr>=0x5000)&&(addr<=0x5fff) )
	{
		static const uint8 security[4] =
		{
			0x83,0x83,0x42,0x00
		};

		return security[g_NESmapper.Mapper187.exRegs[1] & 0x3U];
	}
	return (uint8)(addr>>8);
}

void NES_mapper187_MemoryWriteLow(uint32 addr, uint8 data)
{
	if(addr==0x5000)
	{
		g_NESmapper.Mapper187.exRegs[1] = data;

		if (g_NESmapper.Mapper187.exRegs[0] != data)
		{
			g_NESmapper.Mapper187.exRegs[0] = data;

			if (g_NESmapper.Mapper187.exRegs[0] & 0x80U)
			{
				const unsigned int bank = g_NESmapper.Mapper187.exRegs[0] & 0x1FU;

				if (g_NESmapper.Mapper187.exRegs[0] & 0x20U)
					g_NESmapper.SetPROM_32K_Bank(bank >> 2);
				else
				{
					g_NESmapper.SetPROM_16K_Bank(4,bank);
					g_NESmapper.SetPROM_16K_Bank(6,bank);
					}
			}
			else
			{
				Mmc3_UpdatePrg();
			}
		}
		return;
	}else if( (addr>=0x5001)&&(addr<=0x5fff) )
	{	
		g_NESmapper.Mapper187.exRegs[1] = data;

		if (!g_NESmapper.Mapper187.exRegs[3])
		{
			g_NESmapper.Mapper187.exRegs[3] = 1;
			//cpu.Poke( 0x4017, 0x40 );
			//NES_MemoryWrite(0x4017, 0x40 );
			NES_WriteHighRegs(0x4017, 0x40);
			g_NESmapper.WriteHighRegs(0x4017, 0x40);
		}
	}
}

void NES_mapper187_MemoryWrite(uint32 addr, uint8 data)
{
	if( (addr>=0x8000)&&(addr<0xA000) )
	{
		switch(addr&0x3)
		{
			case 0:
					g_NESmapper.Mapper187.exRegs[2] = 1;
					Poke_8000(addr,data);break;
			case 1:
					if (g_NESmapper.Mapper187.exRegs[2])
						Poke_8001(addr,data);break;
			case 2:break;//dummy
			case 3:
					g_NESmapper.Mapper187.exRegs[2] = 0;

					if (data == 0x28)
					{
						//prg.SwapBank<SIZE_8K,0x4000>( 0x17 );
						g_NESmapper.SetPROM_8K_Bank(6,0x17);
					}
					else if (data == 0x2A)
					{
						//prg.SwapBank<SIZE_8K,0x2000>( 0x0F );
						g_NESmapper.SetPROM_8K_Bank(5,0x0F);
					}break;
		}
	}else
	{	switch( addr & 0xE001 ) 
		{
			//case	0x8000:	Poke_8000(addr,data);break;	
			//case	0x8001:	Poke_8001(addr,data);break;	
			case	0xA000:	Poke_A000(addr,data);break;	
			case	0xA001:	Poke_A001(addr,data);break;	
			case	0xC000:	Poke_C000(addr,data);break;	
			case	0xC001:	Poke_C001(addr,data);break;				
			case	0xE000:	Poke_E000(addr,data);break;
			case	0xE001:	Poke_E001(addr,data);break;
		}
		//Mmc3_MemoryWrite(addr,data);
	}
}