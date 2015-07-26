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

#ifndef _PIXMAP_H_
#define _PIXMAP_H_

#include "types.h"

#define PIXEL uint8

typedef struct pixmap_s
{
  PIXEL* data;
//  uint32 width;
//  uint32 height;
//  uint32 pitch;

//  uint32 bitdepth;	//screen bitdepth
//  uint32 Gbitmask;

} pixmap;

#endif
