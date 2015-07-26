#ifdef _NES_MAPPER_CPP_

#include "../nes_string.h"
#include "../../fileio.h"

/////////////////////////////////////////////////////////////////////
// Mapper 20
void NES_mapper20_Init()
{
	g_NESmapper.Reset = NES_mapper20_Reset;
	g_NESmapper.MemoryReadLow = NES_mapper20_MemoryReadLow;
	g_NESmapper.MemoryWriteLow = NES_mapper20_MemoryWriteLow;
	g_NESmapper.MemoryWriteSaveRAM = NES_mapper20_MemoryWriteSaveRAM;
	g_NESmapper.MemoryWrite = NES_mapper20_MemoryWrite;
	g_NESmapper.HSync = NES_mapper20_HSync;
	g_NESmapper.VSync = NES_mapper20_VSync;
	g_NESmapper.GetDiskSideNum = NES_mapper20_GetDiskSideNum;
	g_NESmapper.GetDiskSide = NES_mapper20_GetDiskSide;
	g_NESmapper.SetDiskSide = NES_mapper20_SetDiskSide;
	g_NESmapper.GetDiskDatap = NES_mapper20_GetDiskDatap;
	g_NESmapper.DiskAccessed = NES_mapper20_DiskAccessed;
}

void NES_mapper20_Reset()
{
	int hFile;
	char fn[256];
	nes6502_context context;
	// Init ExSound
	NES_APU_SelectExSound(4);

	g_NESmapper.Mapper20.wram = g_NES.mapper_extram;
	g_NES.mapper_extramsize = 0x8000;
	_memset(g_NESmapper.Mapper20.wram, 0, 0x8000);

	// DISKSYS.ROMのパスを生成
	GetModulePath(fn, sizeof(fn));
	_strcat(fn, "DISKSYS.ROM");

	if((hFile = NES_fopen(fn, FILE_MODE_READ)) >= 0)
	{
		uint8 head1 = NES_fgetc(hFile);
		uint8 head2 = NES_fgetc(hFile);
		uint8 head3 = NES_fgetc(hFile);
		if(head1 == 'N' && head2 == 'E' && head3 == 'S')
		{
			NES_fseek(hFile, 0x6010, FILE_SEEK_SET);
		}
		else
		{
			NES_fseek(hFile, 0, FILE_SEEK_SET);
		}
#if 0
		for(uint32 i = 0; i < 0x2000; i++)
		{
			g_NESmapper.Mapper20.bios[i] = NES_fgetc(hFile);
		}
#else
		NES_fread(g_NESmapper.Mapper20.bios, 1, 0x2000, hFile);
#endif
		NES_fclose(hFile);
	}
	else
	{
		// とりあえずスルーしちゃおう
		//throw "DISKSYS.ROM not found.";
		return;
	}

	// cancel license screen
	//g_NESmapper.Mapper20.bios[0xfb0] = 0x00;

	NES6502_GetContext(&context);
	context.mem_page[3] = g_NESmapper.Mapper20.wram + 0x0000;
	context.mem_page[4] = g_NESmapper.Mapper20.wram + 0x2000;
	context.mem_page[5] = g_NESmapper.Mapper20.wram + 0x4000;
	context.mem_page[6] = g_NESmapper.Mapper20.wram + 0x6000;
	context.mem_page[7] = g_NESmapper.Mapper20.bios;
	NES6502_SetContext(&context);

	// read FDS g_NESmapper.Mapper20.disk image
	if(g_NESmapper.ROM_banks[0] == 'F' && g_NESmapper.ROM_banks[1] == 'D' && g_NESmapper.ROM_banks[2] == 'S')
	{
#if 0
		for(uint32 i = 0; i < g_NESmapper.ROM_banks[4]; i++)
		{
			for(uint32 j = 0; j < 65500; j++)
			{
				g_NESmapper.Mapper20.disk[0x10000*i+j] = g_NESmapper.ROM_banks[16+65500*i+j];
			}
		}
#else
//		for(uint32 i = 0; i < g_NESmapper.ROM_banks[4]; i++){
//			_memcpy(&g_NESmapper.Mapper20.disk[65500*i], &g_NESmapper.ROM_banks[16+65500*i], 65500);
//		}
		_memcpy(g_NESmapper.Mapper20.disk, &g_NESmapper.ROM_banks[16], g_NESmapper.ROM_banks[4]*65500);
#endif
	}
	else
	{
		return;
		//throw "Invalid g_NESmapper.Mapper20.disk image.";
	}

	g_NESmapper.Mapper20.patch = 0;
	if(NES_fds_id() == 0xc7525744) // Reflect World
	{
		g_NES.frame_irq_disenabled = 1;
	}
	else if(NES_fds_id() == 0x01534d42) // SMB2
	{
		g_NESmapper.Mapper20.patch = 1;
	}

	g_NESmapper.Mapper20.irq_enabled = 0;
	g_NESmapper.Mapper20.irq_counter = 0;
	g_NESmapper.Mapper20.irq_latch = 0;
	g_NESmapper.Mapper20.irq_wait = 0;

	g_NESmapper.Mapper20.access_flag = 0;
	g_NESmapper.Mapper20.disk_enabled = 1;
	g_NESmapper.Mapper20.head_position = 0;
	g_NESmapper.Mapper20.write_skip = 0;
	g_NESmapper.Mapper20.disk_status = 0;
	g_NESmapper.Mapper20.write_reg = 0;
	g_NESmapper.Mapper20.current_side = 1;
	g_NESmapper.Mapper20.last_side = 1;
	g_NESmapper.Mapper20.insert_wait = 180;
}

uint8 NES_mapper20_MemoryReadLow(uint32 addr)
{
	uint8 val = 0x00;

	switch (addr)
	{
	case 0x4030:
		{
			val = g_NESmapper.Mapper20.disk_status;
		}
		break;

	case 0x4031:
		{
			if((g_NESmapper.Mapper20.current_side != 0) && (g_NESmapper.Mapper20.current_side == g_NESmapper.Mapper20.last_side))
			{
				val = g_NESmapper.Mapper20.disk[(g_NESmapper.Mapper20.current_side-1)*65500+g_NESmapper.Mapper20.head_position];
				if(g_NESmapper.Mapper20.write_reg & 0x01)
				{
					g_NESmapper.Mapper20.head_position += (g_NESmapper.Mapper20.head_position < 64999) ? 1 : 0;
					g_NESmapper.Mapper20.irq_wait = 2;
				}
				g_NESmapper.Mapper20.access_flag = 1;

			}
			else
			{
				val = 0xff;
			}
		}
		break;

	case 0x4032:
		{
			uint8 eject = ((g_NESmapper.Mapper20.current_side != 0) && (g_NESmapper.Mapper20.current_side == g_NESmapper.Mapper20.last_side)) ? 0 : 1;
			val = 0x40;
			val |= eject ? 1 : 0;
			val |= eject ? 4 : 0;
			val |= (!eject && (g_NESmapper.Mapper20.write_reg & 0x01) && !(g_NESmapper.Mapper20.write_reg & 0x02)) ? 0 : 2;

			if(g_NESmapper.Mapper20.last_side != g_NESmapper.Mapper20.current_side)
			{
				// wait 2.0 sec for change g_NESmapper.Mapper20.disk
				if(g_NESmapper.Mapper20.insert_wait > 120)
				{
					g_NESmapper.Mapper20.last_side = g_NESmapper.Mapper20.current_side;
				}
			}
		}
		break;

	case 0x4033:
		{
			val = 0x80;
		}
		break;

	default:
		{
			val = NES_APU_ExRead(addr);
		}
		break;
	}

	return val;
}

void NES_mapper20_MemoryWriteLow(uint32 addr, uint8 data)
{
	switch (addr)
	{
	case 0x4020:
		{
			g_NESmapper.Mapper20.irq_latch = (g_NESmapper.Mapper20.irq_latch & 0xFF00) | data;
		}
		break;

	case 0x4021:
		{
			g_NESmapper.Mapper20.irq_latch = (g_NESmapper.Mapper20.irq_latch & 0x00FF) | ((uint32)data << 8);
		}
		break;

	case 0x4022:
		{
			g_NESmapper.Mapper20.irq_counter = g_NESmapper.Mapper20.irq_latch;
			g_NESmapper.Mapper20.irq_enabled = data & 0x03;
		}
		break;

	case 0x4023:
		{
			g_NESmapper.Mapper20.disk_enabled = data & 0x01;
		}
		break;

	case 0x4024:
		{
			if((g_NESmapper.Mapper20.current_side != 0) && (g_NESmapper.Mapper20.current_side == g_NESmapper.Mapper20.last_side))
			{
				if(g_NESmapper.Mapper20.disk_enabled && !(g_NESmapper.Mapper20.write_reg & 0x04) && g_NESmapper.Mapper20.head_position < 65000)
				{
					if(g_NESmapper.Mapper20.write_skip)
					{
						g_NESmapper.Mapper20.write_skip--;
					}
					else if(g_NESmapper.Mapper20.head_position >= 2)
					{
						g_NESmapper.Mapper20.disk[(g_NESmapper.Mapper20.current_side-1)*65500+g_NESmapper.Mapper20.head_position-2] = data;
						g_NESmapper.Mapper20.access_flag = 1;
					}
				}
			}
		}
		break;

	case 0x4025:
		{
			if(data & 0x08)
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_HORIZ);
			}
			else
			{
				g_NESmapper.set_mirroring2(NES_PPU_MIRROR_VERT);
			}
			if((g_NESmapper.Mapper20.current_side != 0) && (g_NESmapper.Mapper20.current_side == g_NESmapper.Mapper20.last_side))
			{
				if((g_NESmapper.Mapper20.write_reg & 0x40) && !(data & 0x10) && !(data & 0x40))
				{
					g_NESmapper.Mapper20.head_position = (g_NESmapper.Mapper20.head_position < 2) ? 0 : g_NESmapper.Mapper20.head_position - 2;
				}
				if(!(data & 0x04))
				{
					g_NESmapper.Mapper20.write_skip = 2;
				}
				if(data & 0x02)
				{
					g_NESmapper.Mapper20.head_position = 0;
					g_NESmapper.Mapper20.irq_wait = 2;
				}
				if(data & 0x80)
				{
					g_NESmapper.Mapper20.irq_wait = 2;
				}
			}
			g_NESmapper.Mapper20.write_reg = data;
		}
		break;

	default:
		{
			if(addr >= 0x4040 && addr < 0x4100)
				NES_APU_ExWrite(addr, data);
		}
		break;
	}
}

void NES_mapper20_MemoryWriteSaveRAM(uint32 addr, uint8 data)
{
	g_NESmapper.Mapper20.wram[addr - 0x6000] = data;
}

void NES_mapper20_MemoryWrite(uint32 addr, uint8 data)
{
	if(addr < 0xE000)
	{
		g_NESmapper.Mapper20.wram[addr - 0x6000] = data;
	}
}

void NES_mapper20_HSync(uint32 scanline)
{
	g_NESmapper.Mapper20.disk_status &= 0xfc;

	if(g_NESmapper.Mapper20.irq_enabled)
	{
		if(g_NESmapper.Mapper20.irq_counter < (unsigned int)((g_NESmapper.Mapper20.patch)?112:113)) //113)
		{
			g_NESmapper.Mapper20.irq_enabled &= 0x01;
			g_NESmapper.Mapper20.irq_counter = g_NESmapper.Mapper20.irq_latch;
			g_NESmapper.Mapper20.disk_status |= 0x01;
			NES6502_DoIRQ();
		}
		else
		{
			g_NESmapper.Mapper20.irq_counter -= ((g_NESmapper.Mapper20.patch)?112:113); //113;
		}
	}
	else if(g_NESmapper.Mapper20.irq_wait)
	{
		g_NESmapper.Mapper20.irq_wait--;
		if(!g_NESmapper.Mapper20.irq_wait && (g_NESmapper.Mapper20.write_reg & 0x80))
		{
			g_NESmapper.Mapper20.disk_status |= 0x02;
			NES6502_DoIRQ();
		}
	}
}

void NES_mapper20_VSync()
{
	// count MAX 3sec
	g_NESmapper.Mapper20.insert_wait += (g_NESmapper.Mapper20.insert_wait < 180) ? 1 : 0;
}

void NES_mapper20_SNSS_fixup()
{
	nes6502_context context;
	NES6502_GetContext(&context);
	context.mem_page[3] = g_NESmapper.Mapper20.wram + 0x0000;
	context.mem_page[4] = g_NESmapper.Mapper20.wram + 0x2000;
	context.mem_page[5] = g_NESmapper.Mapper20.wram + 0x4000;
	context.mem_page[6] = g_NESmapper.Mapper20.wram + 0x6000;
	context.mem_page[7] = g_NESmapper.Mapper20.bios;
	NES6502_SetContext(&context);
}

uint8 NES_mapper20_GetDiskSideNum()
{
	return g_NESmapper.ROM_banks[4];
}

uint8 NES_mapper20_GetDiskSide()
{
	return g_NESmapper.Mapper20.current_side;
}

void  NES_mapper20_SetDiskSide(uint8 side)
{
	if(side <= g_NESmapper.ROM_banks[4]) {
		g_NESmapper.Mapper20.current_side = side;
		g_NESmapper.Mapper20.insert_wait = 0;
	}
}

uint8 *NES_mapper20_GetDiskDatap()
{
	return g_NESmapper.Mapper20.disk;
}

uint8 NES_mapper20_DiskAccessed()
{
	uint8 retval = g_NESmapper.Mapper20.access_flag;
	g_NESmapper.Mapper20.access_flag = 0;
	return retval;
}


/////////////////////////////////////////////////////////////////////

#endif
