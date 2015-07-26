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

#include "nes_config.h"
#include "nes_string.h"

NES_CONFIG g_NESConfig; // NES ÇÃê›íË

void NES_Config_SetDefaults_All(void)
{
	NES_Config_SetPreferencesDefaults();
	NES_Config_SetGraphicsDefaults();
	NES_Config_SetSoundDefaults();
}

void NES_Config_SetPreferencesDefaults(void)
{
	// preferences
	g_NESConfig.preferences.TV_Mode = 0;
	g_NESConfig.preferences.UseFDSDiskASkip = TRUE;
	g_NESConfig.preferences.UseRomDataBase = TRUE;
}

void NES_Config_SetGraphicsDefaults(void)
{
	// graphics
	g_NESConfig.graphics.black_and_white = FALSE;
	g_NESConfig.graphics.show_all_scanlines = FALSE;
	g_NESConfig.graphics.DisableSpriteClipping = FALSE;
	g_NESConfig.graphics.DisableBackGClipping = FALSE;
	g_NESConfig.graphics.DefaultPaletteNo = 0;
	g_NESConfig.graphics.show_more_than_8_sprites = FALSE;
}

void NES_Config_SetSoundDefaults(void)
{
	// sound
	g_NESConfig.sound.enabled = TRUE;
	g_NESConfig.sound.rectangle1_enabled = TRUE;
	g_NESConfig.sound.rectangle2_enabled = TRUE;
	g_NESConfig.sound.triangle_enabled = TRUE;
	g_NESConfig.sound.noise_enabled = TRUE;
	g_NESConfig.sound.dpcm_enabled = TRUE;
	g_NESConfig.sound.ext_enabled = TRUE;

	g_NESConfig.sound.filter_type = FILTER_LOWPASS;
	g_NESConfig.sound.lowpass_filter_f = 15000;
;

	g_NESConfig.sound.ideal_triangle_enabled = FALSE;
	g_NESConfig.sound.smooth_envelope_enabled = FALSE; // reserved
	g_NESConfig.sound.smooth_sweep_enabled = FALSE; // reserved
}
