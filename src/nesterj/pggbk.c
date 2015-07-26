// primitive graphics for Hello World sce

#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspaudio.h>
#include <pspgu.h>
#include "pggbk.h"
#include "pg.h"
#include "string.h"
#include "debug/debug.h"

#include "fontGBK12.c"


void pgPrintGBK12(unsigned long x,unsigned long y,unsigned long color,const char *str)
{
	mh_printGBK12((int)x*6,(int)y*12,str,(int)color);
}

void Draw_Char_GBK12(int x,int y,const unsigned char u,unsigned char d,int col) {

	unsigned short *vr;
	unsigned short *fnt;
	unsigned short pt;
	int x1,y1;

	unsigned long n;
	unsigned short code;
	//int j;

	// SJIS僐乕僪偺惗惉
	code = u;
	code = (code<<8) + d;


	//1 算区号
	unsigned short iare = u - 0x81 ;//区号
    unsigned short ibit = d ;//位号	

	if( ibit<0x7f )
		ibit = ibit - 0x40;
	if( ibit>0x80 )
		ibit = ibit - 0x41;

	n = iare*190 + ibit;

	if( n>23940 )//23940
		n = 0;
		
	fnt = (unsigned short *)&gbk_fontSong12[n*12];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,y);
	for(y1=0;y1<12;y1++) 
	{//一个字12行
		pt = *fnt++;
		for(x1=0;x1<12;x1++) 
		{//13
			if (pt & 1)
				*vr = col;
			vr++;
			pt = pt >> 1;
		}
		vr += LINESIZE-12;
	}
}


void Draw_Char_Asc12(int x,int y,const unsigned char c,int col) {
	unsigned short *vr;
	unsigned short *fnt;
	unsigned short  pt;
	unsigned char ch;
	int x1,y1;

	ch = c;

	// mapping
	fnt = (unsigned short *)&asc_font12[ch*12];

	// draw
	vr = (unsigned short *)pgGetVramAddr(x,y);
	for(y1=0;y1<12;y1++) 
	{
		pt = *fnt++;
		for(x1=0;x1<6;x1++)
		{
			if (pt & 1)
				*vr = col;
			vr++;
			pt = pt >> 1;
		}
		vr += LINESIZE-6;
	}
}

void mh_printGBK12(int x,int y,const char *str,int col) {
	unsigned char ch = 0,bef = 0;

	while(*str != 0) 
	{
		ch = (unsigned char)*str++;
	
		if (bef!=0) 
		{
			Draw_Char_GBK12(x,y,bef,ch,col);
			//Draw_Char_Zenkaku(x,y,ch,bef,col); Unicode
			x+=12;
			bef=0;
		}
		else 
		{
			if ( ch>=0x80 ) 
			{
				bef = ch;
			}
			else 
			{
				Draw_Char_Asc12(x,y,ch,col);
				x+=6;
			}
		}
	}
}

