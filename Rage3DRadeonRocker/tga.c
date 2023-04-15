#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "tga.h"

#define GL_CLAMP_TO_EDGE_EXT 0x812F

#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A

#define GL_DUDV_ATI 0x8779
#define GL_DU8DV8_ATI 0x877A

#define GL_DSDT_NV 0x86F5
#define GL_DSDT8_NV 0x8709

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	unsigned char IDLength;
	unsigned char ColorMapType;
	unsigned char ImageType;
	unsigned short ColorMapStart;
	unsigned short ColorMapLength;
	unsigned char ColorMapDepth;
	unsigned short XOffset;
	unsigned short YOffset;
	unsigned short Width;
	unsigned short Height;
	unsigned char Depth;
	unsigned char ImageDescriptor;
	unsigned char *ColorMap;
	unsigned char *Data;
} TGA_t;

#pragma pack(pop)

void TGA_GetPackets(unsigned char *data, int width, int height, int depth, FILE *stream)
{
	int current_byte=0, run_length, i;
	unsigned char header, buffer8[4];
	unsigned short buffer16;
	int bpp;

	if(depth==16)
		bpp=3;
	else
		bpp=depth/8;

	while(current_byte<width*height*bpp)
	{
		fread(&header, sizeof(unsigned char), 1, stream);

		run_length=(header&0x7F)+1;

		if(header&0x80)
		{
			if(depth==32)
				fread(buffer8, sizeof(unsigned char), 4, stream);

			if(depth==24)
				fread(buffer8, sizeof(unsigned char), 3, stream);

			if(depth==16)
				fread(&buffer16, sizeof(unsigned short), 1, stream);

			if(depth==8)
				fread(buffer8, sizeof(unsigned char), 1, stream);

			for(i=0;i<run_length;i++)
			{
				if(depth==32)
				{
					data[current_byte++]=buffer8[0];
					data[current_byte++]=buffer8[1];
					data[current_byte++]=buffer8[2];
					data[current_byte++]=buffer8[3];
				}

				if(depth==24)
				{
					data[current_byte++]=buffer8[0];
					data[current_byte++]=buffer8[1];
					data[current_byte++]=buffer8[2];
				}

				if(depth==16)
				{
					data[current_byte++]=(buffer16&0x1F)<<3;
					data[current_byte++]=((buffer16>>5)&0x1F)<<3;
					data[current_byte++]=((buffer16>>10)&0x1F)<<3;
				}

				if(depth==8)
					data[current_byte++]=buffer8[0];
			}
		}

		if(!(header&0x80))
		{
			for(i=0;i<run_length;i++)
			{
				if(depth==32)
				{
					fread(buffer8, sizeof(unsigned char), 4, stream);
					data[current_byte++]=buffer8[0];
					data[current_byte++]=buffer8[1];
					data[current_byte++]=buffer8[2];
					data[current_byte++]=buffer8[3];
				}

				if(depth==24)
				{
					fread(buffer8, sizeof(unsigned char), 3, stream);
					data[current_byte++]=buffer8[0];
					data[current_byte++]=buffer8[1];
					data[current_byte++]=buffer8[2];
				}

				if(depth==16)
				{
					fread(&buffer16, sizeof(unsigned short), 1, stream);
					data[current_byte++]=(buffer16&0x1F)<<3;
					data[current_byte++]=((buffer16>>5)&0x1F)<<3;
					data[current_byte++]=((buffer16>>10)&0x1F)<<3;
				}

				if(depth==8)
				{
					fread(buffer8, sizeof(unsigned char), 1, stream);
					data[current_byte++]=buffer8[0];
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

	fread(&TGA->IDLength, sizeof(unsigned char), 1, stream);
	fread(&TGA->ColorMapType, sizeof(unsigned char), 1, stream);
	fread(&TGA->ImageType, sizeof(unsigned char), 1, stream);
	fread(&TGA->ColorMapStart, sizeof(unsigned short), 1, stream);
	fread(&TGA->ColorMapLength, sizeof(unsigned short), 1, stream);
	fread(&TGA->ColorMapDepth, sizeof(unsigned char), 1, stream);
	fread(&TGA->XOffset, sizeof(unsigned short), 1, stream);
	fread(&TGA->XOffset, sizeof(unsigned short), 1, stream);
	fread(&TGA->Width, sizeof(unsigned short), 1, stream);
	fread(&TGA->Height, sizeof(unsigned short), 1, stream);
	fread(&TGA->Depth, sizeof(unsigned char), 1, stream);
	fread(&TGA->ImageDescriptor, sizeof(unsigned char), 1, stream);

	fseek(stream, TGA->IDLength, SEEK_CUR);

	switch(TGA->ImageType)
	{
		case 1:
			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
				TGA->ColorMap=(unsigned char *)malloc(TGA->ColorMapLength*(TGA->ColorMapDepth/8));
				fread(TGA->ColorMap, sizeof(unsigned char), TGA->ColorMapLength*(TGA->ColorMapDepth/8), stream);
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
			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
				TGA->ColorMap=(unsigned char *)malloc(TGA->ColorMapLength*(TGA->ColorMapDepth/8));
				fread(TGA->ColorMap, sizeof(unsigned char), TGA->ColorMapLength*(TGA->ColorMapDepth/8), stream);
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

	switch(TGA->Depth)
	{
		case 32:
			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*4);

			if(TGA->ImageType==2)
			{
				fread(TGA->Data, sizeof(unsigned char), TGA->Width*TGA->Height*4, stream);
				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);
				break;
			}
			break;

		case 24:
			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);

			if(TGA->ImageType==2)
			{
				fread(TGA->Data, sizeof(unsigned char), TGA->Width*TGA->Height*3, stream);
				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);
				break;
			}
			break;

		case 16:
			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);

			if(TGA->ImageType==2)
			{
				unsigned short *buffer=(unsigned short *)malloc(sizeof(unsigned short)*TGA->Width*TGA->Height);
				int i;

				fread(buffer, sizeof(unsigned short), TGA->Width*TGA->Height, stream);

				for(i=0;i<TGA->Width*TGA->Height;i++)
				{
					TGA->Data[3*i]=(buffer[i]&0x1F)<<3;
					TGA->Data[3*i+1]=((buffer[i]>>5)&0x1F)<<3;
					TGA->Data[3*i+2]=((buffer[i]>>10)&0x1F)<<3;
				}

				free(buffer);
				TGA->Depth=24;
				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);
				TGA->Depth=24;
				break;
			}
			break;

		case 8:
			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
				unsigned char *buffer;
				int i;

				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);
				buffer=(unsigned char *)malloc(TGA->Width*TGA->Height);

				if(TGA->ImageType==9)
					TGA_GetPackets(buffer, TGA->Width, TGA->Height, TGA->Depth, stream);
				else
					fread(buffer, sizeof(unsigned char), TGA->Width*TGA->Height, stream);

				for(i=0;i<TGA->Width*TGA->Height;i++)
				{
					TGA->Data[3*i]=TGA->ColorMap[3*buffer[i]];
					TGA->Data[3*i+1]=TGA->ColorMap[3*buffer[i]+1];
					TGA->Data[3*i+2]=TGA->ColorMap[3*buffer[i]+2];
				}

				free(buffer);
				free(TGA->ColorMap);

				TGA->Depth=24;
				break;
			}

			if(TGA->ImageType==3)
			{
				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height);
				fread(TGA->Data, sizeof(unsigned char), TGA->Width*TGA->Height, stream);
				break;
			}

			if(TGA->ImageType==11)
			{
				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height);
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);
				break;
			}
			break;

		default:
			MessageBox(NULL, "Unsupported image depth.", "TGA", MB_OK);
			break;
	}

	fclose(stream);

	return TRUE;
}

// Makes a normal map from a greyscale (or blue channel if BGR[A]) image
// Taken from ATI's DOT3 map tool
void MakeNormalMap(int components, unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dX=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[components*((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			dataout[3*(y*width+x)]=(unsigned char)((nX+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)+1]=(unsigned char)((nY+1.0f)/2.0f*255.0f);
			dataout[3*(y*width+x)+2]=(unsigned char)((nZ+1.0f)/2.0f*255.0f);
		}
	}
}

// Makes a DUDV map from a greyscale (or blue channel if BGR[A]) image.
// Taken from ATI's DUDV map tool.
void MakeDUDVMap(int components, unsigned char *datain, char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, len;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dY=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dX=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[components*((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1;

			len=1.0f/((float)sqrt((nX*nX)+(nY*nY)+(nZ*nZ)));

			nX*=len;
			nY*=len;
			nZ*=len;

			dataout[2*(y*width+x)]=(char)(127.0f*nX);
			dataout[2*(y*width+x)+1]=(char)(127.0f*nY);
		}
	}
}

// Checks for extensions
BOOL IsExtensionSupported(const char *extension)
{
	const unsigned char *extensions=NULL, *start, *where, *terminator;

	where=(unsigned char *)strchr(extension, ' ');

	if(where||*extension=='\0')
		return FALSE;

	extensions=glGetString(GL_EXTENSIONS);
	start=extensions;

	for(;;)
	{
		where=(unsigned char *)strstr((char *)start, extension);

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

// Uploads a plain TGA image
unsigned int TGA_UploadImage(char *file)
{
	unsigned int TextureID;
	TGA_t TGA;

	if(!TGA_GetData(file, &TGA))
		return 0;

	switch(TGA.Depth)
	{
		case 32:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, TGA.Width, TGA.Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			break;

		case 24:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TGA.Width, TGA.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA.Data);
			break;

		case 8:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, TGA.Width, TGA.Height, GL_ALPHA, GL_UNSIGNED_BYTE, TGA.Data);
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

	if(!IsExtensionSupported("GL_ARB_texture_cube_map"))
	{
		MessageBox(NULL, "GL_ARB_texture_cube_map not supported.", "TGA", MB_OK);
		return 0;
	}

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

	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, GL_RGB8, TGA_right.Width, TGA_right.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_right.Data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, GL_RGB8, TGA_left.Width, TGA_left.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_left.Data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, GL_RGB8, TGA_top.Width, TGA_top.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_top.Data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, GL_RGB8, TGA_bottom.Width, TGA_bottom.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_bottom.Data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, GL_RGB8, TGA_front.Width, TGA_front.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_front.Data);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, GL_RGB8, TGA_back.Width, TGA_back.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, TGA_back.Data);

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
	int i=0, width, height;
	BOOL NVTexShader=FALSE, ATIEMBM=FALSE;

	if(IsExtensionSupported("GL_NV_texture_shader"))
		NVTexShader=TRUE;

	if(IsExtensionSupported("GL_ATI_envmap_bumpmap"))
		ATIEMBM=TRUE;

	if(!(NVTexShader||ATIEMBM))
		return 0;

	if(!TGA_GetData(file, &TGA))
		return 0;

	buffer=(char *)malloc(TGA.Width*TGA.Height*2);
	width=TGA.Width;
	height=TGA.Height;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(NVTexShader)
	{
		MakeDUDVMap(TGA.Depth/8, TGA.Data, buffer, width, height);

		free(TGA.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DSDT8_NV, TGA.Width, TGA.Height, 0, GL_DSDT_NV, GL_BYTE, buffer);
	}

	if(ATIEMBM)
	{
		MakeDUDVMap(TGA.Depth/8, TGA.Data, buffer, width, height);

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

	if(!TGA_GetData(file, &TGA))
		return 0;

	buffer=(unsigned char *)malloc(TGA.Width*TGA.Height*3);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	MakeNormalMap(TGA.Depth/8, TGA.Data, buffer, TGA.Width, TGA.Height);

	free(TGA.Data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, TGA.Width, TGA.Height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
}

void FlipImage(unsigned char *Data, int Width, int Height)
{
	int x, y, flipx, flipy;
	unsigned char *buffer=(unsigned char *)malloc(Width*Height*3);

	for(y=0;y<Height;y++)
	{
		flipy=Height-1-y;

		for(x=0;x<Width;x++)
		{
			flipx=Width-1-x;

			buffer[3*(flipy*Width+flipx)]=Data[3*(y*Width+x)];
			buffer[3*(flipy*Width+flipx)+1]=Data[3*(y*Width+x)+1];
			buffer[3*(flipy*Width+flipx)+2]=Data[3*(y*Width+x)+2];
		}
	}

	memcpy(Data, buffer, Width*Height*3);
	free(buffer);
}

void GetBlock(unsigned char *DataIn, unsigned char *DataOut, int StartX, int StartY, int InWidth, int OutWidth, int OutHeight)
{
	int x, y;

	for(y=0;y<OutHeight;y++)
	{
		for(x=0;x<OutWidth;x++)
		{
			DataOut[3*(y*OutWidth+x)]=DataIn[3*((StartY+y)*InWidth+(StartX+x))];
			DataOut[3*(y*OutWidth+x)+1]=DataIn[3*((StartY+y)*InWidth+(StartX+x))+1];
			DataOut[3*(y*OutWidth+x)+2]=DataIn[3*((StartY+y)*InWidth+(StartX+x))+2];
		}
	}
}

unsigned int TGA_UploadCubeVirtCross(char *file)
{
	unsigned int TextureID;
	TGA_t TGA;
	unsigned char *buffer;
	int FaceWidth, FaceHeight;

	if(!TGA_GetData(file, &TGA))
		return 0;

	if(TGA.ColorMapType==1)
	{
		if(TGA.ColorMapDepth!=24)
			return 0;
	}
	else
		if(TGA.Depth!=24)
			return 0;

	FaceWidth=TGA.Width/3;
	FaceHeight=TGA.Height/4;

	buffer=(unsigned char *)malloc(FaceWidth*FaceHeight*3);

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

	GetBlock(TGA.Data, buffer, FaceWidth*0, FaceHeight*2, TGA.Width, FaceWidth, FaceHeight);
	FlipImage(buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	GetBlock(TGA.Data, buffer, FaceWidth*2, FaceHeight*2, TGA.Width, FaceWidth, FaceHeight);
	FlipImage(buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	GetBlock(TGA.Data, buffer, FaceWidth*1, FaceHeight*3, TGA.Width, FaceWidth, FaceHeight);
	FlipImage(buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	GetBlock(TGA.Data, buffer, FaceWidth*1, FaceHeight*1, TGA.Width, FaceWidth, FaceHeight);
	FlipImage(buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	GetBlock(TGA.Data, buffer, FaceWidth*1, FaceHeight*2, TGA.Width, FaceWidth, FaceHeight);
	FlipImage(buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	GetBlock(TGA.Data, buffer, FaceWidth*1, FaceHeight*0, TGA.Width, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, GL_RGB8, FaceWidth, FaceHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);

	free(buffer);
	free(TGA.Data);

	return TextureID;
}