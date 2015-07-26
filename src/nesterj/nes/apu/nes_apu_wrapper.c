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

//#include <string.h>
//#include <stdlib.h>
#include "nes_apu_wrapper.h"
//#include "NES.h"
#include "../../debug/debug.h"
//#include "settings.h"
#include "../nes_config.h"
#include "../nes_string.h"


#include "nes_vrc7.h"	// vrc7 (nes_vrc7.cpp)	<uonester>
#include "nes_vrc6.h"	// vrc6 (nes_vrc6.cpp)	<uonester>
#include "nes_mmc5.h"	// mmc5 (nes_mmc5.cpp)	<uonester>
#include "nes_n106.h"	// n106 (nes_n106.cpp)	<uonester>
#include "nes_fme7.h"	// fme7 (nes_fme7.cpp)	<uonester>
#include "fdssnd.h"		// FDS  (fdssnd.c)		<uonester>

//#include "vrc7.h"		// vrc7 (vrc7.c)		<emu2413>
//#include "vrcvisnd.h"	// vrc6 (vrcvisnd.c)	<nester public bata4>

NES_APU g_APU;
extern apu_t g_apu_t;

#define SAMPLE_BITS 16

void NES_APU_Init()
{
  int sample_rate;
  int sample_bits;

  g_APU.CurrentPlayingBank = 0;

  NES_APU_ShutDown();

  sample_rate = Snd_GetSampleRate();
  sample_bits = SAMPLE_BITS;
  if (!sample_bits || !sample_rate) {
		sample_rate = 44100;
		sample_bits = 16;
  }
  if(!apu_create(sample_rate, 60, 0, sample_bits)) {
	  LOG("Error creating NES APU");
	  return;
  }

  NES_APU_AssertParams();
  NES_APU_reset();
}

void NES_APU_ShutDown()
{
    apu_destroy();
}


void NES_APU_AssertParams()
{
	apu_setchan(0, g_NESConfig.sound.rectangle1_enabled);
	apu_setchan(1, g_NESConfig.sound.rectangle2_enabled);
	apu_setchan(2, g_NESConfig.sound.triangle_enabled);
	apu_setchan(3, g_NESConfig.sound.noise_enabled);
	apu_setchan(4, g_NESConfig.sound.dpcm_enabled);
	apu_setchan(5, g_NESConfig.sound.ext_enabled);

    switch(g_NESConfig.sound.filter_type)
    {
      case FILTER_NONE:
        apu_setfilter(APU_FILTER_NONE);
        break;
      case FILTER_LOWPASS:
        apu_setfilter(APU_FILTER_LOWPASS);
        break;
      case FILTER_LOWPASS_WEIGHTED:
        apu_setfilter(APU_FILTER_WEIGHTED);
        break;
      case FILTER_OLOWPASS:
        apu_setfilter(APU_FILTER_OLOWPASS);
        break;
    }
#ifdef APU_YANO
    apu_setmode( APUMODE_IDEAL_TRIANGLE,
       g_NESConfig.sound.ideal_triangle_enabled);
    apu_setmode( APUMODE_SMOOTH_ENVELOPE,
       g_NESConfig.sound.smooth_envelope_enabled);
    apu_setmode( APUMODE_SMOOTH_SWEEP,
       g_NESConfig.sound.smooth_sweep_enabled);
#endif
	apu_setlowpass_filter_f(g_NESConfig.sound.lowpass_filter_f);
}

void NES_APU_reset()
{
    apu_reset();
  _memset(g_APU.regs, 0x00, sizeof(g_APU.regs));
}

void NES_APU_snd_mgr_changed()
{
  int sample_rate;
  int sample_bits;

	sample_rate = Snd_GetSampleRate();
	sample_bits = SAMPLE_BITS;

    apu_setparams(sample_rate, 60, 0, sample_bits);
//SelectExSound(apu->ex_chip);
  int i;
  for(i=0; i<NESAPU_EXCHIP_MAX; i++){
	  if(g_apu_t.ext[i]){
//			  if(g_apu_t.ext[i]->init)
//				  g_apu_t.ext[i]->init();
		  if(g_apu_t.ext[i]->reset)
			  g_apu_t.ext[i]->reset();
	  }
  }
    NES_APU_AssertParams();
}

uint8 NES_APU_Read(uint32 addr)
{
    return apu_read(addr);
}

void NES_APU_Write(uint32 addr, uint8 data)
{
	g_APU.regs[addr - 0x4000] = data;
	apu_write(addr, data);
	apu_write_cur(addr, data);
}

uint8 NES_APU_ExRead(uint32 addr)
{
  return ex_read(addr);
}

void NES_APU_ExWrite(uint32 addr, uint8 data)
{
  ex_write(addr, data);
}

void NES_APU_SelectExSound(uint8 data)
{
//  apu_reset();
  g_apu_t.ex_chip = data;

  if(data & 1){		// VRC6
/* PSPでは、uonester版を固定で使う
	  if(g_NESConfig.sound.vrc6_type == 0)
		  g_apu_t.ext[0] = &vrc6_ext;  // (nes_vrc6.cpp)	<uonester>
	  else
		  g_apu_t.ext[0] = &vrcvi_ext;	// (vrcvisnd.c)	<nester public bata4>
	  if(g_apu_t.ext[0]->init)
		  g_apu_t.ext[0]->init();
*/
	  g_apu_t.ext[0] = &vrc6_ext;  // (nes_vrc6.cpp)	<uonester>
  }
  if(data & 2){		// VRC7
/* PSPでは、uonester版を固定で使う
	  if(g_NESConfig.sound.vrc7_type == 0)
		  g_apu_t.ext[1] = &vrc7_ext;	// (nes_vrc7.cpp)	<uonester>
	  else
		  g_apu_t.ext[1] = &emu2413_ext;
	  if(g_apu_t.ext[1]->init)
		  g_apu_t.ext[1]->init();
*/
	  g_apu_t.ext[1] = &vrc7_ext;	// (nes_vrc7.cpp)	<uonester>
  }
  if(data & 4){		// FDC
//	  if(fds_dll_ext.init()){
//		  g_apu_t.ext[4] = &fds_dll_ext;
//	  }
//	  else{
		  g_apu_t.ext[4] = &fds_nesp_ext;
		  if(g_apu_t.ext[4]->init)
			  g_apu_t.ext[4]->init();
//	  }
  }
  if(data & 8){		// MMC5
	  g_apu_t.ext[5] = &mmc5_ext;
	  if(g_apu_t.ext[5]->init)
		  g_apu_t.ext[5]->init();
  }
  if(data & 16){	// N106
	  g_apu_t.ext[6] = &n106_ext;
	  if(g_apu_t.ext[6]->init)
		  g_apu_t.ext[6]->init();
  }
  if(data & 32){	// FME7
	  g_apu_t.ext[7] = &fme7_ext;
	  if(g_apu_t.ext[7]->init)
		  g_apu_t.ext[7]->init();
  }
  if(data & 64){	// J106 (reserved)
  }
#if 1
  {
	  int i;
	  for(i=0; i<NESAPU_EXCHIP_MAX; i++){
		  if(g_apu_t.ext[i]){
//			  if(g_apu_t.ext[i]->init)
//				  g_apu_t.ext[i]->init();
			  if(g_apu_t.ext[i]->reset)
				  g_apu_t.ext[i]->reset();
		  }
	  }
  }
#endif
//  reset();
}

void NES_APU_DoFrame()
{
  int cur_ph;  // cur playing half
  int cur_nph; // cur not-playing half

  int16* buf;
  uint32 buf_len;

	cur_ph = Snd_GetCurrentPlayingBank();

	if(cur_ph == g_APU.CurrentPlayingBank) return;

	cur_nph = g_APU.CurrentPlayingBank;
	g_APU.CurrentPlayingBank = cur_ph;

	if(!Snd_Lock(cur_nph, &buf, &buf_len)) {
		LOG("couldn't lock sound buffer" << endl);
		return;
	}

	apu_process(buf, buf_len/(SAMPLE_BITS/8));
	Snd_UnLock();
}

void NES_APU_freeze()
{
  Snd_ClearBuffer();
}

void NES_APU_thaw()
{
  g_APU.CurrentPlayingBank = Snd_GetCurrentPlayingBank();
}

void NES_APU_load_regs(const uint8 new_regs[0x16])
{
  int i;

  for(i = 0; i < 0x16; i++)
  {
    // reg 0x14 not used
    if(i == 0x14) continue;

    // write the DMC regs directly
    if((i >= 0x10) && (i <= 0x13))
    {
      g_apu_t.apus.dmc.regs[i - 0x10] = new_regs[i];
    }
    else
    {
      apu_write(0x4000 + i, new_regs[i]);
      apu_write_cur(0x4000 + i, new_regs[i]);
    }
  }
}

void NES_APU_get_regs(uint8 reg_array[0x16])
{
 // copy last written values
  _memcpy(reg_array, g_APU.regs, 0x16);

    // copy in the per-channel stored values
    g_APU.regs[APU_WRA0-0x4000] = g_apu_t.apus.rectangle[0].regs[0];
    g_APU.regs[APU_WRA1-0x4000] = g_apu_t.apus.rectangle[0].regs[1];
    g_APU.regs[APU_WRA2-0x4000] = g_apu_t.apus.rectangle[0].regs[2];
    g_APU.regs[APU_WRA3-0x4000] = g_apu_t.apus.rectangle[0].regs[3];
    g_APU.regs[APU_WRB0-0x4000] = g_apu_t.apus.rectangle[1].regs[0];
    g_APU.regs[APU_WRB1-0x4000] = g_apu_t.apus.rectangle[1].regs[1];
    g_APU.regs[APU_WRB2-0x4000] = g_apu_t.apus.rectangle[1].regs[2];
    g_APU.regs[APU_WRB3-0x4000] = g_apu_t.apus.rectangle[1].regs[3];
    g_APU.regs[APU_WRC0-0x4000] = g_apu_t.apus.triangle.regs[0];
    g_APU.regs[APU_WRC2-0x4000] = g_apu_t.apus.triangle.regs[1];
    g_APU.regs[APU_WRC3-0x4000] = g_apu_t.apus.triangle.regs[2];
    g_APU.regs[APU_WRD0-0x4000] = g_apu_t.apus.noise.regs[0];
    g_APU.regs[APU_WRD2-0x4000] = g_apu_t.apus.noise.regs[1];
    g_APU.regs[APU_WRD3-0x4000] = g_apu_t.apus.noise.regs[2];
    g_APU.regs[APU_WRE0-0x4000] = g_apu_t.apus.dmc.regs[0];
    g_APU.regs[APU_WRE1-0x4000] = g_apu_t.apus.dmc.regs[1];
    g_APU.regs[APU_WRE2-0x4000] = g_apu_t.apus.dmc.regs[2];
    g_APU.regs[APU_WRE3-0x4000] = g_apu_t.apus.dmc.regs[3];
}

void NES_APU_SyncAPURegister()
{
    sync_apu_register();
}

boolean NES_APU_SyncDMCRegister(uint32 cpu_cycles)
{
    return sync_dmc_register(cpu_cycles);
}

