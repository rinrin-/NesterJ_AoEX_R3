#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <png.h>

typedef u8 byte;
double pow(double x, double y)
{
	long ret=1;
	while(y-->0)
		ret*=x;
	return ret;
}


int write_png(const char *path, const byte *in, unsigned int width, unsigned int height)
{
    png_struct *pPngStruct = png_create_write_struct( PNG_LIBPNG_VER_STRING,
													  NULL, NULL, NULL );
	if (!pPngStruct){
		return 0;
	}

    png_info *pPngInfo = png_create_info_struct( pPngStruct );
	if (!pPngInfo){
		png_destroy_write_struct( &pPngStruct, NULL );
		return 0;
	}

	png_byte **buf = (png_byte**)malloc(height*sizeof(png_byte*));
	if (!buf){
        png_destroy_write_struct( &pPngStruct, &pPngInfo );
		return 0;
	}

	unsigned int y;
	for(y=0; y<height; y++)
		buf[y] = (byte*)&in[y*width*3];

    if (setjmp( pPngStruct->jmpbuf )){
		free(buf);
        png_destroy_write_struct( &pPngStruct, &pPngInfo );
        return 0;
    }

    FILE *fp = fopen( path, "wb" );
	if (!fp){
		free(buf);
        png_destroy_write_struct( &pPngStruct, &pPngInfo );
		return 0;
	}

    png_init_io( pPngStruct, fp );
    png_set_IHDR( pPngStruct, pPngInfo, width, height, 8,
        PNG_COLOR_TYPE_RGB,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info( pPngStruct, pPngInfo );
    png_write_image( pPngStruct, buf );
    png_write_end( pPngStruct, pPngInfo );

    png_destroy_write_struct( &pPngStruct, &pPngInfo );
    fclose(fp);
	free(buf);

    return 1;
}

int read_png(const char *path, unsigned short *out, size_t outlen)
{
	FILE *fp = fopen(path,"rb");
	if(!fp)
		return 0;

	const size_t nSigSize = 8;
	byte signature[nSigSize];
	if (sceIoRead(fileno(fp), signature, sizeof(byte)*nSigSize) != nSigSize){
		fclose(fp);
		return 0;
	}

	if (!png_check_sig( signature, nSigSize )){
		fclose(fp);
		return 0;
	}

	png_struct *pPngStruct = png_create_read_struct( PNG_LIBPNG_VER_STRING,
													 NULL, NULL, NULL );
	if(!pPngStruct){
		fclose(fp);
		return 0;
	}

	png_info *pPngInfo = png_create_info_struct(pPngStruct);
	if(!pPngInfo){
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		fclose(fp);
		return 0;
	}

	if (setjmp( pPngStruct->jmpbuf )){
		png_destroy_read_struct( &pPngStruct, NULL, NULL );
		fclose(fp);
		return 0;
	}

	png_init_io( pPngStruct, fp );
	png_set_sig_bytes( pPngStruct, nSigSize );
	png_read_png( pPngStruct, pPngInfo,
			PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING |
			PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_BGR , NULL);

	png_uint_32 width = pPngInfo->width;
	png_uint_32 height = pPngInfo->height;
	int color_type = pPngInfo->color_type;

	if (outlen != width * height * sizeof(unsigned short)){
		png_destroy_read_struct( &pPngStruct, &pPngInfo, NULL );
		fclose(fp);
		return 0;
	}

	png_byte **pRowTable = pPngInfo->row_pointers;
	unsigned int x, y;
	byte r, g, b;
	for (y=0; y<height; y++){
		png_byte *pRow = pRowTable[y];
		for (x=0; x<width; x++){
			switch(color_type){
				case PNG_COLOR_TYPE_GRAY:
					r = g = b = *pRow++;
					break;
				case PNG_COLOR_TYPE_GRAY_ALPHA:
					r = g = b = pRow[0];
					pRow += 2;
					break;
				case PNG_COLOR_TYPE_RGB:
					b = pRow[0];
					g = pRow[1];
					r = pRow[2];
					pRow += 3;
					break;
				case PNG_COLOR_TYPE_RGB_ALPHA:
					b = pRow[0];
					g = pRow[1];
					r = pRow[2];
					pRow += 4;
					break;
				default:
					r = g = b = 0;
					break;
			}
			*out++ = RGB(r,g,b);
		}
	}

	png_destroy_read_struct( &pPngStruct, &pPngInfo, NULL );
	fclose(fp);

	return 1;
}

