#ifndef _IMAGE_H_
#define _IMAGE_H_

#define IMAGE_FLOAT ('F'|('L'<<8)|('T'<<16)|('P'<<24))
#define IMAGE_DXT1 ('D'|('X'<<8)|('T'<<16)|('1'<<24))
#define IMAGE_DXT3 ('D'|('X'<<8)|('T'<<16)|('3'<<24))
#define IMAGE_DXT5 ('D'|('X'<<8)|('T'<<16)|('5'<<24))

typedef struct
{
	int Width, Height;
	unsigned long Depth;
	unsigned char *Data;
} IMAGE_t;

int JPEG_Load(char *filename, IMAGE_t *Image);
int DDS_Load(char *filename, IMAGE_t *Image);
int TGA_Load(char *filename, IMAGE_t *Image);
int HDR_Load(char *filename, IMAGE_t *Image);

unsigned int IMAGE_UploadImage(char *filename);
unsigned int IMAGE_UploadDOT3(char *filename);
unsigned int IMAGE_UploadDOT3_64Bit(char *filename);
unsigned int IMAGE_UploadNormalMap_64Bit(char *filename);
unsigned int IMAGE_UploadCubeVirtCross(char *filename);

#endif
