/////////////////////////////////////////////////////////////////////
// Mapper 249
void NES_mapper249_Init()
{
	g_NESmapper.Reset = NES_mapper249_Reset;
	g_NESmapper.MemoryWrite = NES_mapper249_MemoryWrite;
	g_NESmapper.MemoryWriteLow = NES_mapper249_MemoryWriteLow;
	g_NESmapper.HSync = NES_mapper249_HSync;
}

void NES_mapper249_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) {
		g_NESmapper.Mapper249.regs[i] = 0x00;
	}
	g_NESmapper.Mapper249.prg0 = 0;
	g_NESmapper.Mapper249.prg1 = 1;

	//SetPROM_32K_Bank( 0,1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	g_NESmapper.Mapper249.chr01 = 0;
	g_NESmapper.Mapper249.chr23 = 2;
	g_NESmapper.Mapper249.chr4  = 4;
	g_NESmapper.Mapper249.chr5  = 5;
	g_NESmapper.Mapper249.chr6  = 6;
	g_NESmapper.Mapper249.chr7  = 7;

	//SetVROM_8K_Bank( 0 );
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);

	g_NESmapper.Mapper249.we_sram  = 0;	// Disable
	g_NESmapper.Mapper249.irq_enable = 0;	// Disable
	g_NESmapper.Mapper249.irq_counter = 0;
	g_NESmapper.Mapper249.irq_latch = 0;
	g_NESmapper.Mapper249.irq_request = 0;

	// IRQƒ^ƒCƒvÝ’è
	//nes->SetIrqType( NES::IRQ_CLOCK );

	g_NESmapper.Mapper249.spdata = 0;	
}


void NES_mapper249_MemoryWriteLow(uint32 addr, uint8 data)
{
	if( addr == 0x5000 ) {
		switch( data ) {
			case	0x00:
				g_NESmapper.Mapper249.spdata = 0;
				break;
			case	0x02:
				g_NESmapper.Mapper249.spdata = 1;
				break;
		}
	}

	if( addr>=0x6000 && addr<0x8000 ) {
		;//CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}

void NES_mapper249_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 m0,m1,m2,m3,m4,m5,m6,m7;
	nes6502_context context;

	switch( addr&0xFF01 ) {
		case	0x8000:
		case    0x8800:
			g_NESmapper.Mapper249.regs[0] = data;
			break;
		case	0x8001:
		case    0x8801:
			switch( g_NESmapper.Mapper249.regs[0] & 0x07 ) {
				case	0x00:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
						m0=data&0x1;
						m1=(data&0x02)>>1;
						m2=(data&0x04)>>2;
						m3=(data&0x08)>>3;
						m4=(data&0x10)>>4;
						m5=(data&0x20)>>5;
						m6=(data&0x40)>>6;
						m7=(data&0x80)>>7;
						data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 0, data&0xFE );
					//SetVROM_1K_Bank( 1, data|0x01 );
					g_NESmapper.set_PPU_bank(0,data&0xFE);
					g_NESmapper.set_PPU_bank(1,data|0x01);
					break;
				case	0x01:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
						m0=data&0x1;
						m1=(data&0x02)>>1;
						m2=(data&0x04)>>2;
						m3=(data&0x08)>>3;
						m4=(data&0x10)>>4;
						m5=(data&0x20)>>5;
						m6=(data&0x40)>>6;
						m7=(data&0x80)>>7;
						data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 2, data&0xFE );
					//SetVROM_1K_Bank( 3, data|0x01 );
					g_NESmapper.set_PPU_bank(2,data&0xFE);
					g_NESmapper.set_PPU_bank(3,data|0x01);
					break;
				case	0x02:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 4, data );
					g_NESmapper.set_PPU_bank(4,data);
					break;
				case	0x03:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 5, data );
					g_NESmapper.set_PPU_bank(5,data);
					break;
				case	0x04:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 6, data );
					g_NESmapper.set_PPU_bank(6,data);
					break;
				case	0x05:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
					}
					//SetVROM_1K_Bank( 7, data );
					g_NESmapper.set_PPU_bank(7,data);
					break;
				case	0x06:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
						if( data < 0x20 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=0;
							m6=0;
							m7=0;
							data=(m7<<7)|(m6<<6)|(m5<<5)|(m2<<4)|(m1<<3)|(m3<<2)|(m4<<1)|m0;
						} else {
							data=data-0x20;
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
						}
					}
					//SetPROM_8K_Bank( 4, data );
					g_NESmapper.set_CPU_bank4(data);
					break;
				case	0x07:
					if( g_NESmapper.Mapper249.spdata == 1 ) {
						if( data < 0x20 ) {
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=0;
							m6=0;
							m7=0;
							data=(m7<<7)|(m6<<6)|(m5<<5)|(m2<<4)|(m1<<3)|(m3<<2)|(m4<<1)|m0;
						} else {
							data=data-0x20;
							m0=data&0x1;
							m1=(data&0x02)>>1;
							m2=(data&0x04)>>2;
							m3=(data&0x08)>>3;
							m4=(data&0x10)>>4;
							m5=(data&0x20)>>5;
							m6=(data&0x40)>>6;
							m7=(data&0x80)>>7;
							data=(m5<<7)|(m4<<6)|(m2<<5)|(m6<<4)|(m7<<3)|(m3<<2)|(m1<<1)|m0;
						}
					}
					//SetPROM_8K_Bank( 5, data );
					g_NESmapper.set_CPU_bank5(data);
					break;
			}
			break;
		case	0xA000:
		case    0xA800:
			g_NESmapper.Mapper249.regs[2] = data;
			/*if( !nes->rom->Is4SCREEN() ) {
				if( data & 0x01 ) SetVRAM_Mirror( VRAM_HMIRROR );
				else		  SetVRAM_Mirror( VRAM_VMIRROR );
			}*/
			if(NES_ROM_get_mirroring() != NES_PPU_MIRROR_FOUR_SCREEN)
			{
				if(data & 0x01)
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
				}
				else
				{
					g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
				}
			}
			break;
		case	0xA001:
		case    0xA801:
			g_NESmapper.Mapper249.regs[3] = data;
			break;
		case	0xC000:
		case    0xC800:
			g_NESmapper.Mapper249.regs[4] = data;
			g_NESmapper.Mapper249.irq_counter = data;
			g_NESmapper.Mapper249.irq_request = 0;
			//nes->cpu->ClrIRQ( IRQ_MAPPER );
			NES6502_GetContext(&context);
			context.int_pending &= ~0x10;
			NES6502_SetContext(&context);	
			break;
		case	0xC001:
		case    0xC801:
			g_NESmapper.Mapper249.regs[5] = data;
			g_NESmapper.Mapper249.irq_latch = data;
			g_NESmapper.Mapper249.irq_request = 0;
			//nes->cpu->ClrIRQ( IRQ_MAPPER );
			NES6502_GetContext(&context);
			context.int_pending &= ~0x10;
			NES6502_SetContext(&context);	
			break;
		case	0xE000:
		case    0xE800:
			g_NESmapper.Mapper249.regs[6] = data;
			g_NESmapper.Mapper249.irq_enable = 0;
			g_NESmapper.Mapper249.irq_request = 0;
			//nes->cpu->ClrIRQ( IRQ_MAPPER );
			NES6502_GetContext(&context);
			context.int_pending &= ~0x10;
			NES6502_SetContext(&context);	
			break;
		case	0xE001:
		case    0xE801:
			g_NESmapper.Mapper249.regs[7] = data;
			g_NESmapper.Mapper249.irq_enable = 1;
			g_NESmapper.Mapper249.irq_request = 0;
			//nes->cpu->ClrIRQ( IRQ_MAPPER );
			NES6502_GetContext(&context);
			context.int_pending &= ~0x10;
			NES6502_SetContext(&context);	
			break;
	}
}

void NES_mapper249_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if(NES_PPU_spr_enabled() || NES_PPU_bg_enabled()) {
			if( g_NESmapper.Mapper249.irq_enable && !g_NESmapper.Mapper249.irq_request ) {
				if( scanline == 0 ) {
					if( g_NESmapper.Mapper249.irq_counter ) {
						g_NESmapper.Mapper249.irq_counter--;
					}
				}
				if( !(g_NESmapper.Mapper249.irq_counter--) ) {
					g_NESmapper.Mapper249.irq_request = 0xFF;
					g_NESmapper.Mapper249.irq_counter = g_NESmapper.Mapper249.irq_latch;
					//nes->cpu->SetIRQ( IRQ_MAPPER );
					//NES6502_DoIRQ();
					nes6502_context context;
					NES6502_GetContext(&context);
					context.int_pending |= 0x10;
					NES6502_SetContext(&context);	
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////

