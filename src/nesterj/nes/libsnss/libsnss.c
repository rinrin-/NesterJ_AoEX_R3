/**************************************************************************/
/*
      libsnss.c

      (C) 2000 The SNSS Group
      See README.TXT file for license and terms of use.

      $Id: libsnss.c,v 1.5 2000/07/09 15:37:21 matt Exp $
*/
/**************************************************************************/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "libsnss.h"
#include "../nes_string.h"
#include "../../debug/debug.h"
#include "../types.h"

/**************************************************************************/
/* This section deals with endian-specific code. */
/**************************************************************************/

static unsigned int
swap32 (unsigned int source)
{
#ifdef USE_LITTLE_ENDIAN
   char buffer[4] __attribute__ ((aligned(4))) ;

   buffer[0] = ((char *) &source)[3];
   buffer[1] = ((char *) &source)[2];
   buffer[2] = ((char *) &source)[1];
   buffer[3] = ((char *) &source)[0];

   return *((unsigned int *) buffer);
#else /* !USE_LITTLE_ENDIAN */
   return source;
#endif /* !USE_LITTLE_ENDIAN */
}

static unsigned short
swap16 (unsigned short source)
{
#ifdef USE_LITTLE_ENDIAN
   char buffer[2];

   buffer[0] = ((char *) &source)[1];
   buffer[1] = ((char *) &source)[0];

   return *((unsigned short *) buffer);
#else /* !USE_LITTLE_ENDIAN */
   return source;
#endif /* !USE_LITTLE_ENDIAN */
}

/**************************************************************************/
/* support functions */
/**************************************************************************/

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadBlockHeader (SnssBlockHeader *header, SNSS_FILE *snssFile)
{
//	DEBUG("SNSS_ReadBlockHeader in");
   char headerBytes[12];

   if (NES_fread(headerBytes, 1, 12, snssFile->fh) != 12)
   {
      return SNSS_READ_FAILED;
   }

   _strncpy (header->tag, &headerBytes[0], TAG_LENGTH);
   header->tag[4] = '\0';
   header->blockVersion = (unsigned int)headerBytes[4] + (((unsigned int)headerBytes[5]) << 8) +
   						 (((unsigned int)headerBytes[6]) << 16) + (((unsigned int)headerBytes[7]) << 24);
   header->blockVersion = swap32 (header->blockVersion);
//	DEBUG("SNSS_ReadBlockHeader 2");
   header->blockLength =  (unsigned int)headerBytes[8] + (((unsigned int)headerBytes[9]) << 8) +
   						 (((unsigned int)headerBytes[10]) << 16) + (((unsigned int)headerBytes[11]) << 24);
   header->blockLength = swap32 (header->blockLength);

//	DEBUG("SNSS_ReadBlockHeader out");
   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteBlockHeader (SnssBlockHeader *header, SNSS_FILE *snssFile)
{
   char headerBytes[12];
   unsigned int tempInt;

   _strncpy (&headerBytes[0], header->tag, TAG_LENGTH);

   tempInt = swap32 (header->blockVersion);
   headerBytes[4] = ((char *) &tempInt)[0];
   headerBytes[5] = ((char *) &tempInt)[1];
   headerBytes[6] = ((char *) &tempInt)[2];
   headerBytes[7] = ((char *) &tempInt)[3];

   tempInt = swap32 (header->blockLength);
   headerBytes[8] = ((char *) &tempInt)[0];
   headerBytes[9] = ((char *) &tempInt)[1];
   headerBytes[10] = ((char *) &tempInt)[2];
   headerBytes[11] = ((char *) &tempInt)[3];

   if (NES_fwrite (headerBytes, 1, 12, snssFile->fh) != 12)
   {
      return SNSS_WRITE_FAILED;
   }

   return SNSS_OK;
}

/**************************************************************************/

const char *
SNSS_GetErrorString (SNSS_RETURN_CODE code)
{
   switch (code)
   {
   case SNSS_OK:
      return "no error";

   case SNSS_BAD_FILE_TAG:
      return "not an SNSS file";

   case SNSS_OPEN_FAILED:
      return "could not open SNSS file";

   case SNSS_CLOSE_FAILED:
      return "could not close SNSS file";

   case SNSS_READ_FAILED:
      return "could not read from SNSS file";

   case SNSS_WRITE_FAILED:
      return "could not write to SNSS file";

   case SNSS_OUT_OF_MEMORY:
      return "out of memory";

   case SNSS_UNSUPPORTED_BLOCK:
      return "unsupported block type";

   default:
      return "unknown error";
   }
}

/**************************************************************************/
/* functions for reading and writing SNSS file headers */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadFileHeader (SNSS_FILE *snssFile)
{
   if (NES_fread (snssFile->headerBlock.tag, 1, 4, snssFile->fh) != 4)
   {
      return SNSS_READ_FAILED;
   }

   if (0 != _strncmp(snssFile->headerBlock.tag, "SNSS", 4))
   {
      return SNSS_BAD_FILE_TAG;
   }

   snssFile->headerBlock.tag[4] = '\0';

   if (NES_fread (&snssFile->headerBlock.numberOfBlocks, 1, sizeof (unsigned int), snssFile->fh) != sizeof (unsigned int))
   {
      return SNSS_READ_FAILED;
   }
   snssFile->headerBlock.numberOfBlocks = swap32 (snssFile->headerBlock.numberOfBlocks);

   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteFileHeader (SNSS_FILE *snssFile)
{
   unsigned int tempInt;
   char writeBuffer[8];

   /* always place the SNSS tag in this field */
   _strncpy (&writeBuffer[0], "SNSS", 4);
   tempInt = swap32 (snssFile->headerBlock.numberOfBlocks);
   writeBuffer[4] = ((char *) &tempInt)[0];
   writeBuffer[5] = ((char *) &tempInt)[1];
   writeBuffer[6] = ((char *) &tempInt)[2];
   writeBuffer[7] = ((char *) &tempInt)[3];

   if (NES_fwrite (writeBuffer, 1, 8, snssFile->fh) != 8)
   {
      return SNSS_WRITE_FAILED;
   }

   return SNSS_OK;
}

/**************************************************************************/
/* general file manipulation functions */
/**************************************************************************/

SNSS_FILE g_SNSSObj;

SNSS_RETURN_CODE
SNSS_OpenFile (SNSS_FILE **snssFile, const char *filename, SNSS_OPEN_MODE mode)
{
   _memset(&g_SNSSObj, 0x00, sizeof(g_SNSSObj));

   g_SNSSObj.mode = mode;

   if (SNSS_OPEN_READ == mode)
      g_SNSSObj.fh = NES_fopen (filename, FILE_MODE_READ);
   else
   {
      g_SNSSObj.fh = NES_fopen (filename, FILE_MODE_WRITE);
      g_SNSSObj.headerBlock.numberOfBlocks = 0;
   }

   if (0 > g_SNSSObj.fh)
   {
      *snssFile = NULL;
      return SNSS_OPEN_FAILED;
   }
	*snssFile = &g_SNSSObj;

   if (SNSS_OPEN_READ == mode)
      return SNSS_ReadFileHeader(*snssFile);
   else
      return SNSS_WriteFileHeader(*snssFile);
}

/**************************************************************************/

SNSS_RETURN_CODE
SNSS_CloseFile (SNSS_FILE **snssFile)
{
//   int prevLoc;
   SNSS_RETURN_CODE code;

   /* file was never open, so this should indicate success- kinda. */
   if (NULL == *snssFile)
   {
      return SNSS_OK;
   }

   if (SNSS_OPEN_WRITE == (*snssFile)->mode)
   {
//      prevLoc = ftell((*snssFile)->fp);
      NES_fseek((*snssFile)->fh, 0, FILE_SEEK_SET);

      /* write the header again to get block count correct */
      if (SNSS_OK != (code = SNSS_WriteFileHeader(*snssFile)))
         return SNSS_CLOSE_FAILED;

//      NES_fseek((*snssFile)->fp, prevLoc, SEEK_SET);
   }

/*
   if (NES_fclose((*snssFile)->fh) != 0)
   {
      return SNSS_CLOSE_FAILED;
   }
*/
   NES_fclose((*snssFile)->fh);

//   free(*snssFile);
   *snssFile = NULL;

   return SNSS_OK;
}

/**************************************************************************/

SNSS_RETURN_CODE
SNSS_GetNextBlockType (SNSS_BLOCK_TYPE *blockType, SNSS_FILE *snssFile)
{
   char tagBuffer[TAG_LENGTH + 1];
	DEBUG("SNSS_GetNextBlockType in");

   if (NES_fread (tagBuffer, 1, TAG_LENGTH, snssFile->fh) != TAG_LENGTH)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_READ_FAILED");
      return SNSS_READ_FAILED;
   }
   tagBuffer[TAG_LENGTH] = '\0';

   /* reset the file pointer to the start of the block */
   if (NES_fseek (snssFile->fh, -TAG_LENGTH, FILE_SEEK_CUR) < 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_READ_FAILED");
      return SNSS_READ_FAILED;
   }

   /* figure out which type of block it is */
   if (_strcmp (tagBuffer, "BASR") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_BASR");
      *blockType = SNSS_BASR;
      return SNSS_OK;
   }
   else if (_strcmp (tagBuffer, "VRAM") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_VRAM");
      *blockType = SNSS_VRAM;
      return SNSS_OK;
   }
   else if (_strcmp (tagBuffer, "SRAM") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_SRAM");
      *blockType = SNSS_SRAM;
      return SNSS_OK;
   }
   else if (_strcmp (tagBuffer, "MPRD") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_MPRD");
      *blockType = SNSS_MPRD;
      return SNSS_OK;
   }
   else if (_strcmp (tagBuffer, "CNTR") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_CNTR");
      *blockType = SNSS_CNTR;
      return SNSS_OK;
   }
   else if (_strcmp (tagBuffer, "SOUN") == 0)
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_SOUN");
      *blockType = SNSS_SOUN;
      return SNSS_OK;
   }
   else
   {
	  DEBUG("SNSS_GetNextBlockType SNSS_UNKNOWN_BLOCK");
      *blockType = SNSS_UNKNOWN_BLOCK;
      return SNSS_OK;
   }
}

/**************************************************************************/

SNSS_RETURN_CODE
SNSS_SkipNextBlock (SNSS_FILE *snssFile)
{
   unsigned int blockLength;

   /* skip the block's tag and version */
   if (NES_fseek (snssFile->fh, TAG_LENGTH + sizeof (unsigned int), FILE_SEEK_CUR) < 0)
   {
      return SNSS_READ_FAILED;
   }

   /* get the block data length */
   if (NES_fread (&blockLength, 1, sizeof (unsigned int), snssFile->fh) != sizeof (unsigned int))
   {
      return SNSS_READ_FAILED;
   }
   blockLength = swap32 (blockLength);

   /* skip over the block data */
   if (NES_fseek (snssFile->fh, blockLength, FILE_SEEK_CUR) < 0)
   {
      return SNSS_READ_FAILED;
   }

   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing base register blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadBaseBlock (SNSS_FILE *snssFile)
{
   char blockBytes[BASE_BLOCK_LENGTH];
   uint8 *pbt = (uint8 *)blockBytes;
   SnssBlockHeader header;

   if (SNSS_ReadBlockHeader (&header, snssFile) != SNSS_OK)
   {
      return SNSS_READ_FAILED;
   }

	int nReadSize = MIN (header.blockLength, BASE_BLOCK_LENGTH);
   if (NES_fread (blockBytes, 1, nReadSize, snssFile->fh) != nReadSize)
   {
      return SNSS_READ_FAILED;
   }

   snssFile->baseBlock.regA = blockBytes[0x0];
   snssFile->baseBlock.regX = blockBytes[0x1];
   snssFile->baseBlock.regY = blockBytes[0x2];
   snssFile->baseBlock.regFlags = blockBytes[0x3];
   snssFile->baseBlock.regStack = blockBytes[0x4];
   snssFile->baseBlock.regPc = pbt[0x5] + (pbt[0x6] << 8);
   snssFile->baseBlock.regPc = swap16 (snssFile->baseBlock.regPc);
   snssFile->baseBlock.reg2000 = blockBytes[0x7];
   snssFile->baseBlock.reg2001 = blockBytes[0x8];
   _memcpy (&snssFile->baseBlock.cpuRam, &blockBytes[0x9], 0x800);
   _memcpy (&snssFile->baseBlock.spriteRam, &blockBytes[0x809], 0x100);
   _memcpy (&snssFile->baseBlock.ppuRam, &blockBytes[0x909], 0x1000);
   _memcpy (&snssFile->baseBlock.palette, &blockBytes[0x1909], 0x20);
   _memcpy (&snssFile->baseBlock.mirrorState, &blockBytes[0x1929], 0x4);
   snssFile->baseBlock.vramAddress = pbt[0x192D] + (pbt[0x192E] << 8);
   snssFile->baseBlock.vramAddress = swap16 (snssFile->baseBlock.vramAddress);
   snssFile->baseBlock.spriteRamAddress = blockBytes[0x192F];
   snssFile->baseBlock.tileXOffset = blockBytes[0x1930];
   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteBaseBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;
   SNSS_RETURN_CODE returnCode;
   char blockBytes[BASE_BLOCK_LENGTH];
   unsigned short tempShort;

   _strcpy (header.tag, "BASR");
   header.blockVersion = SNSS_BLOCK_VERSION;
   header.blockLength = BASE_BLOCK_LENGTH;
   if ((returnCode = SNSS_WriteBlockHeader (&header, snssFile)) != SNSS_OK)
   {
      return returnCode;
   }

   blockBytes[0x0] = snssFile->baseBlock.regA;
   blockBytes[0x1] = snssFile->baseBlock.regX;
   blockBytes[0x2] = snssFile->baseBlock.regY;
   blockBytes[0x3] = snssFile->baseBlock.regFlags;
   blockBytes[0x4] = snssFile->baseBlock.regStack;
   tempShort = swap16 (snssFile->baseBlock.regPc);
   blockBytes[0x5] = ((char *) &tempShort)[0];
   blockBytes[0x6] = ((char *) &tempShort)[1];
   blockBytes[0x7] = snssFile->baseBlock.reg2000;
   blockBytes[0x8] = snssFile->baseBlock.reg2001;
   _memcpy (&blockBytes[0x9], &snssFile->baseBlock.cpuRam, 0x800);
   _memcpy (&blockBytes[0x809], &snssFile->baseBlock.spriteRam, 0x100);
   _memcpy (&blockBytes[0x909], &snssFile->baseBlock.ppuRam, 0x1000);
   _memcpy (&blockBytes[0x1909], &snssFile->baseBlock.palette, 0x20);
   _memcpy (&blockBytes[0x1929], &snssFile->baseBlock.mirrorState, 0x4);
   tempShort = swap16 (snssFile->baseBlock.vramAddress);
   blockBytes[0x192D] = ((char *) &tempShort)[0];
   blockBytes[0x192E] = ((char *) &tempShort)[1];
   blockBytes[0x192F] = snssFile->baseBlock.spriteRamAddress;
   blockBytes[0x1930] = snssFile->baseBlock.tileXOffset;

   if (NES_fwrite (blockBytes, 1, BASE_BLOCK_LENGTH, snssFile->fh) != BASE_BLOCK_LENGTH)
   {
      return SNSS_WRITE_FAILED;
   }

   snssFile->headerBlock.numberOfBlocks++;

   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing VRAM blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadVramBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;

   if (SNSS_ReadBlockHeader (&header, snssFile) != SNSS_OK)
   {
      return SNSS_READ_FAILED;
   }

   if (NES_fread (snssFile->vramBlock.vram, 1, header.blockLength, snssFile->fh) != header.blockLength)
   {
      return SNSS_READ_FAILED;
   }

   snssFile->vramBlock.vramSize = header.blockLength;

   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteVramBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;
   SNSS_RETURN_CODE returnCode;

   _strcpy (header.tag, "VRAM");
   header.blockVersion = SNSS_BLOCK_VERSION;
   header.blockLength = snssFile->vramBlock.vramSize;
   if ((returnCode = SNSS_WriteBlockHeader (&header, snssFile)) != SNSS_OK)
   {
      return returnCode;
   }

   if (NES_fwrite (snssFile->vramBlock.vram, 1, snssFile->vramBlock.vramSize, snssFile->fh) != snssFile->vramBlock.vramSize)
   {
      return SNSS_WRITE_FAILED;
   }

   snssFile->headerBlock.numberOfBlocks++;

   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing SRAM blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadSramBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;

   if (SNSS_ReadBlockHeader (&header, snssFile) != SNSS_OK)
   {
      return SNSS_READ_FAILED;
   }

   if (NES_fread (&snssFile->sramBlock.sramEnabled, 1, 1, snssFile->fh) != 1)
   {
      return SNSS_READ_FAILED;
   }

   /* read blockLength - 1 bytes to get all of the SRAM */
   int nReadSize = MIN (header.blockLength - 1, SRAM_8K);
   if (NES_fread (&snssFile->sramBlock.sram, 1, nReadSize, snssFile->fh) != nReadSize)
   {
      return SNSS_READ_FAILED;
   }

   /* SRAM size is the size of the block - 1 (SRAM enabled byte) */
   snssFile->sramBlock.sramSize = header.blockLength - 1;

   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteSramBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;
   SNSS_RETURN_CODE returnCode;

   _strcpy (header.tag, "SRAM");
   header.blockVersion = SNSS_BLOCK_VERSION;
   /* length of block is size of SRAM plus SRAM enabled byte */
   header.blockLength = snssFile->sramBlock.sramSize + 1;
   if ((returnCode = SNSS_WriteBlockHeader (&header, snssFile)) != SNSS_OK)
   {
      return returnCode;
   }

   if (NES_fwrite (&snssFile->sramBlock.sramEnabled, 1, 1, snssFile->fh) != 1)
   {
      return SNSS_WRITE_FAILED;
   }

   if (NES_fwrite (snssFile->sramBlock.sram, 1, snssFile->sramBlock.sramSize, snssFile->fh) != snssFile->sramBlock.sramSize)
   {
      return SNSS_WRITE_FAILED;
   }

   snssFile->headerBlock.numberOfBlocks++;

   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing mapper data blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadMapperBlock (SNSS_FILE *snssFile)
{
   char blockBytes[0x8 + 0x10 + 0x80];
   int i;
   SnssBlockHeader header;

   if (SNSS_ReadBlockHeader (&header, snssFile) != SNSS_OK)
   {
      return SNSS_READ_FAILED;
   }

   if (NES_fread (blockBytes, 1, MIN (0x8 + 0x10 + 0x80, header.blockLength), snssFile->fh) != MIN (0x8 + 0x10 + 0x80, header.blockLength))
   {
      return SNSS_READ_FAILED;
   }

   for (i = 0; i < 4; i++)
   {
      snssFile->mapperBlock.prgPages[i] = *((unsigned short *) &blockBytes[i * 2]);
      snssFile->mapperBlock.prgPages[i] = swap16 (snssFile->mapperBlock.prgPages[i]);
   }

   for (i = 0; i < 8; i++)
   {
      snssFile->mapperBlock.chrPages[i] = *((unsigned short *) &blockBytes[0x8 + (i * 2)]);
      snssFile->mapperBlock.chrPages[i] = swap16 (snssFile->mapperBlock.chrPages[i]);
   }

   _memcpy (&snssFile->mapperBlock.extraData.mapperData, &blockBytes[0x18], 0x80);

   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteMapperBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;
   char blockBytes[MAPPER_BLOCK_LENGTH];
   unsigned short tempShort;
   int i;
   SNSS_RETURN_CODE returnCode;

   _strcpy (header.tag, "MPRD");
   header.blockVersion = SNSS_BLOCK_VERSION;
   header.blockLength = MAPPER_BLOCK_LENGTH;
   if ((returnCode = SNSS_WriteBlockHeader (&header, snssFile)) != SNSS_OK)
   {
      return returnCode;
   }

   for (i = 0; i < 4; i++)
   {
      tempShort = swap16 (snssFile->mapperBlock.prgPages[i]);
      blockBytes[(i * 2) + 0] = ((char *) &tempShort)[0];
      blockBytes[(i * 2) + 1] = ((char *) &tempShort)[1];
   }

   for (i = 0; i < 8; i++)
   {
      tempShort = swap16 (snssFile->mapperBlock.chrPages[i]);
      blockBytes[0x8 + (i * 2) + 0] = ((char *) &tempShort)[0];
      blockBytes[0x8 + (i * 2) + 1] = ((char *) &tempShort)[1];
   }

   _memcpy (&blockBytes[0x18], &snssFile->mapperBlock.extraData.mapperData, 0x80);

   if (NES_fwrite (blockBytes, 1, MAPPER_BLOCK_LENGTH, snssFile->fh) != MAPPER_BLOCK_LENGTH)
   {
      return SNSS_WRITE_FAILED;
   }

   snssFile->headerBlock.numberOfBlocks++;

   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing controller data blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadControllersBlock (SNSS_FILE *snssFile)
{
   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteControllersBlock (SNSS_FILE *snssFile)
{
   return SNSS_OK;
}

/**************************************************************************/
/* functions for reading and writing sound blocks */
/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_ReadSoundBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;

   if (SNSS_ReadBlockHeader (&header, snssFile) != SNSS_OK)
   {
      return SNSS_READ_FAILED;
   }

   if (NES_fread (snssFile->soundBlock.soundRegisters, 1, MIN (header.blockLength, 0x16), snssFile->fh) != MIN (header.blockLength, 0x16))
   {
      return SNSS_READ_FAILED;
   }

   return SNSS_OK;
}

/**************************************************************************/

static SNSS_RETURN_CODE
SNSS_WriteSoundBlock (SNSS_FILE *snssFile)
{
   SnssBlockHeader header;
   SNSS_RETURN_CODE returnCode;

   _strcpy (header.tag, "SOUN");
   header.blockVersion = SNSS_BLOCK_VERSION;
   header.blockLength = SOUND_BLOCK_LENGTH;
   if ((returnCode = SNSS_WriteBlockHeader (&header, snssFile)) != SNSS_OK)
   {
      return returnCode;
   }

   if (NES_fwrite (snssFile->soundBlock.soundRegisters, 1, SOUND_BLOCK_LENGTH, snssFile->fh) != SOUND_BLOCK_LENGTH)
   {
      return SNSS_WRITE_FAILED;
   }

   snssFile->headerBlock.numberOfBlocks++;

   return SNSS_OK;
}

/**************************************************************************/
/* general functions for reading and writing SNSS data blocks */
/**************************************************************************/

SNSS_RETURN_CODE
SNSS_ReadBlock (SNSS_FILE *snssFile, SNSS_BLOCK_TYPE blockType)
{
	DEBUGVALUE("SNSS_ReadBlock in", blockType);
   switch (blockType)
   {
   case SNSS_BASR:
      return SNSS_ReadBaseBlock (snssFile);

   case SNSS_VRAM:
      return SNSS_ReadVramBlock (snssFile);

   case SNSS_SRAM:
      return SNSS_ReadSramBlock (snssFile);

   case SNSS_MPRD:
      return SNSS_ReadMapperBlock (snssFile);

   case SNSS_CNTR:
      return SNSS_ReadControllersBlock (snssFile);

   case SNSS_SOUN:
      return SNSS_ReadSoundBlock (snssFile);

   case SNSS_UNKNOWN_BLOCK:
   default:
       return SNSS_UNSUPPORTED_BLOCK;
   }
}

/**************************************************************************/

SNSS_RETURN_CODE
SNSS_WriteBlock (SNSS_FILE *snssFile, SNSS_BLOCK_TYPE blockType)
{
   switch (blockType)
   {
   case SNSS_BASR:
      return SNSS_WriteBaseBlock (snssFile);

   case SNSS_VRAM:
      return SNSS_WriteVramBlock (snssFile);

   case SNSS_SRAM:
      return SNSS_WriteSramBlock (snssFile);

   case SNSS_MPRD:
      return SNSS_WriteMapperBlock (snssFile);

   case SNSS_CNTR:
      return SNSS_WriteControllersBlock (snssFile);

   case SNSS_SOUN:
      return SNSS_WriteSoundBlock (snssFile);

   case SNSS_UNKNOWN_BLOCK:
   default:
       return SNSS_UNSUPPORTED_BLOCK;
   }
}

/*
** $Log: libsnss.c,v $
** Revision 1.5  2000/07/09 15:37:21  matt
** all block read/write calls now pass through a common handler
**
** Revision 1.4  2000/07/09 03:39:06  matt
** minor modifications
**
** Revision 1.3  2000/07/08 16:01:39  matt
** added bald's changes, made error checking more robust
**
** Revision 1.2  2000/07/04 04:46:06  matt
** simplified handling of SNSS states
**
** Revision 1.1  2000/06/29 14:13:28  matt
** initial revision
**
*/
