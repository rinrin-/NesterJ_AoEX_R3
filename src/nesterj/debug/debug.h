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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../nes/nes_string.h"


//#define NESTER_DEBUG

#ifdef NESTER_DEBUG

void OutputDebugString(void *buf, int nLen);

// debug for PSP
#define DEBUG(msg) \
{\
	char szDebugMsg[1024];\
	_strcpy(szDebugMsg, msg);\
	_strcat(szDebugMsg, "\n");\
	OutputDebugString(szDebugMsg, _strlen(szDebugMsg));\
}

#define DEBUGVALUE(msg, val) \
{\
	char szDebugMsg[1024], szTemp[32];\
	_strcpy(szDebugMsg, msg);\
	_strcat(szDebugMsg, " = ");\
	_itoa(val, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_strcat(szDebugMsg, "\n");\
	OutputDebugString(szDebugMsg, _strlen(szDebugMsg));\
}

#define DEBUGVALUE2(msg, val1, val2) \
{\
	char szDebugMsg[1024], szTemp[32];\
	_strcpy(szDebugMsg, msg);\
	_strcat(szDebugMsg, " = ");\
	_itoa(val1, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_itoa(val2, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_strcat(szDebugMsg, "\n");\
	OutputDebugString(szDebugMsg, _strlen(szDebugMsg));\
}

#define DEBUGVALUE3(msg, val1, val2, val3) \
{\
	char szDebugMsg[1024], szTemp[32];\
	_strcpy(szDebugMsg, msg);\
	_strcat(szDebugMsg, " = ");\
	_itoa(val1, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_itoa(val2, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_itoa(val3, szTemp); \
	_strcat(szDebugMsg, szTemp);\
	_strcat(szDebugMsg, "\n");\
	OutputDebugString(szDebugMsg, _strlen(szDebugMsg));\
}

#define ADDRCHECK(func, address)\
{\
	if (address & 0xFFFF0000) {\
		DEBUG(func " addr ERROR!");\
	}\
}

#define ASSERT(EXPR) if (!(EXPR)) DEBUG(#EXPR "ASSERT!")\


#define SHOW_ERRMSG(bgcolor,msg,color,vcnt) \
	{int nCount=vcnt; \
	while (nCount--) { \
		pgFillvram(bgcolor); \
		mh_print(0,0,msg,color); \
		pgScreenFlipV(); \
	} \
}


#else

 #define DEBUG(msg)
 #define DEBUGVALUE(msg,val)
 #define DEBUGVALUE2(msg,val)
 #define DEBUGVALUE3(msg,val)
 #define ADDRCHECK(func, address)
 #define ASSERT(EXPR)

#endif

 #define LOG(MSG)

 #define IFDEBUG(X)

#endif
