#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <malloc.h>
#include <math.h>
#include "glati.h"
#include "image.h"

#ifndef GL_DSDT_NV
#define GL_DSDT_NV 0x86F5
#endif

#ifndef GL_DSDT8_NV
#define GL_DSDT8_NV 0x8709
#endif

BOOL IsExtensionSupported(const char *extension);

void IMAGE_MakeNormalMap(int components, unsigned char *datain, unsigned char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, n[3], mag;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dX=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[components*((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dY=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			n[0]=-dX;
			n[1]=-dY;
			n[2]=1.0f;

			mag=1/(float)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
			n[0]*=mag;
			n[1]*=mag;
			n[2]*=mag;

			dataout[3*(y*width+x)+0]=(unsigned char)(n[0]*127.0f+128.0f);
			dataout[3*(y*width+x)+1]=(unsigned char)(n[1]*127.0f+128.0f);
			dataout[3*(y*width+x)+2]=(unsigned char)(n[2]*127.0f+128.0f);
		}
	}
}

void IMAGE_MakeDUDVMap(int components, unsigned char *datain, char *dataout, int width, int height)
{
	int x, y;
	float dX, dY, nX, nY, nZ, mag;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			dX=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*((y%height)*width+((x-1+width)%width))])/255.0f*-2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dX+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;
			dX+=((float)datain[components*((y%height)*width+((x+1)%width))])/255.0f*2.0f;
			dX+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*1.0f;

			dY=((float)datain[components*(((y+1)%height)*width+((x-1+width)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+x%width)])/255.0f*-2.0f;
			dY+=((float)datain[components*(((y+1)%height)*width+((x+1)%width))])/255.0f*-1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x-1+width)%width))])/255.0f*1.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+(x%width))])/255.0f*2.0f;
			dY+=((float)datain[components*(((y-1+height)%height)*width+((x+1)%width))])/255.0f*1.0f;

			nX=-dX;
			nY=-dY;
			nZ=1.0f;

			mag=1/(float)sqrt(nX*nX+nY*nY+nZ*nZ);
			nX*=mag;
			nY*=mag;
			nZ*=mag;

			dataout[2*(y*width+x)]=(char)(127.0f*nX);
			dataout[2*(y*width+x)+1]=(char)(127.0f*nY);
		}
	}
}

unsigned int IMAGE_UploadImage(char *filename)
{
	unsigned int TextureID;
	IMAGE_t Image;

	if(!TGA_Load(filename, &Image))
		if(!JPEG_Load(filename, &Image))
			return 0;

	switch(Image.Depth)
	{
		case 32:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, Image.Width, Image.Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Image.Data);
			break;

		case 24:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image.Width, Image.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.Data);
			break;

		case 8:
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &TextureID);
			glBindTexture(GL_TEXTURE_2D, TextureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, Image.Width, Image.Height, GL_ALPHA, GL_UNSIGNED_BYTE, Image.Data);
			break;

		default:
			break;
	}

	free(Image.Data);

	return TextureID;
}

unsigned int IMAGE_UploadDUDV(char *filename)
{
	char *buffer;
	unsigned int TextureID;
	IMAGE_t Image;
	BOOL NVTexShader=FALSE, ATIEMBM=FALSE;

	if(IsExtensionSupported("GL_NV_texture_shader"))
		NVTexShader=TRUE;

	if(IsExtensionSupported("GL_ATI_envmap_bumpmap"))
		ATIEMBM=TRUE;

	if(!(NVTexShader||ATIEMBM))
		return 0;

	if(!TGA_Load(filename, &Image))
		if(!JPEG_Load(filename, &Image))
			return 0;

	buffer=(char *)malloc(Image.Width*Image.Height*2);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(NVTexShader)
	{
		IMAGE_MakeDUDVMap(Image.Depth/8, Image.Data, buffer, Image.Width, Image.Height);

		free(Image.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DSDT8_NV, Image.Width, Image.Height, 0, GL_DSDT_NV, GL_BYTE, buffer);
	}

	if(ATIEMBM)
	{
		IMAGE_MakeDUDVMap(Image.Depth/8, Image.Data, buffer, Image.Width, Image.Height);

		free(Image.Data);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DU8DV8_ATI, Image.Width, Image.Height, 0, GL_DUDV_ATI, GL_BYTE, buffer);
	}

	free(buffer);

	return TextureID;
}

unsigned int IMAGE_UploadDOT3(char *filename)
{
	unsigned char *buffer;
	unsigned int TextureID;
	IMAGE_t Image;

	if(!TGA_Load(filename, &Image))
		if(!JPEG_Load(filename, &Image))
			return 0;

	buffer=(unsigned char *)malloc(Image.Width*Image.Height*3);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	IMAGE_MakeNormalMap(Image.Depth/8, Image.Data, buffer, Image.Width, Image.Height);

	free(Image.Data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image.Width, Image.Height, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
}

void IMAGE_FlipImage(int Components, unsigned char *Data, int Width, int Height)
{
	int x, y, i, flipx, flipy;
	unsigned char *buffer=(unsigned char *)malloc(Width*Height*3);

	for(y=0;y<Height;y++)
	{
		flipy=Height-1-y;

		for(x=0;x<Width;x++)
		{
			flipx=Width-1-x;

			for(i=0;i<Components;i++)
				buffer[Components*(flipy*Width+flipx)+i]=Data[Components*(y*Width+x)+i];
		}
	}

	memcpy(Data, buffer, Width*Height*3);
	free(buffer);
}

void IMAGE_GetBlock(int Components, unsigned char *DataIn, unsigned char *DataOut, int StartX, int StartY, int InWidth, int OutWidth, int OutHeight)
{
	int x, y, i;

	for(y=0;y<OutHeight;y++)
	{
		for(x=0;x<OutWidth;x++)
		{
			for(i=0;i<Components;i++)
				DataOut[Components*(y*OutWidth+x)+i]=DataIn[Components*((StartY+y)*InWidth+(StartX+x))+i];
		}
	}
}

unsigned int IMAGE_UploadCubeVirtCross(char *filename)
{
	unsigned int TextureID;
	IMAGE_t Image;
	unsigned char *buffer;
	int FaceWidth, FaceHeight, Components;
	unsigned long InternalFormat, Format;

	if(!TGA_Load(filename, &Image))
		if(!JPEG_Load(filename, &Image))
			return 0;

	FaceWidth=Image.Width/3;
	FaceHeight=Image.Height/4;

	switch(Image.Depth)
	{
		case 8:
			Components=1;
			InternalFormat=GL_ALPHA8;
			Format=GL_ALPHA;
			break;

		case 24:
			Components=3;
			InternalFormat=GL_RGB8;
			Format=GL_BGR_EXT;
			break;

		case 32:
			Components=4;
			InternalFormat=GL_RGBA8;
			Format=GL_BGRA_EXT;
			break;

		default:
			return 0;
			break;
	}

	buffer=(unsigned char *)malloc(FaceWidth*FaceHeight*Components);

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

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*0, FaceHeight*2, Image.Width, FaceWidth, FaceHeight);
	IMAGE_FlipImage(Components, buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*2, FaceHeight*2, Image.Width, FaceWidth, FaceHeight);
	IMAGE_FlipImage(Components, buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*1, FaceHeight*3, Image.Width, FaceWidth, FaceHeight);
	IMAGE_FlipImage(Components, buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*1, FaceHeight*1, Image.Width, FaceWidth, FaceHeight);
	IMAGE_FlipImage(Components, buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*1, FaceHeight*2, Image.Width, FaceWidth, FaceHeight);
	IMAGE_FlipImage(Components, buffer, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	IMAGE_GetBlock(Components, Image.Data, buffer, FaceWidth*1, FaceHeight*0, Image.Width, FaceWidth, FaceHeight);
	gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, InternalFormat, FaceWidth, FaceHeight, Format, GL_UNSIGNED_BYTE, buffer);

	free(buffer);
	free(Image.Data);

	return TextureID;
}
