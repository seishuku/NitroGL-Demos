#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <malloc.h>
#include <math.h>
#include "glati.h"
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB=NULL;

#ifndef GL_DSDT_NV
#define GL_DSDT_NV 0x86F5
#endif

#ifndef GL_DSDT8_NV
#define GL_DSDT8_NV 0x8709
#endif

BOOL IMAGE_IsExtensionSupported(const char *extension)
{
	const char *extensions=NULL, *start, *where, *terminator;

	where=strchr(extension, ' ');

	if(where||*extension=='\0')
		return FALSE;

	extensions=(const char *)glGetString(GL_EXTENSIONS);
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

void IMAGE_MakeNormalMap(int channels, unsigned char *src, int width, int height, unsigned char *dstubyte, unsigned short *dstushort)
{
	int x, y;
	float scale[3]={ 4.0f, 4.0f, 1.0f };
	const float OneOver255=1.0f/255.0f;

	for(y=0;y<height;y++)
	{
		int y1=((y+1)>=height)?(y1=y):(y1=y+1);

		for(x=0;x<width;x++)
		{
			int x1=((x+1)>=width)?(x1=x):(x1=x+1);
			float v0[3]={ 2.0f, 0.0f, -(float)((src[channels*(y*width+x)]-src[channels*(y*width+x1)])*OneOver255) };
			float v1[3]={ 0.0f, 2.0f, (float)((src[channels*(y*width+x)]-src[channels*(y1*width+x)])*OneOver255) };
			float n[3]={ (v0[1]*v1[2]-v0[2]*v1[1]), (v0[2]*v1[0]-v0[0]*v1[2]), (v0[0]*v1[1]-v0[1]*v1[0]) }, mag;

			n[0]*=scale[0];
			n[1]*=scale[1];
			n[2]*=scale[2];

			mag=(float)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);

			if(mag)
			{
				mag=1.0f/mag;

				n[0]*=mag;
				n[1]*=mag;
				n[2]*=mag;
			}

			if(dstubyte)
			{
				dstubyte[4*(y*width+x)]=(unsigned char)(255.0f*(n[0]*0.5f+0.5f));
				dstubyte[4*(y*width+x)+1]=(unsigned char)(255.0f*(n[1]*0.5f+0.5f));
				dstubyte[4*(y*width+x)+2]=(unsigned char)(255.0f*(n[2]*0.5f+0.5f));

				if(channels==4)
					dstubyte[4*(y*width+x)+3]=src[4*(y*width+x)+3];
				else
					dstubyte[4*(y*width+x)+3]=255;
			}

			if(dstushort)
			{
				dstushort[4*(y*width+x)]=(unsigned short)(65535.0f*(n[0]*0.5f+0.5f));
				dstushort[4*(y*width+x)+1]=(unsigned short)(65535.0f*(n[1]*0.5f+0.5f));
				dstushort[4*(y*width+x)+2]=(unsigned short)(65535.0f*(n[2]*0.5f+0.5f));

				if(channels==4)
					dstushort[4*(y*width+x)+3]=(unsigned short)(src[4*(y*width+x)+3]*255);
				else
					dstushort[4*(y*width+x)+3]=65535;
			}
		}
	}
}

unsigned int IMAGE_UploadImage(char *filename)
{
	unsigned int TextureID;
	IMAGE_t Image;

	if(!JPEG_Load(filename, &Image))
	{
		if(!DDS_Load(filename, &Image))
		{
			if(!TGA_Load(filename, &Image))
				return 0;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	switch(Image.Depth)
	{
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			if(IMAGE_IsExtensionSupported("GL_ARB_texture_compression")&&IMAGE_IsExtensionSupported("GL_EXT_texture_compression_s3tc"))
			{
				if(glCompressedTexImage2DARB==NULL)
					glCompressedTexImage2DARB=(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IMAGE_IsExtensionSupported("GL_SGIS_generate_mipmap"))
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
				}
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*16, Image.Data);
			}
			break;

		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			if(IMAGE_IsExtensionSupported("GL_ARB_texture_compression")&&IMAGE_IsExtensionSupported("GL_EXT_texture_compression_s3tc"))
			{
				if(glCompressedTexImage2DARB==NULL)
					glCompressedTexImage2DARB=(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IMAGE_IsExtensionSupported("GL_SGIS_generate_mipmap"))
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
				}
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*16, Image.Data);
			}
			break;

		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
			if(IMAGE_IsExtensionSupported("GL_ARB_texture_compression")&&IMAGE_IsExtensionSupported("GL_EXT_texture_compression_s3tc"))
			{
				if(glCompressedTexImage2DARB==NULL)
					glCompressedTexImage2DARB=(PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress("glCompressedTexImage2DARB");

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IMAGE_IsExtensionSupported("GL_SGIS_generate_mipmap"))
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
				}
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*8, Image.Data);
			}
			break;

		case 32:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, Image.Width, Image.Height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Image.Data);
			break;

		case 24:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image.Width, Image.Height, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.Data);
			break;

		case 8:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, Image.Width, Image.Height, GL_ALPHA, GL_UNSIGNED_BYTE, Image.Data);
			break;

		default:
			FREE(Image.Data);
			break;
	}

	FREE(Image.Data);

	return TextureID;
}

unsigned int IMAGE_UploadDOT3(char *filename)
{
	unsigned char *buffer;
	unsigned int TextureID;
	IMAGE_t Image;

	if(!JPEG_Load(filename, &Image))
	{
		if(!DDS_Load(filename, &Image))
		{
			if(!TGA_Load(filename, &Image))
				return 0;
		}
	}

	if(!((Image.Depth==32)||(Image.Depth==24)||(Image.Depth==8)))
		return 0;

	buffer=(unsigned char *)malloc(Image.Width*Image.Height*4);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	IMAGE_MakeNormalMap(Image.Depth/8, Image.Data, Image.Width, Image.Height, buffer, NULL);

	FREE(Image.Data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, Image.Width, Image.Height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	FREE(buffer);

	return TextureID;
}

unsigned int IMAGE_UploadDOT3_64Bit(char *filename)
{
	unsigned short *buffer;
	unsigned int TextureID;
	IMAGE_t Image;

	if(!JPEG_Load(filename, &Image))
	{
		if(!DDS_Load(filename, &Image))
		{
			if(!TGA_Load(filename, &Image))
				return 0;
		}
	}

	if(!((Image.Depth==32)||(Image.Depth==24)||(Image.Depth==8)))
		return 0;

	buffer=(unsigned short *)malloc(sizeof(unsigned short)*Image.Width*Image.Height*4);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	IMAGE_MakeNormalMap(Image.Depth/8, Image.Data, Image.Width, Image.Height, NULL, buffer);

	FREE(Image.Data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA16, Image.Width, Image.Height, GL_RGBA, GL_UNSIGNED_SHORT, buffer);

	FREE(buffer);

	return TextureID;
}

unsigned int IMAGE_UploadNormalMap_64Bit(char *filename)
{
	unsigned short *buffer;
	unsigned int TextureID;
	IMAGE_t Image;
	int i, Components;
	const float OneOver255=1.0f/255.0f;

	if(!JPEG_Load(filename, &Image))
	{
		if(!DDS_Load(filename, &Image))
		{
			if(!TGA_Load(filename, &Image))
				return 0;
		}
	}

	if(!((Image.Depth==32)||(Image.Depth==24)))
		return 0;

	Components=Image.Depth/8;

	buffer=(unsigned short *)malloc(sizeof(unsigned short)*Image.Width*Image.Height*4);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	for(i=0;i<Image.Width*Image.Height;i++)
	{
		float n[3], mag;

		n[0]=(float)(Image.Data[Components*i+2]*OneOver255)*2.0f-1.0f;
		n[1]=(float)(Image.Data[Components*i+1]*OneOver255)*2.0f-1.0f;
		n[2]=(float)(Image.Data[Components*i]*OneOver255)*2.0f-1.0f;

		mag=(float)sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);

		if(mag)
		{
			mag=1.0f/mag;

			n[0]*=mag;
			n[1]*=mag;
			n[2]*=mag;
		}

		buffer[4*i]=(unsigned short)(65535.0f*(n[0]*0.5f+0.5f));
		buffer[4*i+1]=(unsigned short)(65535.0f*(n[1]*0.5f+0.5f));
		buffer[4*i+2]=(unsigned short)(65535.0f*(n[2]*0.5f+0.5f));

		if(Components==4)
			buffer[4*i+3]=(unsigned short)(Image.Data[4*i+3]*255);
		else
			buffer[4*i+3]=65535;
	}

	FREE(Image.Data);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA16, Image.Width, Image.Height, GL_RGBA, GL_UNSIGNED_SHORT, buffer);

	FREE(buffer);

	return TextureID;
}

void IMAGE_FlipImage(int Components, unsigned char *Data, int Width, int Height)
{
	int x, y, i, flipx, flipy;
	unsigned char *buffer=(unsigned char *)malloc(Width*Height*Components);

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
	FREE(buffer);
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

	if(!IMAGE_IsExtensionSupported("GL_ARB_texture_cube_map"))
		return 0;

	if(!JPEG_Load(filename, &Image))
	{
		if(!DDS_Load(filename, &Image))
		{
			if(!TGA_Load(filename, &Image))
				return 0;
		}
	}

	if(!((Image.Depth==32)||(Image.Depth==24)||(Image.Depth==8)))
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

	if(!IMAGE_IsExtensionSupported("GL_EXT_texture_edge_clamp"))
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

	FREE(buffer);
	FREE(Image.Data);

	return TextureID;
}
