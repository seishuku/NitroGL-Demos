#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "tga.h"
//#include "resource.h"

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1
#define GL_CLAMP_TO_EDGE_EXT 0x812F
#endif

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1
#define GL_NORMAL_MAP_ARB 0x8511
#define GL_REFLECTION_MAP_ARB 0x8512
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C
#endif

#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap 1
#define GL_GENERATE_MIPMAP_SGIS 0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS 0x8192
#endif

#ifndef GL_ATI_envmap_bumpmap
#define GL_ATI_envmap_bumpmap 1
#define GL_BUMP_ROT_MATRIX_ATI 0x8775
#define GL_BUMP_ROT_MATRIX_SIZE_ATI 0x8776
#define GL_BUMP_NUM_TEX_UNITS_ATI 0x8777
#define GL_BUMP_TEX_UNITS_ATI 0x8778
#define GL_DUDV_ATI 0x8779
#define GL_DU8DV8_ATI 0x877A
#define GL_BUMP_ENVMAP_ATI 0x877B
#define GL_BUMP_TARGET_ATI 0x877C
typedef void (APIENTRY *PFNGLTEXBUMPPARAMETERIVATIPROC)(GLenum pname, GLint *param);
typedef void (APIENTRY *PFNGLTEXBUMPPARAMETERFVATIPROC)(GLenum pname, GLfloat *param);
typedef void (APIENTRY *PFNGLGETTEXBUMPPARAMETERIVATIPROC)(GLenum pname, GLint *param);
typedef void (APIENTRY *PFNGLGETTEXBUMPPARAMETERFVATIPROC)(GLenum pname, GLfloat *param);
#endif

#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader
#define GL_OFFSET_TEXTURE_RECTANGLE_NV 0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#define GL_SHADER_CONSISTENT_NV 0x86DD
#define GL_TEXTURE_SHADER_NV 0x86DE
#define GL_SHADER_OPERATION_NV 0x86DF
#define GL_CULL_MODES_NV 0x86E0
#define GL_OFFSET_TEXTURE_MATRIX_NV 0x86E1
#define GL_OFFSET_TEXTURE_SCALE_NV 0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV 0x86E3
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV GL_OFFSET_TEXTURE_MATRIX_NV
#define GL_OFFSET_TEXTURE_2D_SCALE_NV GL_OFFSET_TEXTURE_SCALE_NV
#define GL_OFFSET_TEXTURE_2D_BIAS_NV GL_OFFSET_TEXTURE_BIAS_NV
#define GL_PREVIOUS_TEXTURE_INPUT_NV 0x86E4
#define GL_CONST_EYE_NV 0x86E5
#define GL_PASS_THROUGH_NV 0x86E6
#define GL_CULL_FRAGMENT_NV 0x86E7
#define GL_OFFSET_TEXTURE_2D_NV 0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV 0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV 0x86EA
#define GL_DOT_PRODUCT_NV 0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV 0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV 0x86EE
#define GL_DOT_PRODUCT_TEXTURE_3D_NV 0x86EF
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#define GL_HILO_NV 0x86F4
#define GL_DSDT_NV 0x86F5
#define GL_DSDT_MAG_NV 0x86F6
#define GL_DSDT_MAG_VIB_NV 0x86F7
#define GL_HILO16_NV 0x86F8
#define GL_SIGNED_HILO_NV 0x86F9
#define GL_SIGNED_HILO16_NV 0x86FA
#define GL_SIGNED_RGBA_NV 0x86FB
#define GL_SIGNED_RGBA8_NV 0x86FC
#define GL_SIGNED_RGB_NV 0x86FE
#define GL_SIGNED_RGB8_NV 0x86FF
#define GL_SIGNED_LUMINANCE_NV 0x8701
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define GL_SIGNED_ALPHA_NV 0x8705
#define GL_SIGNED_ALPHA8_NV 0x8706
#define GL_SIGNED_INTENSITY_NV 0x8707
#define GL_SIGNED_INTENSITY8_NV 0x8708
#define GL_DSDT8_NV 0x8709
#define GL_DSDT8_MAG8_NV 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#define GL_HI_SCALE_NV 0x870E
#define GL_LO_SCALE_NV 0x870F
#define GL_DS_SCALE_NV 0x8710
#define GL_DT_SCALE_NV 0x8711
#define GL_MAGNITUDE_SCALE_NV 0x8712
#define GL_VIBRANCE_SCALE_NV 0x8713
#define GL_HI_BIAS_NV 0x8714
#define GL_LO_BIAS_NV 0x8715
#define GL_DS_BIAS_NV 0x8716
#define GL_DT_BIAS_NV 0x8717
#define GL_MAGNITUDE_BIAS_NV 0x8718
#define GL_VIBRANCE_BIAS_NV 0x8719
#define GL_TEXTURE_BORDER_VALUES_NV 0x871A
#define GL_TEXTURE_HI_SIZE_NV 0x871B
#define GL_TEXTURE_LO_SIZE_NV 0x871C
#define GL_TEXTURE_DS_SIZE_NV 0x871D
#define GL_TEXTURE_DT_SIZE_NV 0x871E
#define GL_TEXTURE_MAG_SIZE_NV 0x871F
#endif

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	short X_Origin;
	short Y_Origin;
	short Width;
	short Height;
	unsigned char Depth;
	unsigned char Image_Descriptor;
} TGA_ImageSpec_t;

typedef struct
{
	short Origin;
	short Length;
	unsigned char EntrySize;
} TGA_ColorMapSpec_t;

typedef struct
{
	unsigned char ID;
	unsigned char ColorMapType;
	unsigned char ImageType;
	TGA_ColorMapSpec_t ColorMapSpec;
	TGA_ImageSpec_t ImageSpec;
	unsigned char *ColorMap;
	unsigned char *Data;
} TGA_t;

#pragma pack(pop)

void TGA_GetPackets(unsigned char *data, int width, int height, int depth, FILE *stream)
{
	int current_byte=0, run_length, i;
	unsigned char header, buffer[4];

	while(current_byte<width*height*(depth/8))
	{
		fread(&header, sizeof(unsigned char), 1, stream);

		run_length=(header&0x7F)+1;

		if(header&0x80)
		{
			if(depth==32)
				fread(buffer, sizeof(unsigned char), 4, stream);

			if(depth==24)
				fread(buffer, sizeof(unsigned char), 3, stream);

			if(depth==8)
				fread(buffer, sizeof(unsigned char), 1, stream);

			for(i=0;i<run_length;i++)
			{
				if(depth==32)
				{
					data[current_byte]=buffer[0];
					data[current_byte+1]=buffer[1];
					data[current_byte+2]=buffer[2];
					data[current_byte+3]=buffer[3];
					current_byte+=4;
				}

				if(depth==24)
				{
					data[current_byte]=buffer[0];
					data[current_byte+1]=buffer[1];
					data[current_byte+2]=buffer[2];
					current_byte+=3;
				}

				if(depth==8)
				{
					data[current_byte]=buffer[0];
					current_byte++;
				}
			}
		}

		if(!(header&0x80))
		{
			for(i=0;i<run_length;i++)
			{
				if(depth==32)
					fread(buffer, sizeof(unsigned char), 4, stream);

				if(depth==24)
					fread(buffer, sizeof(unsigned char), 3, stream);

				if(depth==8)
					fread(buffer, sizeof(unsigned char), 1, stream);

				if(depth==32)
				{
					data[current_byte]=buffer[0];
					data[current_byte+1]=buffer[1];
					data[current_byte+2]=buffer[2];
					data[current_byte+3]=buffer[3];
					current_byte+=4;
				}

				if(depth==24)
				{
					data[current_byte]=buffer[0];
					data[current_byte+1]=buffer[1];
					data[current_byte+2]=buffer[2];
					current_byte+=3;
				}

				if(depth==8)
				{
					data[current_byte]=buffer[0];
					current_byte++;
				}
			}
		}
	}
}

BOOL TGA_GetData(char *file, TGA_t *TGA)
{
	FILE *stream;

	if((stream=fopen(file, "rb"))==NULL)
	{
		MessageBox(NULL, "File not found.", "TGA", MB_OK);
		return FALSE;
	}

	fread(&TGA->ID, sizeof(unsigned char), 1, stream);
	fread(&TGA->ColorMapType, sizeof(unsigned char), 1, stream);
	fread(&TGA->ImageType, sizeof(unsigned char), 1, stream);
	fread(&TGA->ColorMapSpec, sizeof(unsigned char), sizeof(TGA_ColorMapSpec_t), stream);
	fread(&TGA->ImageSpec, sizeof(char), sizeof(TGA_ImageSpec_t), stream);
	fseek(stream, TGA->ID, SEEK_CUR);

	switch(TGA->ImageType)
	{
		case 1:
			if(TGA->ColorMapType==1&&TGA->ColorMapSpec.EntrySize==24)
			{
				TGA->ColorMap=malloc(TGA->ColorMapSpec.Length*(TGA->ColorMapSpec.EntrySize/8));
				fread(TGA->ColorMap, sizeof(unsigned char), TGA->ColorMapSpec.Length*(TGA->ColorMapSpec.EntrySize/8), stream);
			}
			else
			{
				MessageBox(NULL, "Only 24bit paletted images are supported.", "TGA", MB_OK);
				fclose(stream);
				return FALSE;
			}
			break;

		case 2:
			break;

		case 3:
			break;

		case 9:
			if(TGA->ColorMapType==1&&TGA->ColorMapSpec.EntrySize==24)
			{
				TGA->ColorMap=malloc(TGA->ColorMapSpec.Length*(TGA->ColorMapSpec.EntrySize/8));
				fread(TGA->ColorMap, sizeof(unsigned char), TGA->ColorMapSpec.Length*(TGA->ColorMapSpec.EntrySize/8), stream);
			}
			else
			{
				MessageBox(NULL, "Only 24bit paletted images are supported.", "TGA", MB_OK);
				fclose(stream);
				return FALSE;
			}
			break;

		case 10:
			break;

		case 11:
			break;

		default:
			MessageBox(NULL, "Unsupported image type.", "TGA", MB_OK);
			fclose(stream);
			return FALSE;
			break;
	}

	switch(TGA->ImageSpec.Depth)
	{
		case 32:
			TGA->Data=malloc(TGA->ImageSpec.Width*TGA->ImageSpec.Height*4);

			if(TGA->ImageType==2)
				fread(TGA->Data, sizeof(unsigned char), TGA->ImageSpec.Width*TGA->ImageSpec.Height*4, stream);

			if(TGA->ImageType==10)
				TGA_GetPackets(TGA->Data, TGA->ImageSpec.Width, TGA->ImageSpec.Height, TGA->ImageSpec.Depth, stream);
			break;

		case 24:
			TGA->Data=malloc(TGA->ImageSpec.Width*TGA->ImageSpec.Height*3);

			if(TGA->ImageType==2)
				fread(TGA->Data, sizeof(unsigned char), TGA->ImageSpec.Width*TGA->ImageSpec.Height*3, stream);

			if(TGA->ImageType==10)
				TGA_GetPackets(TGA->Data, TGA->ImageSpec.Width, TGA->ImageSpec.Height, TGA->ImageSpec.Depth, stream);
			break;

		case 8:
			if(TGA->ColorMapType==1&&TGA->ColorMapSpec.EntrySize==24)
			{
				unsigned char *buffer;
				int x, y;

				TGA->Data=malloc(TGA->ImageSpec.Width*TGA->ImageSpec.Height*3);
				buffer=malloc(TGA->ImageSpec.Width*TGA->ImageSpec.Height);

				if(TGA->ImageType==9)
					TGA_GetPackets(buffer, TGA->ImageSpec.Width, TGA->ImageSpec.Height, TGA->ImageSpec.Depth, stream);
				else
					fread(buffer, sizeof(unsigned char), TGA->ImageSpec.Width*TGA->ImageSpec.Height, stream);

				for(y=0;y<TGA->ImageSpec.Height;y++)
				{
					for(x=0;x<TGA->ImageSpec.Width;x++)
					{
						TGA->Data[3*(y*TGA->ImageSpec.Width+x)]=TGA->ColorMap[3*buffer[y*TGA->ImageSpec.Width+x]];
						TGA->Data[3*(y*TGA->ImageSpec.Width+x)+1]=TGA->ColorMap[3*buffer[y*TGA->ImageSpec.Width+x]+1];
						TGA->Data[3*(y*TGA->ImageSpec.Width+x)+2]=TGA->ColorMap[3*buffer[y*TGA->ImageSpec.Width+x]+2];
					}
				}

				free(buffer);
				free(TGA->ColorMap);

				TGA->ImageSpec.Depth=24;
				break;
			}

			TGA->Data=malloc(TGA->ImageSpec.Width*TGA->ImageSpec.Height);

			if(TGA->ImageType==3)
				fread(TGA->Data, sizeof(unsigned char), TGA->ImageSpec.Width*TGA->ImageSpec.Height, stream);

			if(TGA->ImageType==11)
				TGA_GetPackets(TGA->Data, TGA->ImageSpec.Width, TGA->ImageSpec.Height, TGA->ImageSpec.Depth, stream);
			break;

		default:
			break;
	}

	fclose(stream);

	return TRUE;
}

// Makes a DOT3 Normal map from the red channel on a TGA image (Outputs to BGR)
// Taken from ATI's DOT3 map tool
void MakeNormalMap_BGR(unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[3*(((y+1)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dY+=((float)datain[3*(((y+1)%height)*width+x%width)+2])/255.0f*-2.0f;
			dY+=((float)datain[3*(((y+1)%height)*width+((x+1)%width))+2])/255.0f*-1.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+((x-1+width)%width))+2])/255.0f*1.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+(x%width))+2])/255.0f*2.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+((x+1)%width))+2])/255.0f*1.0f;

			dX=((float)datain[3*(((y-1+height)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dX+=((float)datain[3*((y%height)*width+((x-1+width)%width))+2])/255.0f*-2.0f;
			dX+=((float)datain[3*(((y+1)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dX+=((float)datain[3*(((y-1+height)%height)*width+((x+1)%width))+2])/255.0f*1.0f;
			dX+=((float)datain[3*((y%height)*width+((x+1)%width))+2])/255.0f*2.0f;
			dX+=((float)datain[3*(((y+1)%height)*width+((x+1)%width))+2])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			dataout[3*(y*width+x)+2]=(unsigned char)((nX+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)+1]=(unsigned char)((nY+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)]=(unsigned char)((nZ+1.0f)/2.0f*255.0f);
		}
	}
}

// Makes a DOT3 Normal map from a Grayscale TGA image (Outputs to BGR)
// Taken from ATI's DOT3 map tool
void MakeNormalMap_Grayscale(unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dX=((float)datain[(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			dataout[3*(y*width+x)+2]=(unsigned char)((nX+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)+1]=(unsigned char)((nY+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)]=(unsigned char)((nZ+1.0f)/2.0f*255.0f);
		}
	}
}

// Makes a DUDV map from the red channel on a TGA image (Outputs to a 2 channel DUDV image)
// Taken from ATI's DUDV map tool
void MakeDUDVMap_BGR(unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;
	long lNX, lNY;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[3*(((y+1)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dY+=((float)datain[3*(((y+1)%height)*width+x%width)+2])/255.0f*-2.0f;
			dY+=((float)datain[3*(((y+1)%height)*width+((x+1)%width))+2])/255.0f*-1.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+((x-1+width)%width))+2])/255.0f*1.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+(x%width))+2])/255.0f*2.0f;
			dY+=((float)datain[3*(((y-1+height)%height)*width+((x+1)%width))+2])/255.0f*1.0f;

			dX=((float)datain[3*(((y-1+height)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dX+=((float)datain[3*((y%height)*width+((x-1+width)%width))+2])/255.0f*-2.0f;
			dX+=((float)datain[3*(((y+1)%height)*width+((x-1+width)%width))+2])/255.0f*-1.0f;
			dX+=((float)datain[3*(((y-1+height)%height)*width+((x+1)%width))+2])/255.0f*1.0f;
			dX+=((float)datain[3*((y%height)*width+((x+1)%width))+2])/255.0f*2.0f;
			dX+=((float)datain[3*(((y+1)%height)*width+((x+1)%width))+2])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			lNX=(long)(127.0f*nX);
			lNY=(long)(127.0f*nY);

			dataout[2*(y*width+x)]=(char)lNX;
			dataout[2*(y*width+x)+1]=(char)lNY;
		}
	}
}

// Makes a DUDV map from a grayscale TGA image (Outputs to a 2 channel DUDV image)
// Taken from ATI's DUDV map tool
void MakeDUDVMap_Grayscale(unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;
	long lNX, lNY;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dX=((float)datain[(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			lNX=(long)(127.0f*nX);
			lNY=(long)(127.0f*nY);

			dataout[2*(y*width+x)]=(char)lNX;
			dataout[2*(y*width+x)+1]=(char)lNY;
		}
	}
}

// Checks for extensions
BOOL IsExtensionSupported(const char *extension)
{
	const unsigned char *extensions=NULL, *start, *where, *terminator;

	where=strchr(extension, ' ');

	if(where||*extension=='\0')
		return FALSE;

	extensions=glGetString(GL_EXTENSIONS);
	start=extensions;

	for(;;)
	{
		where=strstr(start, extension);

		if(!where)
			break;

		terminator=where+strlen(extension);

		if(where==start||*(where-1)==' ')
			if(*terminator==' '||*terminator=='\0')
				return TRUE;

		start=terminator;
	}

	return FALSE;
}

// Uploads a plain TGA image with mipmapping
unsigned int TGA_UploadImage(char *file)
{
	unsigned int TextureID;
	TGA_t TGA;

	if(!TGA_GetData(file, &TGA))
		return 0;

	switch(TGA.ImageSpec.Depth)
	{
		case 32:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			if(!IsExtensionSupported("GL_SGIS_generate_mipmap"))
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			else
			{
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			}
			break;

		case 24:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			if(!IsExtensionSupported("GL_SGIS_generate_mipmap"))
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			else
			{
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			}
			break;

		case 8:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			if(!IsExtensionSupported("GL_SGIS_generate_mipmap"))
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, GL_ALPHA, GL_UNSIGNED_BYTE, TGA.Data);
			else
			{
				glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, TRUE);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, TGA.Data);
			}
			break;

		default:
			break;
	}

	free(TGA.Data);

	return TextureID;
}

// Uploads six images as a cube map
unsigned int TGA_UploadCube(char *right, char *left, char *top, char *bottom, char *front, char *back)
{
	unsigned int TextureID;
	TGA_t TGA_right, TGA_left, TGA_top, TGA_bottom, TGA_front, TGA_back;

	if(!TGA_GetData(right, &TGA_right))
		return 0;

	if(!TGA_GetData(left, &TGA_left))
		return 0;

	if(!TGA_GetData(top, &TGA_top))
		return 0;

	if(!TGA_GetData(bottom, &TGA_bottom))
		return 0;

	if(!TGA_GetData(front, &TGA_front))
		return 0;

	if(!TGA_GetData(back, &TGA_back))
		return 0;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, TextureID);

	if(!IsExtensionSupported("GL_EXT_texture_edge_clamp"))
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	if(!IsExtensionSupported("GL_SGIS_generate_mipmap"))
	{
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, GL_RGB8, TGA_right.ImageSpec.Width, TGA_right.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_right.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, GL_RGB8, TGA_left.ImageSpec.Width, TGA_left.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_left.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, GL_RGB8, TGA_top.ImageSpec.Width, TGA_top.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_top.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, GL_RGB8, TGA_bottom.ImageSpec.Width, TGA_bottom.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_bottom.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, GL_RGB8, TGA_front.ImageSpec.Width, TGA_front.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_front.Data);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, GL_RGB8, TGA_back.ImageSpec.Width, TGA_back.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_back.Data);
	}
	else
	{
		glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_GENERATE_MIPMAP_SGIS, TRUE);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB8, TGA_right.ImageSpec.Width, TGA_right.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_right.Data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB8, TGA_left.ImageSpec.Width, TGA_left.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_left.Data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB8, TGA_top.ImageSpec.Width, TGA_top.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_top.Data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB8, TGA_bottom.ImageSpec.Width, TGA_bottom.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_bottom.Data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB8, TGA_front.ImageSpec.Width, TGA_front.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_front.Data);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB8, TGA_back.ImageSpec.Width, TGA_back.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_back.Data);
	}

	free(TGA_right.Data);
	free(TGA_left.Data);
	free(TGA_top.Data);
	free(TGA_bottom.Data);
	free(TGA_front.Data);
	free(TGA_back.Data);

	return TextureID;
}

// Uploads a DUDV or DSDT map (ATI and nVidia ONLY)
unsigned int TGA_UploadDUDV(char *file)
{
	char *buffer;
	unsigned int TextureID;
	TGA_t TGA;
	int i=0, width, height, IsRGB;
	BOOL NVTexShader=FALSE, ATIXEMBM=FALSE, ATIEMBM=FALSE;

	if(IsExtensionSupported("GL_NV_texture_shader"))
		NVTexShader=TRUE;

	if(IsExtensionSupported("GL_ATI_envmap_bumpmap"))
		ATIEMBM=TRUE;

	if(!TGA_GetData(file, &TGA))
		return 0;

	if(TGA.ImageType!=3&&TGA.ImageSpec.Depth!=8)
		IsRGB=1;
	else
		IsRGB=0;

	buffer=malloc(TGA.ImageSpec.Width*TGA.ImageSpec.Height*2);
	width=TGA.ImageSpec.Width;
	height=TGA.ImageSpec.Height;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(NVTexShader)
	{
		if(IsRGB)
			MakeDUDVMap_BGR(TGA.Data, buffer, width, height);
		else
			MakeDUDVMap_Grayscale(TGA.Data, buffer, width, height);

		free(TGA.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DSDT8_NV, TGA.ImageSpec.Width, TGA.ImageSpec.Height, 0, GL_DSDT_NV, GL_BYTE, buffer);
	}

	if(ATIEMBM)
	{
		if(IsRGB)
			MakeDUDVMap_BGR(TGA.Data, buffer, width, height);
		else
			MakeDUDVMap_Grayscale(TGA.Data, buffer, width, height);

		free(TGA.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DU8DV8_ATI, width, height, 0, GL_DUDV_ATI, GL_BYTE, buffer);
	}

	free(buffer);

	return TextureID;
}

// Uploads a DOT3 normal map
unsigned int TGA_UploadDOT3(char *file)
{
	unsigned char *buffer;
	unsigned int TextureID;
	TGA_t TGA;
	int IsRGB;

	if(!TGA_GetData(file, &TGA))
		return 0;

	if(TGA.ImageType!=3&&TGA.ImageSpec.Depth!=8)
		IsRGB=1;
	else
		IsRGB=0;

	buffer=malloc(TGA.ImageSpec.Width*TGA.ImageSpec.Height*3);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	if(IsRGB)
		MakeNormalMap_BGR(TGA.Data, buffer, TGA.ImageSpec.Width, TGA.ImageSpec.Height);
	else
		MakeNormalMap_Grayscale(TGA.Data, buffer, TGA.ImageSpec.Width, TGA.ImageSpec.Height);

	free(TGA.Data);

	if(!IsExtensionSupported("GL_SGIS_generate_mipmap"))
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	else
	{
		glHint(GL_GENERATE_MIPMAP_HINT_SGIS, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TGA.ImageSpec.Width, TGA.ImageSpec.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	}

	free(buffer);

	return TextureID;
}

