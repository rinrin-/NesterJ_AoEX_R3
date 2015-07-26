#ifndef NES_CRC_
#define NES_CRC_

unsigned long CrcCalc(unsigned char *buf, int len);
unsigned long CrcCalca(unsigned char *buf, int len, unsigned long crc);

#endif
