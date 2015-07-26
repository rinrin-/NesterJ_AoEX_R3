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

#ifndef _NES_APU_WRAPPER_H_
#define _NES_APU_WRAPPER_H_

//#include <stdio.h>
#include "../types.h"
//#include "sound_mgr.h"
#include "nes_apu.h"
//#include "vrcvisnd.h"
#include "fdssnd.h"
//#include "vrc7.h"
#include "../../soundmanager.h"

//#include "libsnss.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NES_APU
{
  uint8 regs[0x16];
  int CurrentPlayingBank;
} NES_APU;

extern NES_APU g_APU; // nes_apu_wrapper.c

void NES_APU_reset();
void NES_APU_snd_mgr_changed();
static inline void NES_APU_setchan(int chan, boolean enabled) {apu_setchan(chan, enabled);}

uint8 NES_APU_Read(uint32 addr);
void  NES_APU_Write(uint32 addr, uint8 data);

uint8 NES_APU_ExRead(uint32 addr);
void  NES_APU_ExWrite(uint32 addr, uint8 data);
void  NES_APU_SelectExSound(uint8 data);

void NES_APU_SyncAPURegister();
boolean NES_APU_SyncDMCRegister(uint32 cpu_cycles);

void NES_APU_DoFrame();

void NES_APU_freeze();
void NES_APU_thaw();


void NES_APU_Init();
void NES_APU_ShutDown();
void NES_APU_AssertParams();

// this function should be called by a state loading function
// with an array of the apu registers
// reg 0x14 is not used
void NES_APU_load_regs(const uint8 new_regs[0x16]);

// this function should be called by a state saving function
// to fill an array of the apu registers
// reg 0x14 is not used
void NES_APU_get_regs(uint8 reg_array[0x16]);

#ifdef __cplusplus
}
#endif


#endif
