
void Kof97_Init()
{
	Mmc3_Init();
	
	g_NESmapper.Reset = Mmc3_Reset;
	g_NESmapper.MemoryWrite = Kof97_MemoryWrite;
}

void Kof97_MemoryWrite(uint32 A, uint8 V)
{
	if(A<=0xA000)
	{
		V=(V&0xD8)|((V&0x20)>>4)|((V&4)<<3)|((V&2)>>1)|((V&1)<<2);  //76143502
		if(A==0x9000) A=0x8001;  
		//MMC3_CMDWrite(A,V);		
	}else if( (A>=0xc000)&&(A<=0xf000))
	{
		V=(V&0xD8)|((V&0x20)>>4)|((V&4)<<3)|((V&2)>>1)|((V&1)<<2);
		if(A==0xD000) A=0xC001;
		else if(A==0xF000) A=0xE001;
		//MMC3_IRQWrite(A,V);
	}else return;
	switch (A&0xf001)
	{
		case 0x8000: Poke_8000(A, V ); break;
		case 0x8001: Poke_8001(A, V ); break;
		case 0xA000: Poke_A000(A, V ); break;
		case 0xA001: Poke_A001(A, V ); break;
		case 0xC000: Poke_C000(A, V ); break;
		case 0xC001: Poke_C001(A, V ); break;
		case 0xE000: Poke_E000(A, V ); break;
		case 0xE001: Poke_E001(A, V ); break;
	}
}
//////////////