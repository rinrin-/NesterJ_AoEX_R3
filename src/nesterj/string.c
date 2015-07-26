#include <pspkernel.h>
#include "syscall.h"

int _memcmp(const void *buf1, const void *buf2,int n)
{
	int ret;
	int i;
	
	for(i=0; i<n; i++){
		ret = ((unsigned char*)buf1)[i] - ((unsigned char*)buf2)[i];
		if(ret!=0)
			return ret;
	}
	return 0;
}

void* _memcpy(void *buf1, const void *buf2, int n)
{
	while(n-->0)
		((unsigned char*)buf1)[n] = ((unsigned char*)buf2)[n];
	return buf1;
}

void* _memset(void *buf, int ch, int n)
{
	unsigned char *p = buf;
	
	while(n>0)
		p[--n] = ch;
	
	return buf;
}

int _strlen(const char *s)
{
	int ret;
	
	for(ret=0; s[ret]; ret++)
		;
	
	return ret;
}

char* _strcpy(char *dest, const char *src)
{
	int i;
	
	for(i=0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = 0;
	
	return dest;
}

char* _strncpy(char *dest, const char *src, int size)
{
	int i;

	for(i=0; src[i] && size; i++, size--)
		dest[i] = src[i];
	while (size--) {
		dest[i] = 0;
		i++;
	}

	return dest;
}

char* _strrchr(const char *src, int c)
{
	int len;
	
	len=_strlen(src);
	while(len>0){
		len--;
		if(*(src+len) == c)
			return (char*)(src+len);
	}
	
	return NULL;
}

char* _strcat(char *dest, const char *src)
{
	int i;
	int len;
	
	len=_strlen(dest);
	for(i=0; src[i]; i++)
		dest[len+i] = src[i];
	dest[len+i] = 0;
	
	return dest;
}

int _strcmp(const char *str1, const char *str2)
{
	char c1, c2;
	for(;;){
		c1 = *str1;
		c2 = *str2;
		
		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;
		
		str1++; str2++;
	}
}

int _strncmp(const char *str1, const char *str2, int size)
{
	char c1, c2;
	while(size--){
		c1 = *str1;
		c2 = *str2;

		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;

		str1++; str2++;
	}
	return 0;
}

int _strnicmp(const char *str1, const char *str2,int size)
{
	char c1, c2;
	while(size--){
		c1 = *str1;
		if(c1>=0x61 && c1<=0x7A) c1-=0x20;
		c2 = *str2;
		if(c2>=0x61 && c2<=0x7A) c2-=0x20;

		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;

		str1++; str2++;
	}
	return 0;
}

int _stricmp(const char *str1, const char *str2)
{
	char c1, c2;
	for(;;){
		c1 = *str1;
		if(c1>=0x61 && c1<=0x7A) c1-=0x20;
		c2 = *str2;
		if(c2>=0x61 && c2<=0x7A) c2-=0x20;
		
		if(c1!=c2)
			return 1;
		else if(c1==0)
			return 0;
		
		str1++; str2++;
	}
}

void _strrev(char *s){
	char tmp;
	int i;
	int len = _strlen(s);
	
	for(i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-1-i];
		s[len-1-i] = tmp;
	}
}

void _itoa(int val, char *s) {
	char *t;
	int mod;

	if(val < 0) {
		*s++ = '-';
		val = -val;
	}
	t = s;

	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	_strrev(s);
}

void _itoa32(unsigned long val, char *s) {
	char *t;
	unsigned long mod;

	if(val < 0) {
		*s++ = '-';
		val = -val;
	}
	t = s;

	while(val) {
		mod = val % 10;
		*t++ = (char)mod + '0';
		val /= 10;
	}

	if(s == t)
		*t++ = '0';

	*t = '\0';

	_strrev(s);
}

int _atoi(const unsigned char *pszStr)
{
	unsigned long ulVal = 0;
	int cbI = 0;

	while (pszStr[cbI]) {
		if (pszStr[cbI] >= '0' && pszStr[cbI] <= '9') {
			ulVal *= 10;
			ulVal += pszStr[cbI] - '0';
		}
		else {
			break;
		}
		cbI++;
	}
	return ulVal;
}

unsigned long _atoh(const unsigned char *pszStr)
{
	unsigned long ulVal = 0;
	int cbI = 0;

	while (pszStr[cbI]) {
		if (pszStr[cbI] >= '0' && pszStr[cbI] <= '9') {
			ulVal <<= 4;
			ulVal += pszStr[cbI] - '0';
		}
		else if (pszStr[cbI] >= 'a' && pszStr[cbI] <= 'f') {
			ulVal <<= 4;
			ulVal += pszStr[cbI] - 'a' + 10;
		}
		else if (pszStr[cbI] >= 'A' && pszStr[cbI] <= 'F') {
			ulVal <<= 4;
			ulVal += pszStr[cbI] - 'a' + 10;
		}
		else {
			break;
		}
		cbI++;
	}
	return ulVal;
}



char* _strtok(char *s, const char *delim)
{
  const char *spanp;
  int c, sc;
  char *tok;
  static char *last;


  if (s == NULL && (s = last) == NULL)
    return (NULL);

  /*
   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
   */
 cont:
  c = *s++;
  for (spanp = delim; (sc = *spanp++) != 0;) {
    if (c == sc)
      goto cont;
  }

  if (c == 0) {                 /* no non-delimiter characters */
    last = NULL;
    return (NULL);
  }
  tok = s - 1;

  /*
   * Scan token (scan for delimITers: s += strcspn(s, delim), sort of).
   * Note that delim must have one NUL; we stop if we see that, too.
   */
  for (;;) {
    c = *s++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = NULL;
        else
          s[-1] = 0;
        last = s;
        return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}


void _Str2Hex(/*IN*/const char* Str,/*OUT*/ char* pHex)
{
   int i=0,j=0;
   int highBit=0,lowBit=0;//十位数，个位数
   int bNeedConvert;
   
   
   for( i=0,bNeedConvert=0; Str[i]!=0; i++)
   {
		if( Str[i]>>7 )
		{
			bNeedConvert = 1;
		}
		pHex[i]=Str[i];
   }
   pHex[i]=0;
   
   if(bNeedConvert==0)
   {		
		return;
   }
   
   
   for( i=0,j=0; Str[i]!=0; ++i,j+=2 )
   {
       highBit = (Str[i]&0xff)>>4; //除以十六，得到高位数7
       lowBit  = (Str[i]&0x0f);    //得到低位0~0xf

       //高位在前，先处理高位
       if( highBit>=0 && highBit<=9 )
       {
           pHex[j] = highBit + ('0'-0x0);  //将0x0~0x9,转化为'0'~'9'

       }else if( highBit>=0xa && highBit <= 0xf )
       {
           pHex[j] = highBit + ('a'-0xa); //将0xa~0xf,转化为'a'~'f'
       }else
       {
           pHex[j] =0;
       }

       //低位在后，处理低位
       if( lowBit>=0 && lowBit<=9 )
       {
           pHex[j+1] = lowBit + ('0'-0x0);

       }else if( lowBit>=0xa && lowBit <= 0xf )
       {
           pHex[j+1] = lowBit + ('a'-0xa);
       }else
       {
           pHex[j+1] =0;
       }
       if( j>=200 )
    	   break;
   }//for
   pHex[j-2]='\0';
   pHex[j-1]='\0';//这两句必须加上，否则打印字符串时会有小尾巴

   return ;
}

void _Hex2Str(/*IN*/ const char* pHex,/*OUT*/ char* Str)
{
   int i=0,j=0;
   int highBit=0,lowBit=0;//十位数，个位数

   for( i=0,j=0; pHex[i]!=0; ++j,i+=2 )
   {
       //先计算Str[i]高位
       if( pHex[i]>='0' && pHex[i] <='9')
       {
            highBit = pHex[i] -'0';       //'0'~'9' ----> 0~9
       }else if( pHex[i]>='a' && pHex[i]<='f' )
       {
            highBit = pHex[i] -'a' +0xa;  //'a'~'f'---->0xa~0xf
       }
       else if( pHex[i] >='A' && pHex[i]<='F')
       {
            highBit = pHex[i] -'A' +0xa;  //'A'~'F'------>0xa~0xf
       }else
       {
            highBit =0;
       }

       //计算字符Str[i]对应的十六进制的低位
       if( pHex[i+1]>='0' && pHex[i+1] <='9')
       {
            lowBit = pHex[i+1] -'0';      //'0'~'9' ----> 0~9
       }else if( pHex[i+1]>='a' && pHex[i+1]<='f' )
       {
            lowBit = pHex[i+1] -'a' +0xa; //'a'~'f'---->0xa~0xf
       }
       else if( pHex[i+1] >='A' && pHex[i+1]<='F')
       {
            lowBit = pHex[i+1] -'A' +0xa; //'a'~'f'---->0xa~0xf
       }else
       {
            lowBit =0;
       }

       Str[j]  = (highBit<<4) + lowBit;//不能写成highBit<<4+lowBit;

   }//for
   Str[j-1]='\0';//这句必须加上，否则打印字符串时会有小尾巴
   return ;
}
