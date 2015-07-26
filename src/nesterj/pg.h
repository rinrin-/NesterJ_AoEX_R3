// primitive graphics for Hello World PSP
#ifndef PG_H_
#define PG_H_

#include "syscall.h"

#define RGB(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)

extern u32 new_pad, now_pad, old_pad;
//extern SceCtrlData paddata;

// 追加キー割り当て(適当に振ったのでかぶったら変更してね)
#define PSP_CTRL_A_UP    0x10000000 // UP(analog)
#define PSP_CTRL_A_RIGHT 0x20000000 // RIGHT(analog)
#define PSP_CTRL_A_DOWN  0x40000000 // DOWN(analog)
#define PSP_CTRL_A_LEFT  0x80000000 // LEFT(analog)

#define PSP_CTRL_AD_UP    (PSP_CTRL_A_UP | PSP_CTRL_UP)
#define PSP_CTRL_AD_DOWN  (PSP_CTRL_A_DOWN | PSP_CTRL_DOWN)
#define PSP_CTRL_AD_LEFT  (PSP_CTRL_A_LEFT | PSP_CTRL_LEFT)
#define PSP_CTRL_AD_RIGHT (PSP_CTRL_A_RIGHT | PSP_CTRL_RIGHT)


#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 272
#define SCREEN_PITCH  512
#define FRAME_SIZE	0x44000			//in byte
#define		PIXELSIZE	1				//in short
#define		LINESIZE	512				//in short
#define		FRAMESIZE	0x44000			//in byte

// sound
#define PGA_CHANNELS 1	//3
#define PGA_SAMPLES 512	//256


//480*272 = 60*38
#define CMAX_X 60
#define CMAX_Y 34
#define CMAX2_X 30
#define CMAX2_Y 17
#define CMAX4_X 15
#define CMAX4_Y 8

#define MAX_PATH 512		//temp, not confirmed

void pgInit();
void pgWaitV();
void pgWaitVn(unsigned long count);
void pgScreenFrame(long mode,long frame);
void pgScreenFlip();
void pgScreenFlipV();
void pgScreenFlipV_fast();
void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgPrint2(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgPrint4(unsigned long x,unsigned long y,unsigned long color,const char *str);
void pgFillvram(unsigned long color);

void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d);
void pgBitBltAlpha75(unsigned long x,unsigned long y,unsigned long w,unsigned long h,const unsigned short *d);

void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag);
void pgDrawFrame(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color);
void pgFillBox(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color);
void mh_print(int x,int y,const char *str,int col);
unsigned char *pgGetVramAddr(unsigned long x,unsigned long y);


void pgMain(int args, char *argp[]);

void readpad(void);

void pgcPuthex2(const unsigned long s);
void pgcPuthex8(const unsigned long s);


void pgaSetChannelCallback(void *callback);
int pgaInit(void);

//optimize

//long配列をコピー。配列境界は4バイトアラインされている必要あり
static inline void __memcpy4(void *d, void *s, unsigned long c)
{
	for (; c>0; --c) {
		*(unsigned long *)d=*(unsigned long *)s;
		d+=4;
		s+=4;
	}
}


//long配列にセット。配列境界は4バイトアラインされている必要あり
static inline void __memset4(void *d, unsigned long v, unsigned long c)
{
	for (; c>0; --c) {
		*(unsigned long *)d = v;
		d+=4;
	}
}


//long配列をコピー。配列境界は4バイトアラインされている必要あり
//コンパイラの最適化によって予期しないコードが生成されるため、十分に注意のこと。
static inline void __memcpy4aa(void *d, void *s, unsigned long c)
{
	unsigned long wk0,wk1,wk2,wk3;

	asm volatile (
		"		.set	push"				"\n"
		"		.set	noreorder"			"\n"

		"		move	%1,%4 "				"\n"
		"		move	%2,%5 "				"\n"
		"		move	%3,%6 "				"\n"

		"1:		lw		%0,0(%2) "			"\n"
		"		addiu	%3,%3,-1 "			"\n"
		"		addiu	%2,%2,4 "			"\n"
		"		sw		%0,0(%1) "			"\n"
		"		bnez	%3,1b "				"\n"
		"		addiu	%1,%1,4 "			"\n"

		"		.set	pop"				"\n"

			:	"=&r" (wk0),	// %0
				"=&r" (wk1),	// %1
				"=&r" (wk2),	// %2
				"=&r" (wk3)		// %3
			:	"r" (d),		// %4
				"r" (s),		// %5
				"r" (c)			// %6
	);
}

//long配列をコピー。配列境界は4バイトアラインされている必要あり
//s,dは参照渡し扱いになるので、リターン後は変更されていると考えたほうが良い
//コンパイラの最適化によって予期しないコードが生成されるため、十分に注意のこと。__memcpy4のほうが安全。
//といいますかcで書いても全然変わらないような。ｶﾞﾝｶﾞｯﾀのに。
static inline void __memcpy4a(unsigned long *d, unsigned long *s, unsigned long c)
{
	unsigned long wk,counter;

	asm volatile (
		"		.set	push"				"\n"
		"		.set	noreorder"			"\n"

		"		move	%1,%4 "				"\n"
		"1:		lw		%0,0(%3) "			"\n"
		"		addiu	%1,%1,-1 "			"\n"
		"		addiu	%3,%3,4 "			"\n"
		"		sw		%0,0(%2) "			"\n"
		"		bnez	%1,1b "				"\n"
		"		addiu	%2,%2,4 "			"\n"

		"		.set	pop"				"\n"

			:	"=&r" (wk),		// %0
				"=&r" (counter)	// %1
			:	"r" (d),		// %2
				"r" (s),		// %3
				"r" (c)			// %4
	);
}


#define __USE_MIPS32R2__

static inline void cpy2x(unsigned long *d, unsigned long cc)
{
#ifdef __USE_MIPS32R2__
	unsigned long wk0;
	asm volatile (
		"		.set	push"				"\n"
		"		.set	noreorder"			"\n"

		"		.set	mips32r2"			"\n"

		"		srl		%0,%2,16"			"\n"
		"		ins 	%2,%2,16,16"		"\n"
		"		sw		%2,0(%1)"			"\n"
		"		ins 	%0,%0,16,16"		"\n"
		"		sw		%0,4(%1)"			"\n"

		"		.set	pop"				"\n"

			:	"=&r" (wk0)		// %0
			:	"r" (d),		// %1
				"r" (cc)		// %2
	);
#else
	*d      = (cc&0x0000ffff)|(cc<<16);
	*(d+1)  = (cc>>16)|(cc&0xffff0000);
#endif
}

static inline void cpy3x(unsigned long *d, unsigned long cc)
{
#ifdef __USE_MIPS32R2__
	unsigned long wk0;
	asm volatile (
		"		.set	push"				"\n"
		"		.set	noreorder"			"\n"

		"		.set	mips32r2"			"\n"

		"		sw		%2,4(%1)"			"\n"
		"		srl		%0,%2,16"			"\n"
		"		ins 	%2,%2,16,16"		"\n"
		"		sw		%2,0(%1)"			"\n"
		"		ins 	%0,%0,16,16"		"\n"
		"		sw		%0,8(%1)"			"\n"

		"		.set	pop"				"\n"

			:	"=&r" (wk0)		// %0
			:	"r" (d),		// %1
				"r" (cc)		// %2
	);
#else
	*d      = (cc&0x0000ffff)|(cc<<16);
	*(d+1)  = cc;
	*(d+2)  = (cc>>16)|(cc&0xffff0000);
#endif
}

#endif
