/*
** Sunsoft FME7 ExSound by TAKEDA, toshiya
**
** original: s_fme7.c in nezp0922
*/
#include "nes_apu.h"
#include "../nes_string.h"

extern apu_t g_apu_t;

// ----------------------------------------------------------------------------
// FME7 Sound struct

typedef struct {
	uint32 cps;
	int32 cycles;
	uint32 spd;
	uint8 regs[3];
	uint8 update;
	uint8 adr;
	uint8 mute;
	uint8 key;
} PSG_SQUARE;

typedef struct {
	uint32 cps;
	int32 cycles;
	uint32 spd;
	uint32 noiserng;
	uint8 regs[1];
	uint8 update;
	uint8 noiseout;
} PSG_NOISE;

typedef struct {
	uint32 cps;
	int32 cycles;
	uint32 spd;
	uint32 envout;
	int8 *adr;
	uint8 regs[3];
	uint8 update;
} PSG_ENVELOPE;

typedef struct {
	PSG_SQUARE square[3];
	PSG_ENVELOPE envelope;
	PSG_NOISE noise;
	uint32 mastervolume;
	uint32 adr;
} PSGSOUND;


static PSGSOUND psg_sound;


static int8 env_pulse[] =
{
	0x1F,+02,0x1E,+02,0x1D,+02,0x1C,+02,0x1B,+02,0x1A,+02,0x19,+02,0x18,+02,
	0x17,+02,0x16,+02,0x15,+02,0x14,+02,0x13,+02,0x12,+02,0x11,+02,0x10,+02,
	0x0F,+02,0x0E,+02,0x0D,+02,0x0C,+02,0x0B,+02,0x0A,+02,0x09,+02,0x08,+02,
	0x07,+02,0x06,+02,0x05,+02,0x04,+02,0x03,+02,0x02,+02,0x01,+02,0x00,+00,
};
static int8 env_pulse_hold[] =
{
	0x1F,+02,0x1E,+02,0x1D,+02,0x1C,+02,0x1B,+02,0x1A,+02,0x19,+02,0x18,+02,
	0x17,+02,0x16,+02,0x15,+02,0x14,+02,0x13,+02,0x12,+02,0x11,+02,0x10,+02,
	0x0F,+02,0x0E,+02,0x0D,+02,0x0C,+02,0x0B,+02,0x0A,+02,0x09,+02,0x08,+02,
	0x07,+02,0x06,+02,0x05,+02,0x04,+02,0x03,+02,0x02,+02,0x01,+02,0x00,+02,
	0x1F,+00,
};
static int8 env_saw[] =
{
	0x1F,+02,0x1E,+02,0x1D,+02,0x1C,+02,0x1B,+02,0x1A,+02,0x19,+02,0x18,+02,
	0x17,+02,0x16,+02,0x15,+02,0x14,+02,0x13,+02,0x12,+02,0x11,+02,0x10,+02,
	0x0F,+02,0x0E,+02,0x0D,+02,0x0C,+02,0x0B,+02,0x0A,+02,0x09,+02,0x08,+02,
	0x07,+02,0x06,+02,0x05,+02,0x04,+02,0x03,+02,0x02,+02,0x01,+02,0x00,-62,
};
static int8 env_tri[] =
{
	0x1F,+02,0x1E,+02,0x1D,+02,0x1C,+02,0x1B,+02,0x1A,+02,0x19,+02,0x18,+02,
	0x17,+02,0x16,+02,0x15,+02,0x14,+02,0x13,+02,0x12,+02,0x11,+02,0x10,+02,
	0x0F,+02,0x0E,+02,0x0D,+02,0x0C,+02,0x0B,+02,0x0A,+02,0x09,+02,0x08,+02,
	0x07,+02,0x06,+02,0x05,+02,0x04,+02,0x03,+02,0x02,+02,0x01,+02,0x00,+02,
	0x00,+02,0x01,+02,0x02,+02,0x03,+02,0x04,+02,0x05,+02,0x06,+02,0x07,+02,
	0x08,+02,0x09,+02,0x0A,+02,0x0B,+02,0x0C,+02,0x0D,+02,0x0E,+02,0x0F,+02,
	0x10,+02,0x11,+02,0x12,+02,0x13,+02,0x14,+02,0x15,+02,0x16,+02,0x17,+02,
	0x18,+02,0x19,+02,0x1A,+02,0x1B,+02,0x1C,+02,0x1D,+02,0x1E,+02,0x1F,-126,
};
static int8 env_xpulse[] =
{
	0x00,+02,0x01,+02,0x02,+02,0x03,+02,0x04,+02,0x05,+02,0x06,+02,0x07,+02,
	0x08,+02,0x09,+02,0x0A,+02,0x0B,+02,0x0C,+02,0x0D,+02,0x0E,+02,0x0F,+02,
	0x10,+02,0x11,+02,0x12,+02,0x13,+02,0x14,+02,0x15,+02,0x16,+02,0x17,+02,
	0x18,+02,0x19,+02,0x1A,+02,0x1B,+02,0x1C,+02,0x1D,+02,0x1E,+02,0x1F,+00,
};
static int8 env_xpulse_hold[] =
{
	0x00,+02,0x01,+02,0x02,+02,0x03,+02,0x04,+02,0x05,+02,0x06,+02,0x07,+02,
	0x08,+02,0x09,+02,0x0A,+02,0x0B,+02,0x0C,+02,0x0D,+02,0x0E,+02,0x0F,+02,
	0x10,+02,0x11,+02,0x12,+02,0x13,+02,0x14,+02,0x15,+02,0x16,+02,0x17,+02,
	0x18,+02,0x19,+02,0x1A,+02,0x1B,+02,0x1C,+02,0x1D,+02,0x1E,+02,0x1F,+02,
	0x00,+00,
};
static int8 env_xsaw[] =
{
	0x00,+02,0x01,+02,0x02,+02,0x03,+02,0x04,+02,0x05,+02,0x06,+02,0x07,+02,
	0x08,+02,0x09,+02,0x0A,+02,0x0B,+02,0x0C,+02,0x0D,+02,0x0E,+02,0x0F,+02,
	0x10,+02,0x11,+02,0x12,+02,0x13,+02,0x14,+02,0x15,+02,0x16,+02,0x17,+02,
	0x18,+02,0x19,+02,0x1A,+02,0x1B,+02,0x1C,+02,0x1D,+02,0x1E,+02,0x1F,-62,
};

static int8 env_xtri[] =
{
	0x00,+02,0x01,+02,0x02,+02,0x03,+02,0x04,+02,0x05,+02,0x06,+02,0x07,+02,
	0x08,+02,0x09,+02,0x0A,+02,0x0B,+02,0x0C,+02,0x0D,+02,0x0E,+02,0x0F,+02,
	0x10,+02,0x11,+02,0x12,+02,0x13,+02,0x14,+02,0x15,+02,0x16,+02,0x17,+02,
	0x18,+02,0x19,+02,0x1A,+02,0x1B,+02,0x1C,+02,0x1D,+02,0x1E,+02,0x1F,+02,
	0x1F,+02,0x1E,+02,0x1D,+02,0x1C,+02,0x1B,+02,0x1A,+02,0x19,+02,0x18,+02,
	0x17,+02,0x16,+02,0x15,+02,0x14,+02,0x13,+02,0x12,+02,0x11,+02,0x10,+02,
	0x0F,+02,0x0E,+02,0x0D,+02,0x0C,+02,0x0B,+02,0x0A,+02,0x09,+02,0x08,+02,
	0x07,+02,0x06,+02,0x05,+02,0x04,+02,0x03,+02,0x02,+02,0x01,+02,0x00,-126,
};

static int8 *(env_table[16]) =
{
	env_pulse,	env_pulse,	env_pulse,	env_pulse,
	env_xpulse,	env_xpulse,	env_xpulse,	env_xpulse,
	env_saw,	env_pulse,	env_tri,	env_pulse_hold,
	env_xsaw,	env_xpulse,	env_xtri,	env_xpulse_hold,
};

static uint32 PSGSoundNoise(PSG_NOISE *ch)
{
	if (ch->update)
	{
		if (ch->update & 1)
		{
			ch->spd = ((ch->regs[0] & 0x1F) + 1) << 18;
		}
		ch->update = 0;
	}

	if (ch->spd)
	{
		ch->cycles -= ch->cps;
		while (ch->cycles < 0)
		{
			ch->cycles += ch->spd;
			if ((ch->noiserng + 1) & 2) ch->noiseout = ~ch->noiseout;
			if (ch->noiserng & 1) ch->noiserng ^= 0x28000;
			ch->noiserng >>= 1;
		}
	}
	return ch->noiseout;
}

static uint32 PSGSoundEnvelope(PSG_ENVELOPE *ch)
{
	if (ch->update)
	{
		if (ch->update & (1 | 2))
		{
			ch->spd = ((ch->regs[1] << 8) + ch->regs[0] + 1) << 18;
		}
		if (ch->update & 4)
		{
			ch->adr = env_table[ch->regs[2] & 0xF];
		}
		ch->update = 0;
	}

	if (ch->spd)
	{
		ch->cycles -= ch->cps;
		while (ch->cycles < 0)
		{
			ch->cycles += ch->spd;
			ch->adr += ch->adr[1];
		}
		if (ch->adr[0] & 0x1F)
		{
			ch->envout = (0x1F - (ch->adr[0] & 0x1F)) << (LOG_BITS - 2 + 1);
			ch->envout += psg_sound.mastervolume;
			ch->envout = LogToLinear(ch->envout, LOG_LIN_BITS - 21);
		}
		else
			ch->envout = 0;
	}
	return ch->envout;
}

static int32 PSGSoundSquare(PSG_SQUARE *ch)
{
	int32 output = 0;
	if (ch->update)
	{
		if (ch->update & (1 | 2))
		{
			ch->spd = (((ch->regs[1] & 0x0F) << 8) + ch->regs[0] + 1) << 18;
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

	if (!ch->mute && ch->key)
	{
		uint32 volume;
		if (ch->regs[2] & 0x10)
			volume = psg_sound.envelope.envout;
		else if (ch->regs[2] & 0xF)
		{
			volume = (0xF - (ch->regs[2] & 0xF)) << (LOG_BITS - 1 + 1);
			volume += psg_sound.mastervolume;
			volume = LogToLinear(volume, LOG_LIN_BITS - 21);
		}
		else
			volume = 0;
		if (ch->key & 1)
		{
			if (ch->adr & 1)
				output += volume;
			else
				output -= volume;
		}
		if (ch->key & 2)
		{
			if (psg_sound.noise.noiseout)
				output += volume;
			else
				output -= volume;
		}
	}
	return output;
}

static int32 PSGSoundRender(void)
{
	int32 accum = 0;
	PSGSoundNoise(&psg_sound.noise);
	PSGSoundEnvelope(&psg_sound.envelope);
	accum += PSGSoundSquare(&psg_sound.square[0]);
	accum += PSGSoundSquare(&psg_sound.square[1]);
	accum += PSGSoundSquare(&psg_sound.square[2]);
	return accum >> 8;
}

static void PSGSoundVolume(uint32 volume)
{
	psg_sound.mastervolume = (volume << (LOG_BITS - 8)) << 1;
}

static void PSGSoundWrireAddr(uint32 address, uint8 value)
{
	psg_sound.adr = value;
}

static void PSGSoundWrireData(uint32 address, uint8 value)
{
	switch (psg_sound.adr)
	{
		case 0x0: case 0x1:
		case 0x2: case 0x3:
		case 0x4: case 0x5:
			psg_sound.square[psg_sound.adr >> 1].regs[psg_sound.adr & 1] = value;
			psg_sound.square[psg_sound.adr >> 1].update |= 1 << (psg_sound.adr & 1);
			break;
		case 0x6:
			psg_sound.noise.regs[0] = value;
			psg_sound.noise.update |= 1 << 0;
			break;
		case 0x7:
			{
				uint8 ch;
				for (ch = 0; ch < 3; ch++)
				{
					psg_sound.square[ch].key = 0;
					if (!(value & (1 << ch))) psg_sound.square[ch].key |= 1;
					if (!(value & (8 << ch))) psg_sound.square[ch].key |= 2;
				}
			}
			break;
		case 0x8: case 0x9: case 0xA:
			psg_sound.square[psg_sound.adr & 3].regs[2] = value;
			psg_sound.square[psg_sound.adr & 3].update |= 1 << 2;
			break;
		case 0xB: case 0xC: case 0xD:
			psg_sound.envelope.regs[psg_sound.adr - 0xB] = value;
			psg_sound.envelope.update |= 1 << (psg_sound.adr - 0xB);
			break;
	}
}

static void PSGSoundWrite(uint32 address, uint8 value)
{
	if (address == 0xC000)
	{
		PSGSoundWrireAddr(address, value);
	}
	else if (address == 0xE000)
	{
		PSGSoundWrireData(address, value);
	}
}

static void PSGSoundSquareReset(PSG_SQUARE *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 6 * 8 * 4 * SAMPLE_RATE, 18);
}

static void PSGSoundNoiseReset(PSG_NOISE *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 6 * 8 * 4 * SAMPLE_RATE, 18);
	ch->noiserng = 1;
	ch->noiseout = 0xFF;
}

static void PSGSoundEnvelopeReset(PSG_ENVELOPE *ch)
{
	ch->cps = DivFix(NES_BASECYCLES, 6 * 8 * 4 * SAMPLE_RATE, 18);
	ch->adr = env_table[0];
}

static void PSGSoundReset(void)
{
	_memset(&psg_sound, 0, sizeof(PSGSOUND));
	PSGSoundNoiseReset(&psg_sound.noise);
	PSGSoundEnvelopeReset(&psg_sound.envelope);
	PSGSoundSquareReset(&psg_sound.square[0]);
	PSGSoundSquareReset(&psg_sound.square[1]);
	PSGSoundSquareReset(&psg_sound.square[2]);
}

static int PSGSoundInit(void){
	PSGSoundReset();
	PSGSoundVolume(1);
	return 0;
}


static apu_memwrite fme7_memwrite[] = {
   { 0xC000, 0xC000, PSGSoundWrite },
   { 0xE000, 0xE000, PSGSoundWrite },
   {     -1,     -1, NULL }
};

apuext_t fme7_ext = {
	PSGSoundInit,		/* init */
	NULL,		/* shutdown */
	PSGSoundReset,	/* reset */
	NULL,		/* paramschanged */
	PSGSoundRender,	/* process */
	NULL,			/* mem_read */
	fme7_memwrite,	/* mem_write */
	NULL			/* mem_writesync */
};

