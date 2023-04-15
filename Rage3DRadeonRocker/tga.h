#ifndef __tga_h_
#define __tga_h_
#ifdef __cplusplus
extern "C" {
#endif

unsigned int TGA_UploadImage(char *file);
unsigned int TGA_UploadCube(char *right, char *left, char *top, char *bottom, char *front, char *back);
unsigned int TGA_UploadDUDV(char *file);
unsigned int TGA_UploadDOT3(char *file);
unsigned int TGA_UploadCubeVirtCross(char *file);

#ifdef __cplusplus
}
#endif
#endif
