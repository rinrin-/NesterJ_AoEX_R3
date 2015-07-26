/*
** nester - NES emulator
** Copyright (C) 2000  Darren Ranalli
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.  To obtain a
** copy of the GNU Library General Public License, write to the Free
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef _NES_MAPPER_H_
#define _NES_MAPPER_H_

#include "nes_rom.h"
#include "../debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif

// for Mapper 1
typedef enum
{
	MMC1_SMALL,
	MMC1_512K,
	MMC1_1024K
}MMC1_Size_t;

/////////////////////////////////////////////////////////////////////
// mapper factory
struct _NES_mapper* GetMapper(void);
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Mapper struct
typedef struct _NES_mapper
{
	void  (*Reset)();

	uint8 (*MemoryReadLow)(uint32 addr);
	void  (*WriteHighRegs)(uint32 addr, uint8 data);
	void  (*MemoryWrite)(uint32 addr, uint8 data);
	void  (*MemoryWriteLow)(uint32 addr, uint8 data);
	void  (*MemoryWriteSaveRAM)(uint32 addr, uint8 data);

	void  (*MemoryReadSaveRAM)(uint32 addr);

	void  (*HSync)(uint32 scanline);
	void  (*VSync)();
	void  (*Clock)(int cycles );

	// for mmc2 & mmc5 & Oekakidds
	void  (*PPU_Latch_FDFE)(uint32 addr);
	uint8 (*PPU_Latch_RenderScreen)(uint8 mode, uint32 addr);
	void  (*PPU_Latch_Address)(uint32 addr);

	// for disk system (#20)
	uint8 (*GetDiskSideNum)();
	uint8 (*GetDiskSide)();
	void  (*SetDiskSide)(uint8 side);
//	uint8 GetDiskData(uint32 pt)			 {return 0;}
	uint8 *(*GetDiskDatap)();
//	void  SetDiskData(uint32 pt, uint8 data) {}
	uint8 (*DiskAccessed)();

	// for Datach Barcode Battler
	void (*SetBarcodeValue)(uint32 value_low, uint32 value_high);

	uint32 num_16k_ROM_banks;
	uint32 num_8k_ROM_banks;
	uint32 num_1k_VROM_banks;

	uint8* ROM_banks;
	uint8* VROM_banks;

	// for ROM & VROM over 256KB
	uint32 ROM_mask;
	uint32 VROM_mask;

	void (*set_CPU_banks4)(uint32 bank4_num, uint32 bank5_num,
	                   uint32 bank6_num, uint32 bank7_num);
	void (*set_CPU_bank )(int page,uint32 bank_num);
	void (*set_CPU_bank4)(uint32 bank_num);
	void (*set_CPU_bank5)(uint32 bank_num);
	void (*set_CPU_bank6)(uint32 bank_num);
	void (*set_CPU_bank7)(uint32 bank_num);

	// for UNIF
	void (*set_CPU_bank_unif)(uint32, uint8);
	void (*set_VRAM_bank_unif)(uint32, uint8);

	// for mapper 40
	void (*set_CPU_banks5)(uint32 bank3_num,
	                   uint32 bank4_num, uint32 bank5_num,
	                   uint32 bank6_num, uint32 bank7_num);
	void (*set_CPU_bank3)(uint32 bank_num);

	void (*set_PPU_banks8)(uint32 bank0_num, uint32 bank1_num,
	                   uint32 bank2_num, uint32 bank3_num,
	                   uint32 bank4_num, uint32 bank5_num,
	                   uint32 bank6_num, uint32 bank7_num);
	void (*set_PPU_bank )(int page, uint32 bank_num);
	void (*set_PPU_bank0)(uint32 bank_num);
	void (*set_PPU_bank1)(uint32 bank_num);
	void (*set_PPU_bank2)(uint32 bank_num);
	void (*set_PPU_bank3)(uint32 bank_num);
	void (*set_PPU_bank4)(uint32 bank_num);
	void (*set_PPU_bank5)(uint32 bank_num);
	void (*set_PPU_bank6)(uint32 bank_num);
	void (*set_PPU_bank7)(uint32 bank_num);

	// for mapper 19,68,90
	void (*set_PPU_bank8)(uint32 bank_num);
	void (*set_PPU_bank9)(uint32 bank_num);
	void (*set_PPU_bank10)(uint32 bank_num);
	void (*set_PPU_bank11)(uint32 bank_num);

	// for mapper 1,4,5,6,13,19,80,85,96,119
	void (*set_VRAM_bank)(uint8 bank, uint32 bank_num);

	//ÃÌº”
	void	(*SetPROM_Bank)( uint8 page, uint8 *ptr);
	void	(*SetPROM_8K_Bank)( uint8 page, int bank );
	void	(*SetPROM_16K_Bank)( uint8 page, int bank );
	void	(*SetPROM_32K_Bank)( int bank );
	void	(*SetPROM_32K_Banks)( int bank0, int bank1, int bank2, int bank3 );

	//ÃÌº”
	// PPU VROM bank
	void	(*SetVROM_Bank)( uint8 page, uint8* ptr);
	void	(*SetVROM_1K_Bank)( uint8 page, int bank );
	void	(*SetVROM_1K_BankR)(int start, uint8 page, int bank );
	void	(*SetVROM_2K_Bank)( uint8 page, int bank );
	void	(*SetVROM_4K_Bank)( uint8 page, int bank );
	void	(*SetVROM_8K_Bank)( int bank );
	void	(*SetVROM_8K_Banks)( int bank0, int bank1, int bank2, int bank3,int bank4, int bank5, int bank6, int bank7 );

	//ÃÌº”
	void	(*SetCRAM_1K_Bank)( uint8 page, int bank );
	void	(*SetCRAM_2K_Bank)( uint8 page, int bank );
	void	(*SetCRAM_4K_Bank)( uint8 page, int bank );
	void	(*SetCRAM_8K_Bank)( int bank );

	void (*set_mirroring)(uint32 nt0, uint32 nt1, uint32 nt2, uint32 nt3);
	void (*set_mirroring2)(mirroring_type m);

	void (*set_genie)();
	void (*set_cheat)();

#include "mapper/mapper_mem.h"	// äeÉ}ÉbÉpÅ[ÇÃÉÅÉÇÉä(union - structs)

}NES_mapper;

extern NES_mapper g_NESmapper; // é¿ë‘ÇÕ NES_mapper.c

/////////////////////////////////////////////////////////////////////

#include "mapper/000.h"
#include "mapper/v001.h"
#include "mapper/001.h"
#include "mapper/002.h"
#include "mapper/003.h"
#include "mapper/004.h"
#include "mapper/005.h"
#include "mapper/006.h"
#include "mapper/007.h"
#include "mapper/008.h"
#include "mapper/009.h"
#include "mapper/010.h"
#include "mapper/011.h"
#include "mapper/012.h"
#include "mapper/013.h"
//#include "mapper/014.h"
#include "mapper/015.h"
#include "mapper/016.h"
#include "mapper/017.h"
#include "mapper/018.h"
#include "mapper/019.h"
#include "mapper/020.h"
#include "mapper/021.h"
#include "mapper/022.h"
#include "mapper/023.h"
#include "mapper/024.h"
#include "mapper/025.h"
#include "mapper/026.h"
#include "mapper/027.h"
//#include "mapper/028.h"
//#include "mapper/029.h"
//#include "mapper/030.h"
//#include "mapper/031.h"
#include "mapper/032.h"
#include "mapper/033.h"
#include "mapper/034.h"
//#include "mapper/035.h"
//#include "mapper/036.h"
//#include "mapper/037.h"
//#include "mapper/038.h"
//#include "mapper/039.h"
#include "mapper/040.h"
#include "mapper/041.h"
#include "mapper/042.h"
#include "mapper/043.h"
#include "mapper/044.h"
#include "mapper/045.h"
#include "mapper/046.h"
#include "mapper/047.h"
#include "mapper/048.h"
#include "mapper/049.h"
#include "mapper/050.h"
#include "mapper/051.h"
#include "mapper/052.h"
//#include "mapper/053.h"
#include "mapper/054.h"
//#include "mapper/055.h"
//#include "mapper/056.h"
#include "mapper/057.h"
#include "mapper/058.h"
//#include "mapper/059.h"
#include "mapper/060.h"
#include "mapper/061.h"
#include "mapper/062.h"
//#include "mapper/063.h"
#include "mapper/064.h"
#include "mapper/065.h"
#include "mapper/066.h"
#include "mapper/067.h"
#include "mapper/068.h"
#include "mapper/069.h"
#include "mapper/070.h"
#include "mapper/071.h"
#include "mapper/072.h"
#include "mapper/073.h"
#include "mapper/074.h"
#include "mapper/075.h"
#include "mapper/076.h"
#include "mapper/077.h"
#include "mapper/078.h"
#include "mapper/079.h"
#include "mapper/080.h"
//#include "mapper/081.h"
#include "mapper/082.h"
#include "mapper/083.h"
//#include "mapper/084.h"
#include "mapper/085.h"
#include "mapper/086.h"
#include "mapper/087.h"
#include "mapper/088.h"
#include "mapper/089.h"
#include "mapper/090.h"
#include "mapper/091.h"
#include "mapper/092.h"
#include "mapper/093.h"
#include "mapper/094.h"
#include "mapper/095.h"
#include "mapper/096.h"
#include "mapper/097.h"
//#include "mapper/098.h"
#include "mapper/099.h"
#include "mapper/100.h"
#include "mapper/101.h"
//#include "mapper/102.h"
//#include "mapper/103.h"
#include "mapper/104.h"
#include "mapper/105.h"
//#include "mapper/106.h"
//#include "mapper/107.h"
//#include "mapper/108.h"
//#include "mapper/109.h"
//#include "mapper/110.h"
//#include "mapper/111.h"
#include "mapper/112.h"
#include "mapper/113.h"
#include "mapper/114.h"
#include "mapper/115.h"
#include "mapper/116.h"
#include "mapper/117.h"
#include "mapper/118.h"
#include "mapper/119.h"
//#include "mapper/120.h"
#include "mapper/121.h"
#include "mapper/122.h"
//#include "mapper/123.h"
//#include "mapper/124.h"
//#include "mapper/125.h"
//#include "mapper/126.h"
//#include "mapper/127.h"
//#include "mapper/128.h"
//#include "mapper/129.h"
//#include "mapper/130.h"
//#include "mapper/131.h"
#include "mapper/132.h"
#include "mapper/133.h"
#include "mapper/134.h"
//#include "mapper/135.h"
//#include "mapper/136.h"
//#include "mapper/137.h"
//#include "mapper/138.h"
//#include "mapper/139.h"
#include "mapper/140.h"
//#include "mapper/141.h"
#include "mapper/142.h"
//#include "mapper/143.h"
//#include "mapper/144.h"
//#include "mapper/145.h"
//#include "mapper/146.h"
//#include "mapper/147.h"
//#include "mapper/148.h"
#include "mapper/149.h"
//#include "mapper/150.h"
#include "mapper/151.h"
//#include "mapper/152.h"
//#include "mapper/153.h"
//#include "mapper/154.h"
//#include "mapper/155.h"
//#include "mapper/156.h"
//#include "mapper/157.h"
//#include "mapper/158.h"
//#include "mapper/159.h"
#include "mapper/160.h"
//#include "mapper/161.h"
#include "mapper/162.h"
#include "mapper/163.h"
#include "mapper/164.h"
#include "mapper/165.h"
//#include "mapper/166.h"
#include "mapper/167.h"
//#include "mapper/168.h"
//#include "mapper/169.h"
//#include "mapper/170.h"
//#include "mapper/171.h"
//#include "mapper/172.h"
//#include "mapper/173.h"
//#include "mapper/174.h"
#include "mapper/175.h"
#include "mapper/176.h"
#include "mapper/177.h"
#include "mapper/178.h"
//#include "mapper/179.h"
#include "mapper/180.h"
#include "mapper/181.h"
#include "mapper/182.h"
#include "mapper/183.h"
//#include "mapper/184.h"
#include "mapper/185.h"
//#include "mapper/186.h"
#include "mapper/187.h"
#include "mapper/188.h"
#include "mapper/189.h"
//#include "mapper/190.h"
//#include "mapper/191.h"
//#include "mapper/192.h"
//#include "mapper/193.h"
//#include "mapper/194.h"
//#include "mapper/195.h"
//#include "mapper/196.h"
//#include "mapper/197.h"
//#include "mapper/198.h"
//#include "mapper/199.h"
#include "mapper/200.h"
#include "mapper/201.h"
#include "mapper/202.h"
#include "mapper/203.h"
#include "mapper/204.h"
#include "mapper/205.h"
//#include "mapper/206.h"
//#include "mapper/207.h"
//#include "mapper/208.h"
//#include "mapper/209.h"
//#include "mapper/210.h"
//#include "mapper/211.h"
#include "mapper/212.h"
#include "mapper/213.h"
#include "mapper/214.h"
#include "mapper/215.h"
#include "mapper/216.h"
#include "mapper/217.h"
//#include "mapper/218.h"
//#include "mapper/219.h"
//#include "mapper/220.h"
//#include "mapper/221.h"
#include "mapper/222.h"
//#include "mapper/223.h"
//#include "mapper/224.h"
#include "mapper/225.h"
#include "mapper/226.h"
#include "mapper/227.h"
#include "mapper/228.h"
#include "mapper/229.h"
#include "mapper/230.h"
#include "mapper/231.h"
#include "mapper/232.h"
#include "mapper/233.h"
#include "mapper/234.h"
#include "mapper/235.h"
#include "mapper/236.h"
#include "mapper/237.h"
//#include "mapper/238.h"
//#include "mapper/239.h"
#include "mapper/240.h"
#include "mapper/241.h"
#include "mapper/242.h"
#include "mapper/243.h"
#include "mapper/244.h"
#include "mapper/245.h"
#include "mapper/246.h"
//#include "mapper/247.h"
#include "mapper/248.h"
#include "mapper/249.h"
#include "mapper/250.h"
#include "mapper/251.h"
#include "mapper/252.h"
#include "mapper/253.h"
#include "mapper/254.h"
#include "mapper/255.h"
#include "mapper/NSF.h"

#include "mapper/Sachen74x374.h"
#include "mapper/mmc3.h"
#include "mapper/waixing.h"
#include "mapper/Kaiser.h"
#include "mapper/BmcT262.h"
#include "mapper/Unlcc21.h"
#include "mapper/Unl6035052.h"
#include "mapper/UnlSHeroes.h"
#include "mapper/BmcSuper24in1.h"
#include "mapper/BmcCtc65.h"
#include "mapper/BmcHero.h"
#include "mapper/BmcSuperVision16in1.h"
#include "mapper/kof97.h"
#include "mapper/BmcA65as.h"
#include "mapper/fk23c.h"
#include "mapper/BenshengBs5.h"
#include "mapper/BmcSuper40in1.h"
#include "mapper/BmcY2k64in1.h"
#include "mapper/UnlN625092.h"
#include "mapper/BmcSuper22Games.h"
#include "mapper/Rcm.h"
#include "mapper/BmcGolden190in1.h"
#include "mapper/Bmc800in1.h"
#include "mapper/GeniusMerioBros.h"
#include "mapper/Unl.h"

#ifdef __cplusplus
}
#endif


/////////////////////////////////////////////////////////////////////

#endif
