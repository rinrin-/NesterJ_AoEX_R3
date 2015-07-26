//
// NES emu と PSPの架け橋(スクリーン編)
//
#include <pspkernel.h>
#include <pspgu.h>
#include "screenmanager.h"
#include "nes/nes.h"
#include "debug/debug.h"
#include "main.h"
#include "pg.h"
#include "image.h"

uint16 __attribute__((aligned(16))) g_Pal[256];	// palette index (using 0-63)
//uint8 (*g_ScrBuf)[512] = (uint8(*)[512])0x04100000; // Screen palette index buffer
uint8 (*g_vram)[512] = (uint8(*)[512])0x04100000; //davex: mirror memory
uint8 g_ScrBuf[512][512]; //davex:normal buffer

//>>>davex
const int MAXX = 256 +16;
const int MAXY = 224 + 8;
int g_mirror = 0;
int screen_rotated_on = 0;
//<<<

#define VRAM_PITCH 512

//>>>davex: function for mirroring normal buffer to vram
void scrbuf_2_mirror_vram(){

 	int i, j;
 	byte *p, *q;

    for( j=0; j< MAXY; j++){
        p = &(g_ScrBuf[j][0]);
        q = &(g_vram[j][ (MAXX-1)]);
        for( i = 0; i< MAXX; i++){
            *(q--) = *(p++);
        }
    }
}//<<<


// パレット通知 pal[NES_MAX_COLORS][3]
void Scr_AssertPalette(uint8 pal[][3])
{
	int cbI;
	DEBUG("Scr_AssertPalette");
	_memset(g_Pal, 0x00, sizeof(g_Pal));
	for (cbI = 0; cbI < NES_NUM_COLORS; cbI++) {
		g_Pal[NES_COLOR_BASE + cbI] = RGB(pal[cbI][0], pal[cbI][1], pal[cbI][2]);
	}
}

// 画面モード PAL or NTSC
void Scr_SetScreenMode(const unsigned char ScreenMode)
{
}

// 指定した色で塗りつぶし
void Scr_ClearScreen(const uint8 PalNum)
{
	DEBUG("Scr_ClearScreen");
	//_memset(g_ScrBuf, PalNum, sizeof(g_ScrBuf)*512);
	_memset(g_ScrBuf, PalNum, sizeof(&g_ScrBuf)*512);//davex: changed pointer sintax
}

char g_szRenderMsg[80];
int g_nRenderMsgLen;
int g_nRenderMsgCount = 0;
int g_nRenderMsgCounter = 0;
int g_nColor = 0xFFFF;
// 画面にメッセージ表示用
void Scr_SetMessage(const char *pszMsg, int nTimer, int nColor)
{
	_strncpy(g_szRenderMsg, pszMsg, sizeof(g_szRenderMsg) -1);
	g_szRenderMsg[79] = '\0';
	g_nRenderMsgLen=_strlen(g_szRenderMsg);
	g_nRenderMsgCount = nTimer;
	g_nRenderMsgCounter = nTimer;
	g_nColor = nColor;
}

// internal blt function - prototypes
void BitBlt_NormalScreen(void);
void BitBlt_FullScreen(void);
void BitBlt_WideScreen(void);
void BitBlt_Wide_X15(void);
void BitBlt_X15(void);
void BitBlt_X15_CROPPED(void);

void BitBlt_GPU(int width,int height, uint32 bCropped);

// 画面へ転送
// 新規モード追加時は上にプロトタイプ宣言し、Scr_BltScreenのswitch内に追加、
// 下のBltScreen internal functions のところに処理を書く
// screenmanager.h のenumとaszScreenNameに追加すればメニュー対応完了

void Scr_BltScreen()
{
    extern int isLate;//from emu_main.c

	if(g_nRenderMsgCounter==0 || g_nRenderMsgCounter==1 || g_nRenderMsgCounter==2 || g_nRenderMsgCounter == (g_nRenderMsgCount-2) || g_nRenderMsgCounter == (g_nRenderMsgCount-1) || g_nRenderMsgCounter==g_nRenderMsgCount) pgFillBox(0,263,479,271,0);

	//>>>davex
	g_mirror = 0;
	screen_rotated_on = 0;
	//<<<

	// パレットとバッファから画面へ描画処理をする
	switch(setting.screenmode) {
	case SCREEN_GPU_FULL:			BitBlt_GPU(480,272, FALSE);	break;;
	case SCREEN_GPU_FULL_CROPPED:	BitBlt_GPU(480,272, TRUE);	break;
	case SCREEN_GPU_X15:			BitBlt_GPU(363,272, FALSE);	break;
	case SCREEN_GPU_NORMAL: 		BitBlt_GPU(256,224, FALSE);	break;
	case SCREEN_FULL:				BitBlt_FullScreen();		break;
	//>>>nibbles27
	case SCREEN_WIDE:				BitBlt_WideScreen();		break;
	case SCREEN_WIDE_X15:			BitBlt_Wide_X15();			break;
	//<<<
	case SCREEN_X15:				BitBlt_X15();				break;
	case SCREEN_X15_CROPPED:		BitBlt_X15_CROPPED();		break;
	//>>>davex
	case SCREEN_GPU_X15_ROTATED:			{screen_rotated_on = 1; BitBlt_GPU(363,272, FALSE);	}	break;
	case SCREEN_GPU_FULL_MIRROR:			{g_mirror=1; BitBlt_GPU(480,272, FALSE); }	break;
	case SCREEN_GPU_FULL_CROPPED_MIRROR:	{g_mirror=1; BitBlt_GPU(480,272, TRUE);  }	break;
	case SCREEN_GPU_X15_MIRROR:				{g_mirror=1; BitBlt_GPU(363,272, FALSE); }	break;
	case SCREEN_GPU_NORMAL_MIRROR: 			{g_mirror=1; BitBlt_GPU(256,224, FALSE); }	break;
	//<<<
	default:						BitBlt_NormalScreen();		break; // SCREEN_NORMAL
	}

	// draw message
	if(g_nRenderMsgCounter>=3){
		pgFillBox(0,263,g_nRenderMsgLen*8,271,0);
		pgPrint(0,33,g_nColor,g_szRenderMsg);
	}
	if(g_nRenderMsgCounter>-1) g_nRenderMsgCounter--;
	// draw fps
	if (setting.showfps) {
		extern uint32 g_ulFPS;	// emu_main.c
		char szFPS[32];
		_itoa(g_ulFPS, szFPS);
		_strcat(szFPS, "FPS");
		pgFillBox(0,0,_strlen(szFPS)*8,8,0);
		pgPrint(0,0,0xffff,szFPS);
	}
	if (setting.vsync==VSYNC_ALWAYS || (setting.vsync==VSYNC_SOMETIMES && !isLate)) {
        pgScreenFlipV_fast();
	}
	else {
		pgScreenFlip();
	}
}


// 描画ロック
boolean Scr_Lock(pixmap *p)
{
	p->data = &g_ScrBuf[0][0];
	return TRUE;
}

// 描画アンロック
void Scr_Unlock(void)
{
}







/////////////////////////////////////////////////
// BltScreen internal functions

// Parallel blend 1:1で合成
static inline unsigned long PBlend(unsigned long c0, unsigned long c1)
{
	return (c0 & c1) + (((c0 ^ c1) & 0x7bde7bde) >> 1);
}

// Parallel blend 3:1で合成
static inline unsigned long PBlend_3to1(unsigned long d0, unsigned long d1){
	return PBlend( PBlend(d0,d1), d0 );
}

#ifdef __USE_MIPS32R2__

static inline void _asm_Blt_Normal(unsigned long *pVram, unsigned long *src)
{
	unsigned long x, y;
	unsigned long t0, t1, t2, t3;
	const unsigned long mask = 0x3f3f3f3f;
	y = 224;

	asm volatile (
		"	.set	push"				"\n"
		"	.set	noreorder"			"\n"

		"	.set	mips32r2"			"\n"

		//{
		"1:	li		%4, 64"				"\n"	// x=NES_SCREEN_WIDTH/4
		"	addiu	%5, -1"				"\n"	// y--
			//{
		"2:		lw		%3, 0(%7)"			"\n"	// t3=*src
		"		addiu	%4, -1"				"\n"	// x--
		"		addiu	%7, 4"				"\n"	// src++

		"		and		%3, %9"				"\n"	// t3 &= 0x3f3f3f3f
		"		sll		%3, 1"				"\n"	// t3 *= 2

		"		ext		%0, %3, 0, 7"		"\n"	// t0 =  t3&0x7f
		"		addu	%0, %8"				"\n"	// t0 += g_Pal
		"		lhu		%0, 0(%0)"			"\n"	// t0 = |0|A|

		"		ext		%2, %3, 8, 7"		"\n"	// t2 =  (t3>>8)&0x7f
		"		addu	%2, %8"				"\n"	// t2 += g_Pal
		"		lhu		%2, 0(%2)"			"\n"	// t2 = |0|B|

		"		ext		%1, %3, 16, 7"		"\n"	// t1 =  (t3>>16)&0x7f
		"		addu	%1, %8"				"\n"	// t1 += g_Pal
		"		lhu		%1, 0(%1)"			"\n"	// t1 = |0|C|

		"		srl 	%3, 24"				"\n"	// t3 =  (t3>>24)&0x7f
		"		addu 	%3, %8"				"\n"	// t3 += g_Pal
		"		lhu 	%3, 0(%3)"			"\n"	// t3 = |0|D|

		"		ins 	%0, %2, 16, 16"		"\n"	// t0 = |B|A|
		"		sw		%0, 0(%6) "			"\n"	// *pVram = t0

		"		ins		%1, %3, 16, 16"		"\n"	// t1 = |D|C|
		"		sw		%1, 4(%6) "			"\n"	// *(pVram+1) = t1

		"		bnez	%4, 2b "			"\n"
		"		addiu	%6, 8 "				"\n"	// [delay slot] pVram += 2
			//}

		"	addiu	%7, 256"				"\n"	// src+=512 - NES_SCREEN_WIDTH

		"	bnez	%5, 1b "			"\n"
		"	addiu	%6, 512 "			"\n"	// [delay slot] pVram += (LINESIZE-NES_SCREEN_WIDTH)*2/sizeof(long)
		//}

		"	.set	pop"				"\n"

			:	"=&r" (t0),		// %0
				"=&r" (t1),		// %1
				"=&r" (t2),		// %2
				"=&r" (t3),		// %3
				"=&r" (x)		// %4
			:	"r" (y),		// %5
				"r" (pVram),	// %6
				"r" (src),		// %7
				"r" (g_Pal),	// %8
				"r" (mask)		// %9
			:	"memory"
	);
}
#endif //__USE_MIPS32R2__

void BitBlt_NormalScreen(void)
{
#ifndef NESTER_DEBUG
#ifdef __USE_MIPS32R2__

	unsigned long *pVram, *src;

	pVram = (unsigned long *)pgGetVramAddr(112, 30);
	src = (unsigned long*)&g_ScrBuf[8][8];

	_asm_Blt_Normal( pVram, src );

#else

	int x,y;
	unsigned short *pVram;
	pVram = (unsigned short *)pgGetVramAddr(112, 30);
	for (y = 0; y < 224; y++) {
		for (x = 0; x < 256; x++) {
			pVram[x] = g_Pal[g_ScrBuf[y + 8][x + 8]];
		}
		pVram += LINESIZE;
	}

#endif //__USE_MIPS32R2__
#else

	// debug用 仮想画面全部Render
	int x,y;
	unsigned short *pVram;
	pVram = (unsigned short *)pgGetVramAddr(0, 0);
	for (y = 0; y < 272; y++) {
		for (x = 0; x < 480; x++) {
			pVram[x] = g_Pal[g_ScrBuf[y][x]];
		}
		pVram += LINESIZE;
	}

#endif // NESTER_DEBUG

}

/******************************************************************************/

typedef struct Vertex16 {
    unsigned short u, v;
	short x, y, z;
} Vertex16;


//-----------------------------------------------------------------------------
// GPU bitblt
//-----------------------------------------------------------------------------
static void blt_gpu(int scr_w,int scr_h,void *XBuf,int gfx_sx,int gfx_sy,int gfx_w,int gfx_h)
{
    int ScreenX,ScreenY;
    uint32   * GEcmd  = (uint32  *)0x041e0000; // 勝手にメモリ割当
    uint32   *pGEcmd  = GEcmd;
    Vertex16 * vertex = (Vertex16*)0x041f0000; // 勝手にメモリ割当

	// 画面中心に描画ポイントを合わせる
    ScreenX = (SCREEN_WIDTH -scr_w)/2;
    ScreenY = (SCREEN_HEIGHT-scr_h)/2;

	//>>>davex
	 if( screen_rotated_on){

	    vertex[1].u = gfx_sx;
	    vertex[1].v = gfx_sy;
		vertex[0].u = gfx_sx+gfx_w;
    	vertex[0].v = gfx_sy+gfx_h;

	 }else{
	//<<<davex
		vertex[0].u = gfx_sx;           // Texture展開開始位置:X
		vertex[0].v = gfx_sy;           // Texture展開終了位置:Y
		vertex[1].u = gfx_sx+gfx_w;     // Texture展開終了位置:X
		vertex[1].v = gfx_sy+gfx_h;     // Texture展開終了位置:Y

	}

    vertex[0].x = ScreenX;          // 画像展開位置:開始点 X
    vertex[0].y = ScreenY;          // 画像展開位置:開始点 Y
    vertex[1].x = ScreenX+scr_w;    // 画像展開位置:終了点 X
    vertex[1].y = ScreenY+scr_h;    // 画像展開位置:終了点 Y

    vertex[0].z = vertex[1].z = 0;

    // Set Draw Buffer
    *pGEcmd++ = 0x9C000000UL | ((u32)pgGetVramAddr(0,0) & 0x00FFFFFF);
    *pGEcmd++ = 0x9D000000UL |(((u32)pgGetVramAddr(0,0) & 0xFF000000) >> 8) | 512;

    // sceGuClutMode
    *pGEcmd++ = 0xC5000000UL | (1) | (0 << 2) | (0xff << 8) | (0 << 16);

    // sceGuClutLoad
    *pGEcmd++ = 0xB0000000UL | (((u32)g_Pal) & 0x00ffffff);
    *pGEcmd++ = 0xB1000000UL | ((((u32)g_Pal) & 0xff000000)>>8);
    *pGEcmd++ = 0xC4000000UL | (256/8); // このパラメータは適当

    // sceGuTexMode
    *pGEcmd++ = 0xC2000000UL | 0;
    *pGEcmd++ = 0xC3000000UL | 5; // GU_PSM_T8;

    // sceGuTexImage
    *pGEcmd++ = 0xA0000000UL | ((u32)XBuf & 0x00FFFFFF);
    *pGEcmd++ = 0xA8000000UL |(((u32)XBuf & 0xFF000000) >> 8) | 512;
    *pGEcmd++ = 0xB8000000UL | (0x09<<8) | (9); // HEIGHT=512(2y9) | WIDTH=512(2y9);

    // Tex Flush
    *pGEcmd++ = 0xCB000000UL;

    // sceGuTexFunc
    *pGEcmd++ = 0xC9000000UL | (1<<8) | 3  ; // | 0x10000;

    // Set Vertex
    *pGEcmd++ = 0x12000000UL | (1 << 23) | (0 << 11) | (0 << 9) | (2 << 7) | (0 << 5) | (0 << 2) | 2;
    *pGEcmd++ = 0x10000000UL;
    *pGEcmd++ = 0x02000000UL;
    *pGEcmd++ = 0x10000000UL | (((u32)vertex & 0xFF000000) >> 8);
    *pGEcmd++ = 0x01000000UL | ( (u32)vertex & 0x00FFFFFF);

    // Draw Vertex
    *pGEcmd++ = 0x04000000UL | (6 << 16) | 2;

    // List End
    *pGEcmd++ = 0x0F000000UL;
    *pGEcmd++ = 0x0C000000UL;
    *pGEcmd   = 0;

    sceKernelDcacheWritebackAll();

	sceGeListEnQueue(GEcmd,pGEcmd,-1,NULL);
    sceGeDrawSync(PSP_GE_LIST_DONE);
}



// GPU使って拡大
void BitBlt_GPU(int width,int height, uint32 bCropped)
{
	if (bCropped) {
		//>>>davex
		if( g_mirror){
			scrbuf_2_mirror_vram();
			blt_gpu(width,height,g_vram,16,8,240,224);
		}else{
		//<<<
			// 両端8ドットカット
			blt_gpu(width,height,g_ScrBuf,16,8,240,224);
		}
	}
	else {
		//>>>davex
		if( g_mirror){
			scrbuf_2_mirror_vram();
			blt_gpu(width,height,g_vram,8,8,256,224);
		}else{
		//<<<
			// ノーカット
			blt_gpu(width,height,g_ScrBuf,8,8,256,224);
		}
	}
}


//両端8ドットって拡大
void BitBlt_FullScreen(void)
{
	int x,y, dy = 0;
	unsigned long *pVram;
	pVram = (unsigned long *)pgGetVramAddr(0, 0);
	for (y = 0; y < 224; y++) {
		for (x = 0; x < 240; x+=2) {
			cpy2x(&pVram[x], (g_Pal[g_ScrBuf[y + 8][x + 16 + 1]] << 16) + g_Pal[g_ScrBuf[y + 8][x + 16]]);
		}
		pVram += (LINESIZE/2);
		dy += 21300;
		if (dy >= 100000) {
			dy-=100000;
			for (x = 0; x < 240; x+=2) {
				unsigned long ulColor = PBlend((g_Pal[g_ScrBuf[y + 8    ][x + 16 + 1]] << 16) + g_Pal[g_ScrBuf[y + 8    ][x + 16]],
									         (g_Pal[g_ScrBuf[y + 8 + 1][x + 16 + 1]] << 16) + g_Pal[g_ScrBuf[y + 8 + 1][x + 16]]);
				cpy2x(&pVram[x], ulColor);
			}
			pVram += (LINESIZE/2);
		}
	}
}

//WIDTH x2 HEIGHT x1 nibbles27
void BitBlt_WideScreen(void)
{
	int x,y, dy = 0;
	unsigned long *pVram;
	pVram = (unsigned long *)pgGetVramAddr(0, 22);
	for (y = 0; y < 224; y++) {
		for (x = 0; x < 240; x+=2) {
			cpy2x(&pVram[x], (g_Pal[g_ScrBuf[y + 8][x + 16 + 1]] << 16) + g_Pal[g_ScrBuf[y + 8][x + 16]]);
		}
		pVram += (LINESIZE/2);
	}
}


/* 4pixを1.5倍拡大して 6pixを出力
   input : |A|B|C|D| (注:メモリ上のイメージです)
   output: |A|@|B|C|#|D| ("@","#"は隣のpixを合成)*/
static inline void _X15_PIXEL(unsigned long *dist, unsigned char *src){
	unsigned long t0, t1, t2;

	t0  = g_Pal[src[0]];    // t0=|0|A| (注:レジスタ上のイメージです)
	t1  = g_Pal[src[1]];	// t1=|0|B|
	t1 |= g_Pal[src[2]]<<16;// t1=|C|B|
	t0 |= g_Pal[src[3]]<<16;// t0=|D|A|
	t2  = PBlend(t0, t1);	// t2=|#|@|
	*dist     = (t0&0xffff)|(t2<<16);       // |@|A|
	*(dist+1) = t1;                         // |C|B|
	*(dist+2) = (t2>>16)|(t0&0xffff0000);   // |D|#|
}

// x1.5 拡大
static inline unsigned char* _X15_1LINE(unsigned long *pVram, unsigned char *src){
	unsigned long x;

	for (x = NES_SCREEN_WIDTH/4; x > 0; x--) {
		_X15_PIXEL(pVram, src);
		pVram+=3;
		src+=4;
	}
	return src+2*SIDE_MARGIN;
}

/* 4lineを1.25倍拡大して 5lineを出力
input: line A,B,C,D
output:
line1 A
line2 (A + B*3)/4
line3 (B + C)/2
line4 (C*3 + D)/4
line5 D
*/
static inline void _X15_5LINE_X125(unsigned long *pVram, unsigned char *src){
	const unsigned long next=VRAM_PITCH;
	const unsigned long NVL = LINESIZE/2;/* next vram line*/
	unsigned long x;

	for(x = NES_SCREEN_WIDTH/4; x > 0; x--){
		unsigned long L0[3], L1[3];

		/* 1st line*/
		_X15_PIXEL(L0, src);
		*(pVram) 	= L0[0];
		*(pVram+1)	= L0[1];
		*(pVram+2)	= L0[2];

		/* 2nd line [Blend Ratio 3:1] */
		_X15_PIXEL(L1, src+next);
		*(pVram+NVL) 	= PBlend_3to1(L1[0], L0[0]);
		*(pVram+NVL+1)	= PBlend_3to1(L1[1], L0[1]);
		*(pVram+NVL+2)	= PBlend_3to1(L1[2], L0[2]);

		/* 3rd line [Blend Ratio 1:1] */
		_X15_PIXEL(L0, src+next*2);
		*(pVram+NVL*2) 		= PBlend(L0[0], L1[0]);
		*(pVram+NVL*2+1)	= PBlend(L0[1], L1[1]);
		*(pVram+NVL*2+2)	= PBlend(L0[2], L1[2]);

		/* 4th line [Blend Ratio 1:3] */
		_X15_PIXEL(L1, src+next*3);
		*(pVram+NVL*3)		= PBlend_3to1(L0[0], L1[0]);
		*(pVram+NVL*3+1)	= PBlend_3to1(L0[1], L1[1]);
		*(pVram+NVL*3+2)	= PBlend_3to1(L0[2], L1[2]);

		/* 5th line */
		*(pVram+NVL*4) 		= L1[0];
		*(pVram+NVL*4+1)	= L1[1];
		*(pVram+NVL*4+2)	= L1[2];
		/* END OF 5th line*/

		pVram+=3;
		src+=4;
	}
}

//WIDTH x1.5 HEIGHT x1 nibbles27
void BitBlt_Wide_X15(void)
{
	unsigned long y;
	unsigned long *pVram;
	unsigned char *src;

	pVram = (unsigned long *)pgGetVramAddr((480-256*3/2)/2, 24);
	src = (unsigned char*)&g_ScrBuf[8][8];

	for (y = 0; y < 224; y++) {

		_X15_1LINE( pVram, src);
			src   += VRAM_PITCH;
			pVram += LINESIZE/2;
	}
}

//WIDTH x1.5 HEIGHT x1.2142 224/14*(14+3)=272line
void BitBlt_X15(void)
{
	unsigned long y;
	unsigned long *pVram;
	unsigned char *src;

	pVram = (unsigned long *)pgGetVramAddr((480-256*3/2)/2, 0);
	src = (unsigned char*)&g_ScrBuf[8][8];

	for (y = 224/14; y > 0; y--) {

		_X15_5LINE_X125( pVram, src);
			src   += VRAM_PITCH*4;
			pVram += LINESIZE/2*5;

		_X15_1LINE( pVram, src);
			src   += VRAM_PITCH;
			pVram += LINESIZE/2;

		_X15_5LINE_X125( pVram, src);
			src   += VRAM_PITCH*4;
			pVram += LINESIZE/2*5;

		_X15_1LINE( pVram, src);
			src   += VRAM_PITCH;
			pVram += LINESIZE/2;

		_X15_5LINE_X125( pVram, src);
			src   += VRAM_PITCH*4;
			pVram += LINESIZE/2*5;
	}
}

//WIDTH x1.5 HEIGHT x1.25 上下3dotずつカット (224-4*2)/4*(4+1)+2=272line
void BitBlt_X15_CROPPED(void)
{
	unsigned long y;
	unsigned long *pVram;
	unsigned char *src;

	pVram = (unsigned long *)pgGetVramAddr((480-256*3/2)/2, 0);
	src = (unsigned char*)&g_ScrBuf[8+3][8];

	_X15_1LINE( pVram, src);
		src   += VRAM_PITCH;
		pVram += LINESIZE/2;

	for (y = (224-4*2)/4; y > 0; y--) {
		_X15_5LINE_X125( pVram, src);
			src   += VRAM_PITCH*4;
			pVram += LINESIZE/2*5;
	}

	_X15_1LINE( pVram, src);
}

#define THUMB_RGBMASK2 0x1CE71CE7	// = 0BBBBBGGGGGRRRRR0BBBBBGGGGGRRRRR = 00011100111001110001110011100111B

// 75%合成
void DrawCurrentScreen(int nWidth, int nHeight)
{
	int x,y;
	if (nWidth & 1) return;
	uint32 *pVram;
	uint32 color;
	pVram = (uint32 *)pgGetVramAddr(nWidth, nHeight);
	for (y = 0; y < 224; y++) {
		for (x = 0; x < 128; x++) {
			color = ((g_Pal[g_ScrBuf[y + 8][x*2 + 8]] + (g_Pal[g_ScrBuf[y + 8][x*2 + 8+1]] << 16)) >> 2) & THUMB_RGBMASK2;
			pVram[x] = ((pVram[x] >> 2) & THUMB_RGBMASK2) + color + color + color;
		}
		pVram += LINESIZE/2;
	}
}




/////////////////////////////////////////////////
// thumbnail
// 128x112、16Bit です。

#define THUMB_RGBMASK 0x1CE7	// = 0BBBBBGGGGGRRRRR = 0001110011100111B

// create thumbnail
boolean CreateThumnailFile(char *szThumbnailPath)
{
	// 小さいしローカルで(小さいか？)
	uint8 thumbnail[112][128][3];
	byte r, g, b;
	unsigned short color;
	int x,y;
	// create thumbnail forom palette and index buffer.
	for (y = 0; y < 224; y+=2) {
		for (x = 0; x < 256; x+=2) {
			color =
				((g_Pal[g_ScrBuf[y + 8  ][x + 8  ]] >> 2) & THUMB_RGBMASK)+
				((g_Pal[g_ScrBuf[y + 8  ][x + 8+1]] >> 2) & THUMB_RGBMASK)+
				((g_Pal[g_ScrBuf[y + 8+1][x + 8  ]] >> 2) & THUMB_RGBMASK)+
				((g_Pal[g_ScrBuf[y + 8+1][x + 8+1]] >> 2) & THUMB_RGBMASK);
			r = (color      ) & 0x1F;
			g = (color >>  5) & 0x1F;
			b = (color >> 10) & 0x1F;
			thumbnail[y/2][x/2][0] = r * 0xFF / 0x1F;
			thumbnail[y/2][x/2][1] = g * 0xFF / 0x1F;
			thumbnail[y/2][x/2][2] = b * 0xFF / 0x1F;
		}
	}
	int ret = write_png(szThumbnailPath, (byte *)thumbnail, 128, 112);
	if (!ret) {
		// 容量不足っぽいので、ゴミデータ破棄
		sceIoRemove(szThumbnailPath);
	}
	return ret;
}

// create screen shot from current emu
boolean CreateScreenShotFile(char *pszFilePath)
{
	extern uint8 g_ReadRomImage[];// 間借り
	uint8 *pBuf = g_ReadRomImage;
	int x,y;
	byte r, g, b;
	unsigned short color;
	// セーブ画面作成
	for (y = 0; y < 224; y++) {
		for (x = 0; x < 256; x++) {
			color = g_Pal[g_ScrBuf[y + 8][x + 8]];
			r = (color      ) & 0x1F;
			g = (color >>  5) & 0x1F;
			b = (color >> 10) & 0x1F;
			pBuf[0] = r * 0xFF / 0x1F;
			pBuf[1] = g * 0xFF / 0x1F;
			pBuf[2] = b * 0xFF / 0x1F;
			pBuf += 3;
		}
	}
	// 保存
	int ret = write_png(pszFilePath, g_ReadRomImage, 256, 224);
	if (!ret) {
		// 容量不足っぽいので、ゴミデータ破棄
		sceIoRemove(pszFilePath);
	}
	return ret;
}

// load thumbnail
boolean LoadThumnailFile(const char *szThumbnailPath, uint16 *pBuf)
{
	char szFile[MAXPATH];
	char *p;
	if(read_png(szThumbnailPath, pBuf, 128*112*sizeof(uint16))) {
		return TRUE;
	}
	_strcpy(szFile, szThumbnailPath);
	// tn? file
	p = _strrchr(szFile, '.');
	if (p) {
		*p = '\0';
		// load file
		int hFile = NES_fopen(szFile, FILE_MODE_READ);
		if (hFile >= 0) {
			NES_fread(pBuf, sizeof(uint16)*128*112, 1, hFile);
			// Readsize == sizeof(uint16)*128*112 なら非圧縮サムネイル
			NES_fclose(hFile);
			return TRUE;
		}
	}
	return FALSE;
}
