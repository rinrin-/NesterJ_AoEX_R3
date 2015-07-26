/**************************************************************************/
/*
      libsnss.h

      (C) 2000 The SNSS Group
      See README.TXT file for license and terms of use.

      $Id: libsnss.h,v 1.6 2000/07/09 15:37:21 matt Exp $
*/
/**************************************************************************/

#ifndef _LIBSNSS_H_
#define _LIBSNSS_H_

#include "../../fileio.h"

/**************************************************************************/
/* endian customization */
/**************************************************************************/
/*
   Endian-ness quick reference:
   the number is:
      $12345678
   the little-endian representation (e.g.: 6502, Intel x86) is:
      78 56 34 12
   the big-endian representation (e.g.: Motorola 68000) is:
      12 34 56 78
   the SNSS file format uses big-endian representation
*/

/* comment/uncomment depending on your processor architecture */
/* commenting this out implies BIG ENDIAN */
#define USE_LITTLE_ENDIAN

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**************************************************************************/
/* SNSS constants */
/**************************************************************************/

typedef enum _SNSS_OPEN_MODES
{
   SNSS_OPEN_READ,
   SNSS_OPEN_WRITE
} SNSS_OPEN_MODE;

/* block types */
typedef enum _SNSS_BLOCK_TYPES
{
   SNSS_BASR,
   SNSS_VRAM,
   SNSS_SRAM,
   SNSS_MPRD,
   SNSS_CNTR,
   SNSS_SOUN,
   SNSS_UNKNOWN_BLOCK,
} SNSS_BLOCK_TYPE;

/* function return types */
typedef enum _SNSS_RETURN_CODES
{
   SNSS_OK,
   SNSS_BAD_FILE_TAG,
   SNSS_OPEN_FAILED,
   SNSS_CLOSE_FAILED,
   SNSS_READ_FAILED,
   SNSS_WRITE_FAILED,
   SNSS_OUT_OF_MEMORY,
   SNSS_UNSUPPORTED_BLOCK,
} SNSS_RETURN_CODE;


#define TAG_LENGTH 4
#define SNSS_BLOCK_VERSION 1

/**************************************************************************/
/* SNSS data structures */
/**************************************************************************/

struct mapper1Data
{
   unsigned char registers[4];
   unsigned char latch;
   unsigned char numberOfBits;
};

struct mapper4Data
{
   unsigned char p[22];
};

struct mapper5Data
{
   unsigned char irqLineLowByte;
   unsigned char irqLineHighByte;
   unsigned char irqEnabled;
   unsigned char irqStatus;
   unsigned char wramWriteProtect0;
   unsigned char wramWriteProtect1;
   unsigned char romBankSize;
   unsigned char vromBankSize;
   unsigned char gfxMode;
   unsigned char splitControl;
   unsigned char splitBank;
   unsigned char last5205Write;
   unsigned char last5206Write;
   unsigned char wramBank3;
   unsigned char wramBank4;
   unsigned char wramBank5;
   unsigned char wramBank6;
   unsigned char vromBank[8][2];
};

struct mapper6Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
};

struct mapper9Data
{
   unsigned char latch[2];
   unsigned char lastB000Write;
   unsigned char lastC000Write;
   unsigned char lastD000Write;
   unsigned char lastE000Write;
};

struct mapper10Data
{
   unsigned char latch[2];
   unsigned char lastB000Write;
   unsigned char lastC000Write;
   unsigned char lastD000Write;
   unsigned char lastE000Write;
};

struct mapper13Data
{
   unsigned char wramBank;
};

struct mapper16Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounterLowByte;
   unsigned char irqLatchCounterHighByte;
};

struct mapper17Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
};

struct mapper18Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
};

struct mapper19Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char lastE800Write;
   unsigned char lastF800Write;
};

struct mapper20Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounterLowByte;
   unsigned char irqLatchCounterHighByte;
   unsigned char irqWaitCounter;
   unsigned char last4023Write;
   unsigned char last4025Write;
   unsigned char HeadPositionLowByte;
   unsigned char HeadPositionHighByte;
   unsigned char DiskStatus;
   unsigned char WriteSkip;
   unsigned char CurrentDiskSide;
   unsigned char LastDiskSide;
   unsigned char DiskInsertWait;
};

struct mapper21Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
   unsigned char last9002Write;
};

struct mapper23Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
   unsigned char last9008Write;
};

struct mapper24Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
};

struct mapper25Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
   unsigned char last9001Write;
};

struct mapper26Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
};

struct mapper32Data
{
   unsigned char last9000Write;
};

struct mapper33Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
};

struct mapper40Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
};

struct mapper41Data
{
   unsigned char last6000Write;
};

struct mapper42Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
};

struct mapper43Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
};

struct mapper46Data
{
   unsigned char last6000Write;
   unsigned char last8000Write;
};

struct mapper48Data
{
   unsigned char lastE000Write;
};

struct mapper49Data
{
   unsigned char irqCounter;
   unsigned char irqLatchCounter;
   unsigned char irqCounterEnabled;
   unsigned char last8000Write;
   unsigned char last6000Write;
   unsigned char lastA001Write;
};

struct mapper50Data
{
   unsigned char irqCounterEnabled;
};

struct mapper51Data
{
   unsigned char BankSelect;
   unsigned char MapperMode;
};

struct mapper57Data
{
   unsigned char last8800Write;
};

struct mapper64Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
   unsigned char last8000Write;
};

struct mapper65Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounterLowByte;
   unsigned char irqLatchCounterHighByte;
};

struct mapper67Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
};

struct mapper68Data
{
   unsigned char lastC000Write;
   unsigned char lastD000Write;
   unsigned char lastE000Write;
};

struct mapper69Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char last8000Write;
};

struct mapper73Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
};

struct mapper74Data
{
   unsigned char p[20];
};

struct mapper75Data
{
   unsigned char lastE000Write;
   unsigned char lastF000Write;
};

struct mapper76Data
{
   unsigned char last8000Write;
};

struct mapper82Data
{
   unsigned char last7EF6Write;
};

struct mapper83Data
{
   unsigned char irqCounterLowByte;
   unsigned char irqCounterHighByte;
   unsigned char irqCounterEnabled;
   unsigned char last8000Write;
   unsigned char last8100Write;
   unsigned char last5101Write;
};

struct mapper85Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
};

struct mapper88Data
{
   unsigned char last8000Write;
};

struct mapper90Data
{
   unsigned char irqCounter;
   unsigned char irqLatchCounter;
   unsigned char irqCounterEnabled;
};

struct mapper91Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
};

struct mapper95Data
{
   unsigned char last8000Write;
};

struct mapper96Data
{
   unsigned char wramBank;
};

struct mapper105Data
{
   unsigned char irqCounter[4];
   unsigned char irqCounterEnabled;
   unsigned char InitialCounter;
   unsigned char WriteCounter;
   unsigned char Bits;
   unsigned char registers[4];
};

struct mapper117Data
{
   unsigned char irqLine;
   unsigned char irqEnabled1;
   unsigned char irqEnabled2;
};

struct mapper160Data
{
   unsigned char irqCounter;
   unsigned char irqLatchCounter;
   unsigned char irqCounterEnabled;
   unsigned char RefreshType;
};


struct mapper165Data
{
   unsigned char latch;
   unsigned char lastB000Write;
   unsigned char lastC000Write;
   unsigned char lastD000Write;
   unsigned char lastE000Write;
};

struct mapper182Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char lastA000Write;
};

struct mapper189Data
{
   unsigned char irqCounter;
   unsigned char irqCounterEnabled;
   unsigned char irqLatchCounter;
   unsigned char last8000Write;
};

struct mapper226Data
{
   unsigned char registers[2];
};

struct mapper230Data
{
   unsigned char numberOfResets;
};

struct mapper232Data
{
   unsigned char last9000Write;
   unsigned char lastA000Write;
};

struct mapper234Data
{
   unsigned char lastFF80Write;
   unsigned char lastFFE8Write;
   unsigned char lastFFC0Write;
};

struct mapper236Data
{
   unsigned char BankSelect;
   unsigned char MapperMode;
};

struct mapper243Data
{
   unsigned char registers[4];
};

struct mapper255Data
{
   unsigned char registers[4];
};

typedef struct _SnssFileHeader
{
   char tag[TAG_LENGTH + 1];
   unsigned int numberOfBlocks;
} SnssFileHeader;

/* this block appears before every block in the SNSS file */
typedef struct _SnssBlockHeader
{
   char tag[TAG_LENGTH + 1];
   unsigned int blockVersion;
   unsigned int blockLength;
} SnssBlockHeader;

#define BASE_BLOCK_LENGTH 0x1931
typedef struct _SnssBaseBlock
{
   unsigned char regA;
   unsigned char regX;
   unsigned char regY;
   unsigned char regFlags;
   unsigned char regStack;
   unsigned short regPc;
   unsigned char reg2000;
   unsigned char reg2001;
   unsigned char cpuRam[0x800];
   unsigned char spriteRam[0x100];
   unsigned char ppuRam[0x1000];
   unsigned char palette[0x20];
   unsigned char mirrorState[4];
   unsigned short vramAddress;
   unsigned char spriteRamAddress;
   unsigned char tileXOffset;
} SnssBaseBlock;

#define VRAM_16K 0x4000
#define VRAM_8K 0x2000
typedef struct _SnssVramBlock
{
   unsigned short vramSize;
   unsigned char vram[0x8000];
} SnssVramBlock;

#define SRAM_1K 0x0400
#define SRAM_2K 0x0800
#define SRAM_3K 0x0C00
#define SRAM_4K 0x1000
#define SRAM_5K 0x1400
#define SRAM_6K 0x1800
#define SRAM_7K 0x1C00
#define SRAM_8K 0x2000
typedef struct _SnssSramBlock
{
   unsigned short sramSize;
   unsigned char sramEnabled;
   unsigned char sram[0x2000];
} SnssSramBlock;

#define MAPPER_BLOCK_LENGTH 0x98
typedef struct _SnssMapperBlock
{
   unsigned short prgPages[4];
   unsigned short chrPages[8];

   union _extraData
   {
      unsigned char mapperData[128];
      struct mapper1Data mapper1;
      struct mapper4Data mapper4;
      struct mapper5Data mapper5;
      struct mapper6Data mapper6;
      struct mapper9Data mapper9;
      struct mapper10Data mapper10;
      struct mapper13Data mapper13;
      struct mapper16Data mapper16;
      struct mapper17Data mapper17;
      struct mapper18Data mapper18;
      struct mapper19Data mapper19;
      struct mapper20Data mapper20;
      struct mapper21Data mapper21;
      struct mapper23Data mapper23;
      struct mapper24Data mapper24;
      struct mapper25Data mapper25;
      struct mapper26Data mapper26;
      struct mapper32Data mapper32;
      struct mapper33Data mapper33;
      struct mapper40Data mapper40;
      struct mapper41Data mapper41;
      struct mapper42Data mapper42;
      struct mapper43Data mapper43;
      struct mapper46Data mapper46;
      struct mapper48Data mapper48;
      struct mapper49Data mapper49;
      struct mapper50Data mapper50;
      struct mapper51Data mapper51;
      struct mapper57Data mapper57;
      struct mapper64Data mapper64;
      struct mapper65Data mapper65;
      struct mapper67Data mapper67;
      struct mapper68Data mapper68;
      struct mapper69Data mapper69;
      struct mapper73Data mapper73;
      struct mapper74Data mapper74;
      struct mapper75Data mapper75;
      struct mapper76Data mapper76;
      struct mapper82Data mapper82;
      struct mapper83Data mapper83;
      struct mapper85Data mapper85;
      struct mapper88Data mapper88;
      struct mapper90Data mapper90;
      struct mapper91Data mapper91;
      struct mapper95Data mapper95;
      struct mapper96Data mapper96;
      struct mapper105Data mapper105;
      struct mapper117Data mapper117;
      struct mapper160Data mapper160;
      struct mapper182Data mapper182;
      struct mapper189Data mapper189;
      struct mapper226Data mapper226;
      struct mapper230Data mapper230;
      struct mapper232Data mapper232;
      struct mapper234Data mapper234;
      struct mapper236Data mapper236;
      struct mapper243Data mapper243;
      struct mapper255Data mapper255;
   } extraData;
} SnssMapperBlock;

typedef struct _SnssControllersBlock
{
   unsigned char dummy; /* needed for some compilers; remove if any members are added */
} SnssControllersBlock;

#define SOUND_BLOCK_LENGTH 0x16
typedef struct _SnssSoundBlock
{
   unsigned char soundRegisters[0x16];
} SnssSoundBlock;

/**************************************************************************/
/* SNSS file manipulation functions */
/**************************************************************************/

typedef struct _SNSS_FILE
{
   int fh;
   SNSS_OPEN_MODE mode;
   SnssFileHeader headerBlock;
   SnssBaseBlock baseBlock;
   SnssVramBlock vramBlock;
   SnssSramBlock sramBlock;
   SnssMapperBlock mapperBlock;
   SnssControllersBlock contBlock;
   SnssSoundBlock soundBlock;
} SNSS_FILE;

/* general file manipulation routines */
SNSS_RETURN_CODE SNSS_OpenFile (SNSS_FILE **snssFile, const char *filename,
                                SNSS_OPEN_MODE mode);
SNSS_RETURN_CODE SNSS_CloseFile (SNSS_FILE **snssFile);

/* block traversal */
SNSS_RETURN_CODE SNSS_GetNextBlockType (SNSS_BLOCK_TYPE *blockType,
                                        SNSS_FILE *snssFile);
SNSS_RETURN_CODE SNSS_SkipNextBlock (SNSS_FILE *snssFile);

/* functions to read/write SNSS blocks */
SNSS_RETURN_CODE SNSS_ReadBlock (SNSS_FILE *snssFile, SNSS_BLOCK_TYPE blockType);
SNSS_RETURN_CODE SNSS_WriteBlock (SNSS_FILE *snssFile, SNSS_BLOCK_TYPE blockType);

/* support functions */
const char *SNSS_GetErrorString (SNSS_RETURN_CODE code);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LIBSNSS_H_ */

/*
** $Log: libsnss.h,v $
** Revision 2.1  2001/05/01 16:36:00  TAKEDA, tohisya
** add mapper infomations
**
** Revision 2.0  2000/12/09 12:58:00  TAKEDA, tohisya
** add mapper infomations
**
** ------------------------------------------------------------
**
** Revision 1.6  2000/07/09 15:37:21  matt
** all block read/write calls now pass through a common handler
**
** Revision 1.5  2000/07/09 03:39:06  matt
** minor modifications
**
** Revision 1.4  2000/07/08 16:01:39  matt
** added bald's changes, made error checking more robust
**
** Revision 1.3  2000/07/05 22:46:52  matt
** cleaned up header
**
** Revision 1.2  2000/07/04 04:46:06  matt
** simplified handling of SNSS states
**
** Revision 1.1  2000/06/29 14:13:28  matt
** initial revision
**
*/
