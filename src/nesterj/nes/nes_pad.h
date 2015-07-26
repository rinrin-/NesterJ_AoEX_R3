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

#ifndef NES_PAD_H_
#define NES_PAD_H_


// these bitmasks correspond to the order
// of the internal NES pad-polling scheme
enum NES_BUTTON
{
    NES_A      = 0x01,
    NES_B      = 0x02,
    NES_SELECT = 0x04,	// PLAYER2 MIC
    NES_START  = 0x08,
    NES_UP     = 0x10,
    NES_DOWN   = 0x20,
    NES_LEFT   = 0x40,
    NES_RIGHT  = 0x80
};

#endif
