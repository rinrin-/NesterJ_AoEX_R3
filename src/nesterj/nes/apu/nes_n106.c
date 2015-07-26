/*
** Namco 106 ExSound by TAKEDA, toshiya
**
** original: s_n106.c in nezp0922
*/
#include "nes_apu.h"
#include "../nes_string.h"

extern apu_t g_apu_t;

// ----------------------------------------------------------------------------
// N106 Sound struct

typedef struct {
	uint32 logvol;
	int32 cycles;
	uint32 spd;
	uint32 phase;
	uint32 tlen;
	uint8 update;
	uint8 freql;
	uint8 freqm;
	uint8 freqh;
	uint8 vreg;
	uint8 tadr;
	uint8 nazo;
	uint8 mute;
} N106_WM;

typedef struct {
	uint32 cps;
	uint32 mastervolume;

	N106_WM ch[8];

	uint8 addressauto;
	uint8 address;
	uint8 chinuse;

	uint32 tone[0x100];	/* TONE DATA */
	uint8 data[0x80];
} N106SOUND;


N106SOUND n106_sound;


__inline static void UPDATE(N106_WM *chp)
{
	if (chp->update & 3)
	{
		uint32 freq;
		freq  = ((int)chp->freql);
		freq += ((int)chp->freqm) << 8;
		freq += ((int)chp->freqh) << 16;
		chp->spd = freq & 0x3ffff;
	}
	if (chp->update & 2)
	{
		uint32 tlen;
		tlen = (0x20 - (chp->freqh & 0x1c)) << 18;
		if (chp->tlen != tlen)
		{
			chp->tlen = tlen;
			chp->phase = 0;
		}
	}
	if (chp->update & 4)
	{
		chp->logvol = LinearToLog((chp->vreg & 0x0f) << 2);
	}
	chp->update = 0;
}

static int32 N106SoundRender(void)
{
	N106_WM *chp;
	int32 accum = 0;
	for (chp = &n106_sound.ch[8 - n106_sound.chinuse]; chp < &n106_sound.ch[8]; chp++)
	{
		uint32 cyclesspd = n106_sound.chinuse << 20;
		if (chp->update) UPDATE(chp);
		chp->cycles -= n106_sound.cps;
		while (chp->cycles < 0)
		{
			chp->cycles += cyclesspd;
			chp->phase += chp->spd;
		}
		while (chp->phase >= chp->tlen) chp->phase -= chp->tlen;
		if (chp->mute) continue;
		accum += LogToLinear(n106_sound.tone[((chp->phase >> 18) + chp->tadr) & 0xff] + chp->logvol + n106_sound.mastervolume, LOG_LIN_BITS - LIN_BITS - LIN_BITS - 10);
	}
	return accum >> 8;
}

static void N106SoundVolume(uint32 volume)
{
	n106_sound.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void N106SoundWriteAddr(uint32 address, uint8 value)
{
	n106_sound.address     = value & 0x7f;
	n106_sound.addressauto = (value & 0x80) ? 1 : 0;
}

static void N106SoundWriteData(uint32 address, uint8 value)
{
	n106_sound.data[n106_sound.address] = value;
	n106_sound.tone[n106_sound.address * 2]     = LinearToLog(((int)(value & 0xf) << 2) - 0x20);
	n106_sound.tone[n106_sound.address * 2 + 1] = LinearToLog(((int)(value >>  4) << 2) - 0x20);
	if (n106_sound.address >= 0x40)
	{
		N106_WM *chp = &n106_sound.ch[(n106_sound.address - 0x40) >> 3];
		switch (n106_sound.address & 7)
		{
			case 0:
				chp->update |= 1;
				chp->freql = value;
				break;
			case 2:
				chp->update |= 1;
				chp->freqm = value;
				break;
			case 4:
				chp->update |= 2;
				chp->freqh = value;
				break;
			case 6:
				chp->tadr = value & 0xff;
				break;
			case 7:
				chp->update |= 4;
				chp->vreg = value;
				chp->nazo = (value >> 4) & 0x07;
				if (chp == &n106_sound.ch[7])
					n106_sound.chinuse = 1 + chp->nazo;
				break;
		}
	}
	if (n106_sound.addressauto)
	{
		n106_sound.address = (n106_sound.address + 1) & 0x7f;
	}
}

static void N106SoundWrite(uint32 address, uint8 value)
{
	if (address == 0x4800)
	{
		N106SoundWriteData(address, value);
	}
	else if (address == 0xF800)
	{
		N106SoundWriteAddr(address, value);
	}
}

static uint8 N106SoundReadData(uint32 address)
{
	uint8 ret = n106_sound.data[n106_sound.address];
	if (n106_sound.addressauto)
	{
		n106_sound.address = (n106_sound.address + 1) & 0x7f;
	}
	return ret;
}

static uint8 N106SoundRead(uint32 address)
{
	if (address == 0x4800)
	{
		return N106SoundReadData(address);
	}
	else
	{
		return 0x00;
	}
}

static void N106SoundReset(void)
{
	int i;

	apu_t *apu;
	apu = apu_getcontext ();

	_memset (&n106_sound, 0, sizeof(N106SOUND));
	for (i = 0; i < 8; i++)
	{
		n106_sound.ch[i].tlen = 0x10 << 18;
		n106_sound.ch[i].logvol = LinearToLog(0);
	}

	n106_sound.addressauto = 1;
	n106_sound.chinuse = 8;
	n106_sound.cps = DivFix(NES_BASECYCLES, 45 * SAMPLE_RATE, 20);
}

static int N106SoundInit(void){
	N106SoundReset();
	N106SoundVolume(1);
	return 0;
}


static void N106SoundWriteDR(uint32 address, uint8 value)
{
	N106SoundRead(address & 0xFFFF);
}


static apu_memwrite n106_memwrite[] = {
   { 0x4800, 0x4800, N106SoundWrite },
   { 0xF800, 0xF800, N106SoundWrite },
   { 0x10000, 0x1FFFF, N106SoundWriteDR },
   {     -1,     -1, NULL }
};

apuext_t n106_ext = {
	N106SoundInit,		/* init */
	NULL,		/* shutdown */
	N106SoundReset,	/* reset */
	NULL,		/* paramschanged */
	N106SoundRender,	/* process */
	NULL,			/* mem_read */
	n106_memwrite,	/* mem_write */
	NULL			/* mem_writesync */
};

