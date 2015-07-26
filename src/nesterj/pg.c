// primitive graphics for Hello World sce

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspaudio.h>
#include <pspgu.h>
#include "pg.h"
#include "string.h"
#include "debug/debug.h"

#include "font.c"
#include "fontNaga10.c"

// Gu List;
static unsigned int __attribute__((aligned(16))) list[262144];

void *pFrameBuf[2];

//variables
//char *pg_vramtop=(char *)0x04000000;
#define pg_vramtop ((unsigned char *)0x04000000)
long pg_screenmode;
long pg_showframe;
long pg_drawframe;
//unsigned long pgc_csr_x[2], pgc_csr_y[2];
//unsigned long pgc_fgcolor[2], pgc_bgcolor[2];
//char pgc_fgdraw[2], pgc_bgdraw[2];
//char pgc_mag[2];

char pg_workdir[MAX_PATH];

void pgWaitVn(unsigned long count)
{
	for (; count>0; --count) {
		sceDisplayWaitVblankStart();
	}
}


void pgWaitV()
{
	// Startまで待たないでVBlank中なら戻るように変更
// sceDisplayWaitVblankはPSPSDKだと未定義？
//	sceDisplayWaitVblank();
	sceDisplayWaitVblankStart();
}


unsigned char *pgGetVramAddr(unsigned long x,unsigned long y)
{
	return pg_vramtop+(pg_drawframe*FRAMESIZE)+x*PIXELSIZE*2+y*LINESIZE*2+0x40000000;
//	return pg_vramtop+(pg_drawframe?FRAMESIZE:0)+x*PIXELSIZE*2+y*LINESIZE*2;//+0x40000000;	//変わらないらしい
}


void pgInit()
{
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);
	pgScreenFrame(0,0);
}


void pgPrint(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX_X && y<CMAX_Y) {
		pgPutChar(x*8,y*8,color,0,*str,1,0,1);
		str++;
		x++;
		if (x>=CMAX_X) {
			x=0;
			y++;
		}
	}
}

void pgPrint2(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX2_X && y<CMAX2_Y) {
		pgPutChar(x*16,y*16,color,0,*str,1,0,2);
		str++;
		x++;
		if (x>=CMAX2_X) {
			x=0;
			y++;
		}
	}
}


void pgPrint4(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	while (*str!=0 && x<CMAX4_X && y<CMAX4_Y) {
		pgPutChar(x*32,y*32,color,0,*str,1,0,4);
		str++;
		x++;
		if (x>=CMAX4_X) {
			x=0;
			y++;
		}
	}
}

void pgDrawFrame(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i;

	vptr0=pgGetVramAddr(0,0);
	for(i=x1; i<=x2; i++){
		((unsigned short *)vptr0)[i*PIXELSIZE + y1*LINESIZE] = color;
		((unsigned short *)vptr0)[i*PIXELSIZE + y2*LINESIZE] = color;
	}
	for(i=y1; i<=y2; i++){
		((unsigned short *)vptr0)[x1*PIXELSIZE + i*LINESIZE] = color;
		((unsigned short *)vptr0)[x2*PIXELSIZE + i*LINESIZE] = color;
	}
}

void pgFillBox(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i, j;

	vptr0=pgGetVramAddr(0,0);
	for(i=y1; i<=y2; i++){
		for(j=x1; j<=x2; j++){
			((unsigned short *)vptr0)[j*PIXELSIZE + i*LINESIZE] = color;
		}
	}
}

void pgFillvram(unsigned long color)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned long i;

	vptr0=pgGetVramAddr(0,0);
	for (i=0; i<FRAMESIZE/2; i++) {
		*(unsigned short *)vptr0=color;
		vptr0+=PIXELSIZE*2;
	}
}

void pgBitBlt(unsigned long x,unsigned long y,unsigned long w,unsigned long h,unsigned long mag,const unsigned short *d)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	unsigned long xx,yy,mx,my;
	const unsigned short *dd;

	vptr0=pgGetVramAddr(x,y);
	for (yy=0; yy<h; yy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			dd=d;
			for (xx=0; xx<w; xx++) {
				for (mx=0; mx<mag; mx++) {
					*(unsigned short *)vptr=*dd;
					vptr+=PIXELSIZE*2;
				}
				dd++;
			}
			vptr0+=LINESIZE*2;
		}
		d+=w;
	}

}
#define THUMB_RGBMASK2 0x1CE71CE7	// = 0BBBBBGGGGGRRRRR0BBBBBGGGGGRRRRR = 00011100111001110001110011100111B

// 75%合成
void pgBitBltAlpha75(unsigned long x,unsigned long y,unsigned long w,unsigned long h,const unsigned short *d)
{
	unsigned long *pVram, *pSrc, color;
	if (x & 1) return;
	w >>= 1;
	pSrc = (unsigned long *)d;
	pVram = (unsigned long *)pgGetVramAddr(x, y);
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			color = ((pSrc[x] >> 2) & THUMB_RGBMASK2);
			pVram[x] = ((pVram[x] >> 2) & THUMB_RGBMASK2) + color + color + color;
		}
		pVram += LINESIZE/2;
		pSrc += w;
	}
}

void pgPutChar(unsigned long x,unsigned long y,unsigned long color,unsigned long bgcolor,unsigned char ch,char drawfg,char drawbg,char mag)
{
	unsigned char *vptr0;		//pointer to vram
	unsigned char *vptr;		//pointer to vram
	const unsigned char *cfont;		//pointer to font
	unsigned long cx,cy;
	unsigned long b;
	char mx,my;

//	if (ch>255) return;
	cfont=font+ch*8;
	vptr0=pgGetVramAddr(x,y);
	for (cy=0; cy<8; cy++) {
		for (my=0; my<mag; my++) {
			vptr=vptr0;
			b=0x80;
			for (cx=0; cx<8; cx++) {
				for (mx=0; mx<mag; mx++) {
					if ((*cfont&b)!=0) {
						if (drawfg) *(unsigned short *)vptr=color;
					} else {
						if (drawbg) *(unsigned short *)vptr=bgcolor;
					}
					vptr+=PIXELSIZE*2;
				}
				b=b>>1;
			}
			vptr0+=LINESIZE*2;
		}
		cfont++;
	}
}


void pgScreenFrame(long mode,long frame)
{
	pg_screenmode=mode;
	frame=(frame)%3;
	pg_showframe=frame;
	if (mode==0) {
		//screen off
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(0,0,0,1);
	} else if (mode==1) {
		//show/draw same
		pg_drawframe=frame;
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe*FRAMESIZE),LINESIZE,PIXELSIZE,1);
	} else if (mode==2) {
		//show/draw different
		pg_drawframe=(frame +1)%3;
		sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe*FRAMESIZE),LINESIZE,PIXELSIZE,1);
	}
}


void pgScreenFlip()
{
	pg_showframe=(pg_showframe +1) % 3;
	pg_drawframe=(pg_drawframe +1) % 3;
	sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe*FRAMESIZE),LINESIZE,PIXELSIZE,
                        PSP_DISPLAY_SETBUF_IMMEDIATE);

}


void pgScreenFlipV()
{
	pgWaitV();
	pgScreenFlip();


}
void pgScreenFlipV_fast()
{
	/*pgWaitV();
	pgScreenFlip();
	*/
	pg_showframe=(pg_showframe +1) % 3;
	pg_drawframe=(pg_drawframe +1) % 3;
	sceDisplaySetFrameBuf(pg_vramtop+(pg_showframe*FRAMESIZE),LINESIZE,PIXELSIZE,
                        PSP_DISPLAY_SETBUF_NEXTFRAME);
}

// by kwn
void Draw_Char_Hankaku(int x,int y,const unsigned char c,int col) {
	unsigned short *vr;
	unsigned char  *fnt;
	unsigned char  pt;
	unsigned char ch;
	int x1,y1;

	ch = c;

	// mapping
	if (ch<0x20)
		ch = 0;
	else if (ch<0x80)
		ch -= 0x20;
	else if (ch<0xa0)
		ch = 0;
	else
		ch -= 0x40;

	fnt = (unsigned char *)&hankaku_font10[ch*10];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,y);
	for(y1=0;y1<10;y1++) {
		pt = *fnt++;
		for(x1=0;x1<5;x1++) {
			if (pt & 1)
				*vr = col;
			vr++;
			pt = pt >> 1;
		}
		vr += LINESIZE-5;
	}
}

// by kwn
void Draw_Char_Zenkaku(int x,int y,const unsigned char u,unsigned char d,int col) {
	// ELISA100.FNTに存在しない文字
	const unsigned short font404[] = {
		0xA2AF, 11,
		0xA2C2, 8,
		0xA2D1, 11,
		0xA2EB, 7,
		0xA2FA, 4,
		0xA3A1, 15,
		0xA3BA, 7,
		0xA3DB, 6,
		0xA3FB, 4,
		0xA4F4, 11,
		0xA5F7, 8,
		0xA6B9, 8,
		0xA6D9, 38,
		0xA7C2, 15,
		0xA7F2, 13,
		0xA8C1, 720,
		0xCFD4, 43,
		0xF4A5, 1030,
		0,0
	};
	unsigned short *vr;
	unsigned short *fnt;
	unsigned short pt;
	int x1,y1;

	unsigned long n;
	unsigned short code;
	int j;

	// SJISコードの生成
	code = u;
	code = (code<<8) + d;

	// SJISからEUCに変換
	if(code >= 0xE000) code-=0x4000;
	code = ((((code>>8)&0xFF)-0x81)<<9) + (code&0x00FF);
	if((code & 0x00FF) >= 0x80) code--;
	if((code & 0x00FF) >= 0x9E) code+=0x62;
	else code-=0x40;
	code += 0x2121 + 0x8080;

	// EUCから恵梨沙フォントの番号を生成
	n = (((code>>8)&0xFF)-0xA1)*(0xFF-0xA1)
		+ (code&0xFF)-0xA1;
	j=0;
	while(font404[j]) {
		if(code >= font404[j]) {
			if(code <= font404[j]+font404[j+1]-1) {
				n = -1;
				break;
			} else {
				n-=font404[j+1];
			}
		}
		j+=2;
	}
	fnt = (unsigned short *)&zenkaku_font10[n*10];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,y);
	for(y1=0;y1<10;y1++) {
		pt = *fnt++;
		for(x1=0;x1<10;x1++) {
			if (pt & 1)
				*vr = col;
			vr++;
			pt = pt >> 1;
		}
		vr += LINESIZE-10;
	}
}

// by kwn
void mh_print(int x,int y,const char *str,int col) {
	unsigned char ch = 0,bef = 0;

	while(*str != 0) {
		ch = (unsigned char)*str++;
		if (bef!=0) {
			Draw_Char_Zenkaku(x,y,bef,ch,col);
			x+=10;
			bef=0;
		} else {
			if (((ch>=0x80) && (ch<0xa0)) || (ch>=0xe0)) {
				bef = ch;
			} else {
				Draw_Char_Hankaku(x,y,ch,col);
				x+=5;
			}
		}
	}
}

u32 new_pad;
u32 old_pad;
u32 now_pad;
//SceCtrlData paddata;

void readpad(void)
{
	static int n=0;
	SceCtrlData paddata;

	sceCtrlPeekBufferPositive(&paddata, 1);
	// アナログはデジタルキーとして変換する
	if (paddata.Ly == 0xff) {
		paddata.Buttons |= PSP_CTRL_A_DOWN;  // DOWN
	}
	else if (paddata.Ly == 0x00) {
		paddata.Buttons |= PSP_CTRL_A_UP;    // UP
	}
	if (paddata.Lx == 0x00) {
		paddata.Buttons |= PSP_CTRL_A_LEFT;  // LEFT
	}
	else if (paddata.Lx == 0xff) {
		paddata.Buttons |= PSP_CTRL_A_RIGHT; // RIGHT
	}

	now_pad = paddata.Buttons;
	new_pad = now_pad & ~old_pad;
	if(old_pad==now_pad){
		n++;
		if(n>=25){
			new_pad=now_pad;
			n = 20;
		}
	}else{
		n=0;
		old_pad = now_pad;
	}
}

/******************************************************************************/

/*

void pgcLocate(unsigned long x, unsigned long y)
{
	if (x>=CMAX_X) x=0;
	if (y>=CMAX_Y) y=0;
	pgc_csr_x[pg_drawframe?1:0]=x;
	pgc_csr_y[pg_drawframe?1:0]=y;
}


void pgcColor(unsigned long fg, unsigned long bg)
{
	pgc_fgcolor[pg_drawframe?1:0]=fg;
	pgc_bgcolor[pg_drawframe?1:0]=bg;
}


void pgcDraw(char drawfg, char drawbg)
{
	pgc_fgdraw[pg_drawframe?1:0]=drawfg;
	pgc_bgdraw[pg_drawframe?1:0]=drawbg;
}


void pgcSetmag(char mag)
{
	pgc_mag[pg_drawframe?1:0]=mag;
}

void pgcCls()
{
	pgFillvram(pgc_bgcolor[pg_drawframe]);
	pgcLocate(0,0);
}

void pgcPutchar_nocontrol(const char ch)
{
	pgPutChar(pgc_csr_x[pg_drawframe]*8, pgc_csr_y[pg_drawframe]*8, pgc_fgcolor[pg_drawframe], pgc_bgcolor[pg_drawframe], ch, pgc_fgdraw[pg_drawframe], pgc_bgdraw[pg_drawframe], pgc_mag[pg_drawframe]);
	pgc_csr_x[pg_drawframe]+=pgc_mag[pg_drawframe];
	if (pgc_csr_x[pg_drawframe]>CMAX_X-pgc_mag[pg_drawframe]) {
		pgc_csr_x[pg_drawframe]=0;
		pgc_csr_y[pg_drawframe]+=pgc_mag[pg_drawframe];
		if (pgc_csr_y[pg_drawframe]>CMAX_Y-pgc_mag[pg_drawframe]) {
			pgc_csr_y[pg_drawframe]=CMAX_Y-pgc_mag[pg_drawframe];
//			pgMoverect(0,pgc_mag[pg_drawframe]*8,SCREEN_WIDTH,SCREEN_HEIGHT-pgc_mag[pg_drawframe]*8,0,0);
		}
	}
}

void pgcPutchar(const char ch)
{
	if (ch==0x0d) {
		pgc_csr_x[pg_drawframe]=0;
		return;
	}
	if (ch==0x0a) {
		if ((++pgc_csr_y[pg_drawframe])>=CMAX_Y) {
			pgc_csr_y[pg_drawframe]=CMAX_Y-1;
//			pgMoverect(0,8,SCREEN_WIDTH,SCREEN_HEIGHT-8,0,0);
		}
		return;
	}
	pgcPutchar_nocontrol(ch);
}
void pgcPuthex2(const unsigned long s)
{
	char ch;
	ch=((s>>4)&0x0f);
	pgcPutchar((ch<10)?(ch+0x30):(ch+0x40-9));
	ch=(s&0x0f);
	pgcPutchar((ch<10)?(ch+0x30):(ch+0x40-9));
}


void pgcPuthex8(const unsigned long s)
{
	pgcPuthex2(s>>24);
	pgcPuthex2(s>>16);
	pgcPuthex2(s>>8);
	pgcPuthex2(s);
}
*/
/******************************************************************************/



void pgiInit()
{
//	sceCtrlInit(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

/******************************************************************************/



int pga_ready=0;
int pga_handle;

// 1つしか使わないので変更。バッファ受け取り方も変更

void *(*pga_callback)(void);
int pga_threadhandle;
volatile int pga_terminate=0;

int pga_channel_thread(SceSize args, void *argp)
{
	void *bufptr;
	if (pga_callback) {
		while (pga_terminate==0) {
			bufptr = pga_callback();
			sceAudioOutputBlocking(pga_handle, PSP_AUDIO_VOLUME_MAX, bufptr);
		}
	}
	sceKernelExitThread(0);
	return 0;
}

void pgaSetChannelCallback(void *callback)
{
	pga_callback=callback;
}

/******************************************************************************/

int pgaInit()
{
	int ret;

	pga_terminate=0;
	pga_ready=0;

	pga_handle=-1;
	pga_threadhandle=-1;

	// NESはMONOなのでMONO出力
	if ((pga_handle=sceAudioChReserve(-1, PGA_SAMPLES, PSP_AUDIO_FORMAT_MONO)) < 0) return -1;

	pga_ready=1;

	pga_threadhandle=sceKernelCreateThread("pgasnd0", pga_channel_thread,0x12,0x10000,0,NULL);
	if (pga_threadhandle>=0) {
		ret=sceKernelStartThread(pga_threadhandle,0,NULL);
		if (ret==0) {
			return 0;
		}
		sceKernelDeleteThread(pga_threadhandle);
	}
	pga_threadhandle = -1;
	pga_ready=0;
	return -1;
}


void pgaTermPre()
{
	pga_ready=0;
	pga_terminate=1;
}


void pgaTerm()
{
	pga_ready=0;
	pga_terminate=1;

	if (pga_threadhandle!=-1) {
		sceKernelWaitThreadEnd(pga_threadhandle,NULL);
		sceKernelDeleteThread(pga_threadhandle);
		pga_threadhandle=-1;
	}

	if (pga_handle!=-1) {
		sceAudioChRelease(pga_handle);
		pga_handle = -1;
	}
}


//バッファは64バイト境界じゃなくても大丈夫みたい
//[0]が左、[1]が右
//サンプル速度は44100
//vol1が左


volatile int pg_terminate=0;

void pgMain(int args, char *argp[])
{
	int n;
	char pg_mypath[MAX_PATH];

	_strncpy(pg_mypath,argp[0], sizeof(pg_mypath));
	pg_mypath[sizeof(pg_mypath)-1]=0;
	DEBUG("pg_mypath");
	DEBUG(pg_mypath);
	_strcpy(pg_workdir,pg_mypath);
	for (n=_strlen(pg_workdir); n>0 && pg_workdir[n-1]!='/'; --n) pg_workdir[n-1]=0;
	DEBUG("pg_workdir");
	DEBUG(pg_workdir);

	pspDebugScreenInit();

	pFrameBuf[0] = (void*)0;
	pFrameBuf[1] = (void*)FRAME_SIZE;

	DEBUG("sceGuInit();");
	sceGuInit();
	DEBUG("sceGuStart(GU_DIRECT,list);");
	sceGuStart(GU_DIRECT,list);

	DEBUG("sceGuDrawBuffer(GU_PSM_5551, pFrameBuf[0], SCREEN_PITCH);");
	sceGuDrawBuffer(GU_PSM_5551, pFrameBuf[0], SCREEN_PITCH);
	DEBUG("sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, pFrameBuf[1], SCREEN_PITCH);");
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, pFrameBuf[1], SCREEN_PITCH);
	DEBUG("sceGuOffset(2048 - (SCREEN_WIDTH/2),2048 - (SCREEN_HEIGHT/2));");
	sceGuOffset(2048 - (SCREEN_WIDTH/2),2048 - (SCREEN_HEIGHT/2));
	DEBUG("sceGuViewport(2048,2048, SCREEN_WIDTH, SCREEN_HEIGHT);");
	sceGuViewport(2048,2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	DEBUG("sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);");
	sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	DEBUG("sceGuEnable(GU_SCISSOR_TEST);");
	sceGuEnable(GU_SCISSOR_TEST);
	DEBUG("sceGuFrontFace(GU_CW);");
	sceGuFrontFace(GU_CW);
	DEBUG("sceGuTexFilter(GU_LINEAR,GU_LINEAR);");
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	DEBUG("sceGuEnable(GU_TEXTURE_2D);");
	sceGuEnable(GU_TEXTURE_2D);
	DEBUG("sceGuClear(GU_COLOR_BUFFER_BIT);");
	sceGuClear(GU_COLOR_BUFFER_BIT);
	DEBUG("sceGuFinish();");
	sceGuFinish();
	DEBUG("sceGuSync(0,0);");
	sceGuSync(0,0);

	DEBUG("sceDisplayWaitVblankStart();");
	sceDisplayWaitVblankStart();
	DEBUG("sceGuDisplay(GU_TRUE);");
	sceGuDisplay(GU_TRUE);

/*
	sceDisplaySetMode(0,SCREEN_WIDTH,SCREEN_HEIGHT);

	pgScreenFrame(0,1);
	pgScreenFrame(0,0);
*/
/*
	pgScreenFrame(0,1);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgcDraw(1,1);
	pgcSetmag(1);
	pgScreenFrame(0,0);
	pgcLocate(0,0);
	pgcColor(0xffff,0x0000);
	pgcDraw(1,1);
	pgcSetmag(1);
*/
	pgiInit();

//	ret=pgaInit();
//	if (ret) pgErrorHalt("pga subsystem initialization failed.");

//	ret=xmain();
//	pgExit(ret);
}

/*
void pgExit(int n)
{
	pg_terminate=1;

	// terminate subsystem preprocess
	pgaTermPre();

	// terminate subsystem
	pgaTerm();

	__exit();
}



const char *pguGetMypath()
{
	return pg_mypath;
}

const char *pguGetWorkdir()
{
	return pg_workdir;
}
*/
