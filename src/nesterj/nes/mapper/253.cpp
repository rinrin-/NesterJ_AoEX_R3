//////////////////////////////////////////////////////////////////////////
// Mapper253  WaiXing                                                   //
//////////////////////////////////////////////////////////////////////////

void NES_mapper253_Init()
{
	g_NESmapper.Reset = NES_mapper253_Reset;
	g_NESmapper.MemoryWrite = NES_mapper253_MemoryWrite;
	g_NESmapper.HSync = NES_mapper253_HSync;
}

void NES_mapper253_Reset()
{
	int i;
	for( i = 0; i < 8; i++ ) g_NESmapper.Mapper253.regs[i] = i;
	g_NESmapper.Mapper253.regs[8] = 0;
	g_NESmapper.Mapper253.irq_enable = 0;
	g_NESmapper.Mapper253.irq_counter = 0;
	g_NESmapper.Mapper253.irq_latch = 0;
	g_NESmapper.Mapper253.VRAM_switch = 0;
	g_NESmapper.Mapper253.rom_type = 0;
	g_NESmapper.set_CPU_banks4(0, 1, g_NESmapper.num_8k_ROM_banks-2, g_NESmapper.num_8k_ROM_banks-1);
	g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	
	//g_PPU.vram_write_protect = 0;
	
	if( NES_crc32() == 0x0530b26e ) 
	{
		g_NESmapper.Mapper253.rom_type = 1;
	}
}

void NES_mapper253_MemoryWrite(uint32 addr, uint8 data)
{
	if( addr == 0x8010 ) {
		g_NESmapper.set_CPU_bank4(data);
		return;
	}
	if( addr == 0xA010 ) {
		g_NESmapper.set_CPU_bank5(data);
		return;
	}
	if( addr == 0x9400 ) {
		data &= 0x03;
		if( data == 0 )	     g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
		else if( data == 1 ) g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
		else if( data == 2 ) g_NESmapper.set_mirroring(0,0,0,0);
		else		     g_NESmapper.set_mirroring(1,1,1,1);
	}
	switch( addr & 0xF00C ) {
		case 0xB000:
			g_NESmapper.Mapper253.regs[0] = (g_NESmapper.Mapper253.regs[0] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 0, g_NESmapper.Mapper253.regs[0] );
			break;	
		case 0xB004:
			g_NESmapper.Mapper253.regs[0] = (g_NESmapper.Mapper253.regs[0] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 0, g_NESmapper.Mapper253.regs[0] + ((data>>4)*0x100) );
			break;
		case 0xB008:
			g_NESmapper.Mapper253.regs[1] = (g_NESmapper.Mapper253.regs[1] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 1, g_NESmapper.Mapper253.regs[1] );
			break;
		case 0xB00C:
			g_NESmapper.Mapper253.regs[1] = (g_NESmapper.Mapper253.regs[1] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 1, g_NESmapper.Mapper253.regs[1] + ((data>>4)*0x100) );
			break;
		case 0xC000:
			g_NESmapper.Mapper253.regs[2] = (g_NESmapper.Mapper253.regs[2] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 2, g_NESmapper.Mapper253.regs[2] );
			break;
		case 0xC004:
			g_NESmapper.Mapper253.regs[2] = (g_NESmapper.Mapper253.regs[2] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 2, g_NESmapper.Mapper253.regs[2] + ((data>>4)*0x100) );
			break;
		case 0xC008:
			g_NESmapper.Mapper253.regs[3] = (g_NESmapper.Mapper253.regs[3] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 3, g_NESmapper.Mapper253.regs[3] );
			break;
		case 0xC00C:
			g_NESmapper.Mapper253.regs[3] = (g_NESmapper.Mapper253.regs[3] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 3, g_NESmapper.Mapper253.regs[3] + ((data>>4)*0x100) );
			break;
		case 0xD000:
			g_NESmapper.Mapper253.regs[4] = (g_NESmapper.Mapper253.regs[4] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 4, g_NESmapper.Mapper253.regs[4] );
			break;
		case 0xD004:
			g_NESmapper.Mapper253.regs[4] = (g_NESmapper.Mapper253.regs[4] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 4, g_NESmapper.Mapper253.regs[4] + ((data>>4)*0x100) );
			break;
		case 0xD008:
			g_NESmapper.Mapper253.regs[5] = (g_NESmapper.Mapper253.regs[5] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 5, g_NESmapper.Mapper253.regs[5] );
			break;
		case 0xD00C:
			g_NESmapper.Mapper253.regs[5] = (g_NESmapper.Mapper253.regs[5] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 5, g_NESmapper.Mapper253.regs[5] + ((data>>4)*0x100) );
			break;
		case 0xE000:
			g_NESmapper.Mapper253.regs[6] = (g_NESmapper.Mapper253.regs[6] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 6, g_NESmapper.Mapper253.regs[6] );
			break;
		case 0xE004:
			g_NESmapper.Mapper253.regs[6] = (g_NESmapper.Mapper253.regs[6] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 6, g_NESmapper.Mapper253.regs[6] + ((data>>4)*0x100) );
			break;
		case 0xE008:
			g_NESmapper.Mapper253.regs[7] = (g_NESmapper.Mapper253.regs[7] & 0xF0) | (data & 0x0F);
			NES_mapper253_SetBank_PPUSUB( 7, g_NESmapper.Mapper253.regs[7] );
			break;
		case 0xE00C:
			g_NESmapper.Mapper253.regs[7] = (g_NESmapper.Mapper253.regs[7] & 0x0F) | ((data & 0x0F) << 4);
			NES_mapper253_SetBank_PPUSUB( 7, g_NESmapper.Mapper253.regs[7] + ((data>>4)*0x100) );
			break;
		case 0xF000:
			g_NESmapper.Mapper253.irq_latch = (g_NESmapper.Mapper253.irq_latch & 0xF0) | (data & 0x0F);
			break;
		case 0xF004:
			g_NESmapper.Mapper253.irq_latch = (g_NESmapper.Mapper253.irq_latch & 0x0F) | ((data & 0x0F) << 4);
			break;
		case 0xF008:
			g_NESmapper.Mapper253.irq_enable = data & 0x03;
			if( g_NESmapper.Mapper253.irq_enable & 0x02 ) g_NESmapper.Mapper253.irq_counter = g_NESmapper.Mapper253.irq_latch;
			break;
	}
}

void NES_mapper253_SetBank_PPUSUB( int bank, int page )
{
	if(g_NESmapper.Mapper253.rom_type == 1){
		if(page == 0x88){
			g_NESmapper.Mapper253.VRAM_switch = 0;
			return;
		}else if(page == 0xc8){
			g_NESmapper.Mapper253.VRAM_switch = 1;
			return;
		}
	}
	if( (page == 4) || (page == 5) ) {
		if( (g_NESmapper.Mapper253.VRAM_switch==0)&&(g_NESmapper.Mapper253.rom_type==1) ){			
			NES_mapper253_set_PPU_bank( bank, page );
		} else {
			g_NESmapper.set_VRAM_bank( bank, page );
		}
	} else {
			NES_mapper253_set_PPU_bank( bank, page );
	}
}

void NES_mapper253_set_PPU_bank( uint8 bank, uint32 page )
{
	switch( bank ) {
		case 0x00:
			g_NESmapper.set_PPU_bank0(page);
			break;
		case 0x01:
			g_NESmapper.set_PPU_bank1(page);
			break;
		case 0x02:
			g_NESmapper.set_PPU_bank2(page);
			break;
		case 0x03:
			g_NESmapper.set_PPU_bank3(page);
			break;
		case 0x04:
			g_NESmapper.set_PPU_bank4(page);
			break;
		case 0x05:
			g_NESmapper.set_PPU_bank5(page);
			break;
		case 0x06:
			g_NESmapper.set_PPU_bank6(page);
			break;
		case 0x07:
			g_NESmapper.set_PPU_bank7(page);
			break;
		}
}

void NES_mapper253_HSync(uint32 scanline)
{
	if( g_NESmapper.Mapper253.irq_enable & 0x02 ) {
		if( g_NESmapper.Mapper253.irq_counter == 0xFF ) {
			g_NESmapper.Mapper253.irq_counter = g_NESmapper.Mapper253.irq_latch;
			g_NESmapper.Mapper253.irq_enable = (g_NESmapper.Mapper253.irq_enable & 0x01) * 3;
			NES6502_DoIRQ();
		} else {
			g_NESmapper.Mapper253.irq_counter++;
		}
	}
}
