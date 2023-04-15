#ifndef _IMAGE_H_
#define _IMAGE_H_

typedef struct
{
	int Width, Height;
	unsigned long Depth;
	unsigned char *Data;
} IMAGE_t;

int JPEG_Load(char *filename, IMAGE_t *Image);
int DDS_Load(char *filename, IMAGE_t *Image);
int TGA_Load(char *filename, IMAGE_t *Image);

unsigned int IMAGE_UploadImage(char *filename);
unsigned int IMAGE_UploadDOT3(char *filename);
unsigned int IMAGE_UploadDOT3_64Bit(char *filename);
unsigned int IMAGE_UploadNormalMap_64Bit(char *filename);
unsigned int IMAGE_UploadCubeVirtCross(char *filename);

#endif
