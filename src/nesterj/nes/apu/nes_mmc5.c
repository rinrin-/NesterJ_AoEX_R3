/*
** Nintendo MMC3 ExSound by TAKEDA, toshiya
**
** original: s_apu.c in nezp0922
*/
#include "nes_apu.h"
#include "../nes_string.h"
extern apu_t g_apu_t;

// ----------------------------------------------------------------------------
// MMC5 Sound struct

typedef struct {
	uint32 cps;
	int32 cycles;
	int32 sweepphase;
	int32 envphase;

	uint32 spd;
	uint32 envspd;
	uint32 sweepspd;

	uint32 length;
	uint32 freq;
	uint32 mastervolume;
	uint32 release;

	uint8 regs[4];
	uint8 update;
	uint8 key;
	uint8 adr;
	uint8 envadr;
	uint8 duty;
	uint8 mute;
} MMC5_SQUARE;

typedef struct {
	int32 output;
	uint8 key;
	uint8 mute;
} MMC5_DA;

typedef struct {
	MMC5_SQUARE square[2];
	MMC5_DA da;
} MMC5SOUND;


static MMC5SOUND mmc5_sound;


#define V(x) (x*64/60)
static uint32 mmc5_vbl_length[32] =
{
	V(0x05), V(0x7F), V(0x0A), V(0x01), V(0x14), V(0x02), V(0x28), V(0x03),
	V(0x50), V(0x04), V(0x1E), V(0x05), V(0x07), V(0x06), V(0x0E), V(0x07),
	V(0x06), V(0x08), V(0x0C), V(0x09), V(0x18), V(0x0A), V(0x30), V(0x0B),
	V(0x60), V(0x0C), V(0x24), V(0x0D), V(0x08), V(0x0E), V(0x10), V(0x0F),
};
#undef V

#define V(x) ((x) << 19)
static const uint32 mmc5_spd_limit[8] =
{
	V(0x3FF), V(0x555), V(0x666), V(0x71C),
	V(0x787), V(0x7C1), V(0x7E0), V(0x7F0),
};
#undef V

static int32 MMC5SoundSquareRender(MMC5_SQUARE *ch)
{
	uint32 output;
	if (ch->update)
	{
		if (ch->update & 1)
		{
			ch->duty = (ch->regs[0] >> 4) & 0x0C;
			if (ch->duty == 0) ch->duty = 2;
			ch->envspd = ((ch->regs[0] & 0x0F) + 1) << (19 + 7);
		}
		if (ch->update & 2)
		{
			ch->sweepspd = (((ch->regs[1] >> 4) & 0x07) + 1) << (19 + 8);
		}
		if (ch->update & (4 | 8))
		{
			ch->spd = (((ch->regs[3] & 7) << 8) + ch->regs[2] + 1) << 19;
		}
		if ((ch->update & 8) && (ch->key & 1))
		{
			ch->key &= ~2;
			ch->length = (mmc5_vbl_length[ch->regs[3] >> 3] * ch->freq) >> 6;
			ch->envadr = 0;
		}
		ch->update = 0;
	}

	if (ch->key == 0) return 0;

	ch->envphase -= ch->cps >> (13 - 7);
	if (ch->regs[0] & 0x20)
	{
		while (ch->envphase < 0)
		{
			ch->envphase += ch->envspd;
			ch->envadr++;
		}
		ch->envadr &= 0x0F;
	}
	else
	{
		while (ch->envphase < 0)
		{
			ch->envphase += ch->envspd;
			ch->envadr += (ch->envadr < 15);
		}
	}

	if (ch->length)
	{
		if (!(ch->regs[0] & 0x20)) ch->length--;
	}
	else
	{
		ch->key |= 2;
	}

	if ((ch->regs[1] & 0x80))
	{
		if (ch->regs[1] & 7)
		{
			ch->sweepphase -= ch->cps >> (14 - 8);
			if (ch->regs[1] & 8)
				while (ch->sweepphase < 0)
				{
					ch->sweepphase += ch->sweepspd;
					ch->spd -= ch->spd >> (ch->regs[1] & 7);
				}
			else
				while (ch->sweepphase < 0)
				{
					ch->sweepphase += ch->sweepspd;
					ch->spd += ch->spd >> (ch->regs[1] & 7);
				}
		}
	}

	if (ch->spd < (4 << 19)) return 0;
	if (!(ch->regs[1] & 8))
	{
		if (ch->spd > mmc5_spd_limit[ch->regs[1] & 7]) return 0;
	}

	ch->cycles -= ch->cps;
	while (ch->cycles < 0)
	{
		ch->cycles += ch->spd;
		ch->adr++;
	}
	ch->adr &= 0x0F;

	if (ch->key & 2)
	{
		if (ch->release < (31 << (LOG_BITS + 1)))
			ch->release += 3 << (LOG_BITS - 8 + 1);
	}
	else
	{
		ch->release = 0;
	}

	if (ch->mute) return 0;

	if (ch->regs[0] & 0x10) /* fixed volume */
		output = ch->regs[0] & 0x0F;
	else
		output = 15 - ch->envadr;

	output = LinearToLog(output) + ch->mastervolume + ch->release;
	output += (ch->adr < ch->duty);
	return LogToLinear(output, LOG_LIN_BITS - LIN_BITS - 16);
}

static void MMC5SoundSquareReset(MMC5_SQUARE *ch)
{
	_memset(ch, 0, sizeof(MMC5_SQUARE));
	ch->freq = SAMPLE_RATE;
	ch->cps = DivFix(NES_BASECYCLES, 12 * ch->freq, 19);
}

static int32 MMC5SoundRender(void)
{
	int32 accum = 0;
	accum += MMC5SoundSquareRender(&mmc5_sound.square[0]);
	accum += MMC5SoundSquareRender(&mmc5_sound.square[1]);
	return accum >> 8;
}

static void MMC5SoundVolume(uint32 volume)
{
	volume = (volume << (LOG_BITS - 8)) << 1;
	mmc5_sound.square[0].mastervolume = volume;
	mmc5_sound.square[1].mastervolume = volume;
}

static void MMC5SoundWrite(uint32 address, uint8 value)
{
	if (0x5000 <= address && address <= 0x5015)
	{
		switch (address)
		{
			case 0x5000: case 0x5002: case 0x5003:
			case 0x5004: case 0x5006: case 0x5007:
				{
					int ch = address >= 0x5004;
					int port = address & 3;
					mmc5_sound.square[ch].regs[port] = value;
					mmc5_sound.square[ch].update |= 1 << port;
				}
				break;
			case 0x5011:
				mmc5_sound.da.output = ((int32)value) - 0x80;
				break;
			case 0x5010:
				mmc5_sound.da.key = (value & 0x01);
				break;
			case 0x5015:
				if (value & 1)
					mmc5_sound.square[0].key = 1;
				else
				{
					mmc5_sound.square[0].key = 0;
					mmc5_sound.square[0].length = 0;
				}
				if (value & 2)
					mmc5_sound.square[1].key = 1;
				else
				{
					mmc5_sound.square[1].key = 0;
					mmc5_sound.square[1].length = 0;
				}
				break;
		}
	}
}

static void MMC5SoundDaReset(MMC5_DA *ch)
{
	_memset(ch, 0, sizeof(MMC5_DA));
}

static void MMC5SoundReset(void)
{
	MMC5SoundSquareReset(&mmc5_sound.square[0]);
	MMC5SoundSquareReset(&mmc5_sound.square[1]);
	MMC5SoundDaReset(&mmc5_sound.da);

	MMC5SoundWrite(0x5000, 0x00);
	MMC5SoundWrite(0x5002, 0x00);
	MMC5SoundWrite(0x5003, 0x00);
	MMC5SoundWrite(0x5004, 0x00);
	MMC5SoundWrite(0x5006, 0x00);
	MMC5SoundWrite(0x5007, 0x00);
	MMC5SoundWrite(0x5010, 0x00);
	MMC5SoundWrite(0x5011, 0x00);
}

static int MMC5SoundInit(void){
	MMC5SoundReset();
	MMC5SoundVolume(1);
	return 0;
}

static apu_memwrite mmc5_memwrite[] = {
   { 0x5000, 0x5015, MMC5SoundWrite },
   {     -1,     -1, NULL }
};

apuext_t mmc5_ext = {
	MMC5SoundInit,		/* init */
	NULL,		/* shutdown */
	MMC5SoundReset,	/* reset */
	NULL,		/* paramschanged */
	MMC5SoundRender,	/* process */
	NULL,			/* mem_read */
	mmc5_memwrite,	/* mem_write */
	NULL			/* mem_writesync */
};

