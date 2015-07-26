void S74x374a_Init()
{
	g_NESmapper.Reset = S74x374a_Reset;
	g_NESmapper.MemoryWriteLow = S74x374a_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = S74x374a_MemoryWriteLow;
}

void S74x374a_Reset()
{
	g_NESmapper.S74x374.ctrl = 0;
	g_NESmapper.S74x374.latch[0]=
	g_NESmapper.S74x374.latch[2]=
	g_NESmapper.S74x374.latch[3]=
	g_NESmapper.S74x374.latch[4]=0;
	g_NESmapper.S74x374.latch[1]=3;
	S74LS374NASynco();
}

void S74LS374NASynco(void)
{
	g_NESmapper.SetPROM_32K_Bank(g_NESmapper.S74x374.latch[0]);
	g_NESmapper.SetVROM_8K_Bank(g_NESmapper.S74x374.latch[1]);
	S74x374a_UpdateNmt(g_NESmapper.S74x374.latch[2]);		
}

void S74x374a_UpdateNmt(unsigned int mode)
{
  switch(mode&3)
  {
    case 0:g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);break;
    case 1:g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);break;
    case 2:g_NESmapper.set_mirroring(0,1,1,1);break;
    case 3:g_NESmapper.set_mirroring(0,0,0,0);break;
  }
}

void S74x374a_MemoryWriteLow(uint32 A, uint8 V)
{
	if( (A>=0x4100)&&(A<=0x7fff) )
	{
		A&=0x4101;
		if(A==0x4100)
			g_NESmapper.S74x374.ctrl=V&7;
		else
		{
			switch(g_NESmapper.S74x374.ctrl)
			{
				case 0:	g_NESmapper.S74x374.latch[0]=0;
						g_NESmapper.S74x374.latch[1]=3;break;
				case 2:	g_NESmapper.S74x374.latch[3]=(V&1)<<3;break;
				case 4:	g_NESmapper.S74x374.latch[1]=(g_NESmapper.S74x374.latch[1]&6)|(V&3);break;
				case 5:	g_NESmapper.S74x374.latch[0]=V&1;break;
				case 6:	g_NESmapper.S74x374.latch[1]=(g_NESmapper.S74x374.latch[1]&1)|g_NESmapper.S74x374.latch[3]|((V&3)<<1);break;
				case 7:	g_NESmapper.S74x374.latch[2]=V&1;break;
			}
			S74LS374NASynco();
		}
	}
}






void S74x374b_Init()
{
	g_NESmapper.Reset = S74x374b_Reset;
	g_NESmapper.MemoryWriteLow = S74x374b_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = S74x374b_MemoryWriteLow;
	g_NESmapper.MemoryReadLow = S74x374b_MemoryReadLow;
	g_NESmapper.S74x374.dip = 0;
}


void S74LS374NSynco(void)
{
	g_NESmapper.SetPROM_32K_Bank(g_NESmapper.S74x374.latch[0]);
	g_NESmapper.SetVROM_8K_Bank(g_NESmapper.S74x374.latch[1]|g_NESmapper.S74x374.latch[3]|g_NESmapper.S74x374.latch[4]);
	S74x374a_UpdateNmt(g_NESmapper.S74x374.latch[2]);				
}

void S74x374b_Reset()
{
	S74x374a_Reset();
	g_NESmapper.S74x374.dip^=1;
	g_NESmapper.S74x374.latch[0]=
	g_NESmapper.S74x374.latch[1]=
	g_NESmapper.S74x374.latch[2]=
	g_NESmapper.S74x374.latch[3]=
	g_NESmapper.S74x374.latch[4]=0;
	S74LS374NSynco();
}
	

uint8 S74x374b_MemoryReadLow( uint32 A )
{
	uint8 ret;
	if((A&0x4100)==0x4100)
		ret=((~g_NESmapper.S74x374.ctrl)&0x3F)^g_NESmapper.S74x374.dip;
	else
		ret=(A>>8);
	return ret;
}
		
void S74x374b_MemoryWriteLow(uint32 A, uint8 V)
{
	if( A<0x4100 )
		return;
		
	A&=0x4101;
	if(A==0x4100)
		g_NESmapper.S74x374.ctrl=V&7;
	else
	{
		switch(g_NESmapper.S74x374.ctrl)
		{
			case 2:	g_NESmapper.S74x374.latch[0]=V&1; 
					g_NESmapper.S74x374.latch[3]=(V&1)<<3;break;
			case 4:	g_NESmapper.S74x374.latch[4]=(V&1)<<2;break;
			case 5:	g_NESmapper.S74x374.latch[0]=V&7;break;
			case 6:	g_NESmapper.S74x374.latch[1]=V&3;break;
			case 7:	g_NESmapper.S74x374.latch[2]=V>>1;break;
		}		
		S74LS374NSynco();
	}  
}









void S8259A_Init()
{
	g_NESmapper.Reset = S8259_Reset;
	g_NESmapper.MemoryWriteLow = S8259_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = S8259_MemoryWriteLow;
	g_NESmapper.S74x374.type = 0;
}

void S8259_Reset()
{
	int x;
	g_NESmapper.S74x374.ctrl=0;

	for(x=0;x<8;x++) g_NESmapper.S74x374.latch[x]=0;
	g_NESmapper.SetVROM_8K_Bank(0);
	S8259Synco();
}

void S8259Synco(void)
{
	int x;
	g_NESmapper.SetPROM_32K_Bank(g_NESmapper.S74x374.latch[5]&7);

	if(1)        // No CHR RAM?  Then BS'ing is ok.
	{
		for(x=0;x<4;x++)
		{
			int bank;
			if(g_NESmapper.S74x374.latch[7]&1)
				bank=(g_NESmapper.S74x374.latch[0]&0x7)|((g_NESmapper.S74x374.latch[4]&7)<<3);
			else
				bank=(g_NESmapper.S74x374.latch[x]&0x7)|((g_NESmapper.S74x374.latch[4]&7)<<3);
			switch (g_NESmapper.S74x374.type)
			{
				case 00: bank=(bank<<1)|(x&1); g_NESmapper.SetVROM_2K_Bank(2*x,bank); break;
				case 01: g_NESmapper.SetVROM_2K_Bank(2*x,bank); break;
				case 02: bank=(bank<<2)|(x&3); g_NESmapper.SetVROM_2K_Bank(2*x,bank); break;
				case 03: bank=g_NESmapper.S74x374.latch[x]&7;
				switch (x&3)
                {
					case 01: bank|=(g_NESmapper.S74x374.latch[4]&1)<<4;break;
					case 02: bank|=(g_NESmapper.S74x374.latch[4]&2)<<3;break;
					case 03: bank|=((g_NESmapper.S74x374.latch[4]&4)<<2)|((g_NESmapper.S74x374.latch[6]&1)<<3);break;
				}
                g_NESmapper.SetVROM_1K_Bank(x,bank);
                g_NESmapper.SetVROM_4K_Bank(4,~0);
                break;
			}
		}
	}
	
	if(!(g_NESmapper.S74x374.latch[7]&1))
		S74x374a_UpdateNmt(g_NESmapper.S74x374.latch[7]>>1);
	else
		g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
}


void S8259_MemoryWriteLow(uint32 A, uint8 V)
{
	A&=0x4101;
	if(A==0x4100)
		g_NESmapper.S74x374.ctrl=V;
	else
	{
		g_NESmapper.S74x374.latch[g_NESmapper.S74x374.ctrl&7]=V;
		S8259Synco();
	}
}








//SACHEN SA0161M, Mapper 146
void SA0161M_Init()
{
	g_NESmapper.Reset = SA0161M_Reset;
	g_NESmapper.MemoryWriteLow = SA0161M_MemoryWriteLow;
}
void SA0161M_Reset()
{	
  g_NESmapper.S74x374.latch[0]=0;
  SA0161MSynco();
}
void SA0161MSynco()
{
	g_NESmapper.SetPROM_32K_Bank((g_NESmapper.S74x374.latch[0]>>3)&1);
	g_NESmapper.SetVROM_8K_Bank(g_NESmapper.S74x374.latch[0]&7);
}
void SA0161M_MemoryWriteLow(uint32 A, uint8 V)
{
  if(A&0x100)
  {
    g_NESmapper.S74x374.latch[0]=V;
    SA0161MSynco();
  }
}







//147
void TCU01_Init()
{
	g_NESmapper.Reset = TCU01_Reset;
	g_NESmapper.MemoryWrite = TCU01_MemoryWriteLow;
	g_NESmapper.MemoryWriteLow = TCU01_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = TCU01_MemoryWriteLow;
}

void TCU01_Reset()
{
  g_NESmapper.S74x374.latch[0]=0;
  TCU01Synco();
}

void TCU01Synco()
{
  g_NESmapper.SetPROM_32K_Bank(((g_NESmapper.S74x374.latch[0]&0x80)>>6)|((g_NESmapper.S74x374.latch[0]>>2)&1));
  g_NESmapper.SetVROM_8K_Bank((g_NESmapper.S74x374.latch[0]>>3)&0xF);
}

void TCU01_MemoryWriteLow(uint32 A, uint8 V)
{
  if((A&0x103)==0x102)
  {
    g_NESmapper.S74x374.latch[0]=V;
    TCU01Synco();
  }
}


//148
void SA0037_Init()
{
	g_NESmapper.Reset = SA0037_Reset;
	g_NESmapper.MemoryWrite = SA0037_MemoryWrite;
}
void SA0037_Reset()
{
	g_NESmapper.S74x374.latch[0]=0;
	SA0161MSynco();
}

void SA0037_MemoryWrite(uint32 A, uint8 V)
{
  g_NESmapper.S74x374.latch[0]=V;
  SA0161MSynco();
}



//S74LS374N_Init mapper 150
void S74LS374N_Init()
{
	g_NESmapper.Reset = S74LS374N_Reset;
	g_NESmapper.MemoryReadLow = S74LS374N_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = S74LS374N_MemoryWriteLow;
	g_NESmapper.S74x374.dip=1;
}
void S74LS374N_Reset()
{
   g_NESmapper.S74x374.dip^=1;
   g_NESmapper.S74x374.latch[0]=
   g_NESmapper.S74x374.latch[1]=
   g_NESmapper.S74x374.latch[2]=
   g_NESmapper.S74x374.latch[3]=
   g_NESmapper.S74x374.latch[4]=0;
   S74LS374NSynco();
}

uint8 S74LS374N_MemoryReadLow( uint32 A )
{
  uint8 ret;
  if((A&0x4100)==0x4100)
    ret=((~g_NESmapper.S74x374.cmd)&0x3F)^g_NESmapper.S74x374.dip;
  else
    ret=A>>8;//X.DB;
  return ret;	
}

void S74LS374N_MemoryWriteLow(uint32 A, uint8 V)
{
  A&=0x4101;
  if(A==0x4100)
    g_NESmapper.S74x374.cmd=V&7;
  else
  {
    switch(g_NESmapper.S74x374.cmd)
    {
      case 2:g_NESmapper.S74x374.latch[0]=V&1; g_NESmapper.S74x374.latch[3]=(V&1)<<3;break;
      case 4:g_NESmapper.S74x374.latch[4]=(V&1)<<2;break;
      case 5:g_NESmapper.S74x374.latch[0]=V&7;break;
      case 6:g_NESmapper.S74x374.latch[1]=V&3;break;
      case 7:g_NESmapper.S74x374.latch[2]=V>>1;break;
    }
    S74LS374NSynco();
  }
}












