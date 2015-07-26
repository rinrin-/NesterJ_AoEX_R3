#ifndef IMAGE_H
#define IMAGE_H

int write_png(const char *path, const byte *in, unsigned int width, unsigned int height);
int read_png(const char *path, unsigned short *out, size_t outlen);

#endif

