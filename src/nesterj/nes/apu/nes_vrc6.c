/*
** Konami VRC6 ExSound by TAKEDA, toshiya
**
** original: s_vrc6.c in nezp0922
*/
#include "nes_apu.h"
#include "../nes_string.h"

extern apu_t g_apu_t;

// ----------------------------------------------------------------------------
// VRC6 Sound struct

typedef struct {
	uint32 cps;
	int32 cycles;
	uint32 spd;
	uint8 regs[3];
	uint8 update;
	uint8 adr;
	uint8 mute;
} VRC6_SQUARE;

typedef struct {
	uint32 cps;
	int32 cycles;
	uint32 spd;
	uint32 output;
	uint8 regs[3];
	uint8 update;
	uint8 adr;
	uint8 mute;
} VRC6_SAW;

typedef struct {
	VRC6_SQUARE square[2];
	VRC6_SAW saw;
	uint32 mastervolume;
} VRC6SOUND;


VRC6SOUND vrc6_sound;


static int32 VRC6SoundSquareRender(VRC6_SQUARE *ch)
{
	uint32 output;
	if (ch->update)
	{
		if (ch->update & (2 | 4))
		{
			ch->spd = (((ch->regs[2] & 0x0F) << 8) + ch->regs[1] + 1) << 18;
		}
		ch->update = 0;
	}

	if (!ch->spd) return 0;

	ch->cycles -= ch->cps;
	while (ch->cycles < 0)
	{
		ch->cycles += ch->spd;
		ch->adr++;
	}
	ch->adr &= 0xF;

	if (ch->mute || !(ch->regs[2] & 0x80)) return 0;

	output = LinearToLog(ch->regs[0] & 0x0F) + vrc6_sound.mastervolume;
	if (!(ch->regs[0] & 0x80) && (ch->adr < ((ch->regs[0] >> 4) + 1)))
	{
#if 1
		return 0;	/* and array gate */
#else
		output++;	/* negative gate */
#endif
	}
	return LogToLinear(output, LOG_LIN_BITS - LIN_BITS - 16 - 1);
}

static int32 VRC6SoundSawRender(VRC6_SAW *ch)
{
	uint32 output;

	if (ch->update)
	{
		if (ch->update & (2 | 4))
		{
			ch->spd = (((ch->regs[2] & 0x0F) << 8) + ch->regs[1] + 1) << 18;
		}
		ch->update = 0;
	}

	if (!ch->spd) return 0;

	ch->cycles -= ch->cps;
	while (ch->cycles < 0)
	{
		ch->cycles += ch->spd;
		ch->output += (ch->regs[0] & 0x3F);
		if (7 == ++ch->adr)
		{
			ch->adr = 0;
			ch->output = 0;
		}
	}

	if (ch->mute || !(ch->regs[2] & 0x80)) return 0;

	output = LinearToLog((ch->output >> 3) & 0x1F) + vrc6_sound.mastervolume;
	return LogToLinear(output, LOG_LIN_BITS - LIN_BITS - 16 - 1);
}

static int32 VRC6SoundRender(void)
{
	int32 accum = 0;
	accum += VRC6SoundSquareRender(&vrc6_sound.square[0]);
	accum += VRC6SoundSquareRender(&vrc6_sound.square[1]);
	accum += VRC6SoundSawRender(&vrc6_sound.saw);
	return accum >> 8;
}

static void VRC6SoundVolume(uint32 volume)
{
	vrc6_sound.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void VRC6SoundWrite9000(uint32 address, uint8 value)
{
	vrc6_sound.square[0].regs[address & 3] = value;
	vrc6_sound.square[0].update |= 1 << (address & 3);
}
static void VRC6SoundWriteA000(uint32 address, uint8 value)
{
	vrc6_sound.square[1].regs[address & 3] = value;
	vrc6_sound.square[1].update |= 1 << (address & 3);
}
static void VRC6SoundWriteB000(uint32 address, uint8 value)
{
	vrc6_sound.saw.regs[address & 3] = value;
	vrc6_sound.saw.update |= 1 << (address & 3);
}

static void VRC6SoundWrite(uint32 address, uint8 value)
{
	switch(address & 0xF000) {
		case 0x9000:
			VRC6SoundWrite9000(address, value);
			break;
		case 0xA000:
			VRC6SoundWriteA000(address, value);
			break;
		case 0xB000:
			VRC6SoundWriteB000(address, value);
			break;
	}
}

static void VRC6SoundSquareReset(VRC6_SQUARE *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 12 * SAMPLE_RATE, 18);
}

static void VRC6SoundSawReset(VRC6_SAW *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 24 * SAMPLE_RATE, 18);
}

static void VRC6SoundReset(void)
{
	_memset(&vrc6_sound, 0, sizeof(VRC6SOUND));
	VRC6SoundSquareReset(&vrc6_sound.square[0]);
	VRC6SoundSquareReset(&vrc6_sound.square[1]);
	VRC6SoundSawReset(&vrc6_sound.saw);
}

static int VRC6SoundInit(void){
	VRC6SoundReset();
	VRC6SoundVolume(1);
	return 0;
}

static apu_memwrite vrc6_memwrite[] = {
   { 0x9000, 0xbfff, VRC6SoundWrite },
   {     -1,     -1, NULL }
};

apuext_t vrc6_ext = {
	VRC6SoundInit,		/* init */
	NULL,		/* shutdown */
	VRC6SoundReset,	/* reset */
	NULL,		/* paramschanged */
	VRC6SoundRender,	/* process */
	NULL,			/* mem_read */
	vrc6_memwrite,	/* mem_write */
	NULL			/* mem_writesync */
};

