#ifndef STRING_H
#define STRING_H

int _memcmp(const void *buf1, const void *buf2,int n);
void *_memcpy(void *buf1, const void *buf2, int n);
void *_memset(void *buf, int ch, int n);
int _strlen(const char *s);
char* _strcpy(char *dest, const char *src);
char* _strncpy(char *dest, const char *src, int size);
char* _strrchr(const char *, int);
char *_strcat(char *s1, const char *s2);
int _strcmp(const char *str1, const char *str2);
int _stricmp(const char *str1, const char *str2);
int _strnicmp(const char *str1, const char *str2, int size);
int _strncmp(const char *str1, const char *str2, int size);
void _itoa(int val, char *s);
void _itoa32(unsigned long val, char *s);
int _atoi(const unsigned char *pszStr);
unsigned long _atoh(const unsigned char *pszStr);
char* _strtok(char *s, const char *delim);
void _Str2Hex(/*IN*/const char* Str,/*OUT*/ char* pHex);

#endif
