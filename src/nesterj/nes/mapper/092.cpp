
/////////////////////////////////////////////////////////////////////
// Mapper 92
void NES_mapper92_Init()
{
	g_NESmapper.Reset = NES_mapper92_Reset;
	g_NESmapper.MemoryWrite = NES_mapper92_MemoryWrite;
}

void NES_mapper92_Reset()
{
	// set CPU bank pointers
	g_NESmapper.set_CPU_banks4(0,1,g_NESmapper.num_8k_ROM_banks-2,g_NESmapper.num_8k_ROM_banks-1);

	// set PPU bank pointers
	if(g_NESmapper.num_1k_VROM_banks)
	{
		g_NESmapper.set_PPU_banks8(0,1,2,3,4,5,6,7);
	}
}

// Created by rinao
void NES_mapper92_MemoryWrite(uint32 addr, uint8 data)
{
	uint8 c_bank, p_bank;

	data = (uint8)addr & 0xff;
	c_bank = (data & 0x0f) << 1;
	p_bank = data & 0x0f;

	if (addr >= 0x9000){ // Moero!! ProSoccer
		if ((data & 0xf0) == 0xd0){
			g_NESmapper.set_CPU_banks4(0,1,c_bank,c_bank+1);
		} else if ((data & 0xf0) == 0xe0){
			g_NESmapper.set_PPU_banks8(p_bank*8,p_bank*8+1,p_bank*8+2,p_bank*8+3,p_bank*8+4,p_bank*8+5,p_bank*8+6,p_bank*8+7);
		}
	} else { // Moero!! Proyakyuu '88 ketteiban
		if ((data & 0xf0) == 0xb0){
			g_NESmapper.set_CPU_banks4(0,1,c_bank,c_bank+1);
		} else if ((data & 0xf0) == 0x70){
			g_NESmapper.set_PPU_banks8(p_bank*8,p_bank*8+1,p_bank*8+2,p_bank*8+3,p_bank*8+4,p_bank*8+5,p_bank*8+6,p_bank*8+7);
		}
	}
}
/////////////////////////////////////////////////////////////////////

