#include <windows.h>
#include <malloc.h>
#include <stdio.h>
#include "math.h"
#include "opengl.h"
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void _MakeNormalMap(Image_t *Image)
{
	int x, y, xx, yy;
	int Channels=Image->Depth>>3;
	unsigned short *Buffer=NULL;
	const float OneOver255=1.0f/255.0f;
	float KernelX[9]={ 1.0f, 0.0f, -1.0f, 2.0f, 0.0f, -2.0f, 1.0f, 0.0f, -1.0f };
	float KernelY[9]={ -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f };

	if(!((Image->Depth==32)||(Image->Depth==24)||(Image->Depth==8)))
		return;

	Buffer=(unsigned short *)malloc(sizeof(unsigned short)*Image->Width*Image->Height*4);

	if(Buffer==NULL)
		return;

	for(y=0;y<Image->Height;y++)
	{
		int oy=min(Image->Height-1, y+1);

		for(x=0;x<Image->Width;x++)
		{
			int ox=min(Image->Width-1, x+1);
			float n[3]={ 0.0f, 0.0f, 1.0f }, mag;

			for(yy=0;yy<3;yy++)
			{
				int oy=min(Image->Height-1, y+yy);

				for(xx=0;xx<3;xx++)
				{
					int ox=min(Image->Width-1, x+xx);

					n[0]+=KernelX[yy*3+xx]*(float)(Image->Data[Channels*(oy*Image->Width+ox)]*OneOver255);
					n[1]+=KernelY[yy*3+xx]*(float)(Image->Data[Channels*(oy*Image->Width+ox)]*OneOver255);
				}
			}

			mag=1.0f/sqrtf(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
			n[0]*=mag;
			n[1]*=mag;
			n[2]*=mag;

			Buffer[4*(y*Image->Width+x)+0]=(unsigned short)(65535.0f*(n[0]*0.5f+0.5f));
			Buffer[4*(y*Image->Width+x)+1]=(unsigned short)(65535.0f*(n[1]*0.5f+0.5f));
			Buffer[4*(y*Image->Width+x)+2]=(unsigned short)(65535.0f*(n[2]*0.5f+0.5f));

			if(Channels==4)
				Buffer[4*(y*Image->Width+x)+3]=(unsigned short)(Image->Data[4*(y*Image->Width+x)+3]<<8);
			else
				Buffer[4*(y*Image->Width+x)+3]=65535;
		}
	}

	Image->Depth=64;

	FREE(Image->Data);
	Image->Data=(unsigned char *)Buffer;
}

void _Normalize(Image_t *Image)
{
	int i, Channels=Image->Depth>>3;
	unsigned short *Buffer=NULL;
	const float OneOver255=1.0f/255.0f;

	if(!((Image->Depth==32)||(Image->Depth==24)))
		return;

	Buffer=(unsigned short *)malloc(sizeof(unsigned short)*Image->Width*Image->Height*4);

	if(Buffer==NULL)
		return;

	for(i=0;i<Image->Width*Image->Height;i++)
	{
		float n[3], mag;

		n[0]=(float)(Image->Data[Channels*i+2]*OneOver255)*2.0f-1.0f;
		n[1]=(float)(Image->Data[Channels*i+1]*OneOver255)*2.0f-1.0f;
		n[2]=(float)(Image->Data[Channels*i+0]*OneOver255)*2.0f-1.0f;

		mag=1.0f/sqrtf(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
		n[0]*=mag;
		n[1]*=mag;
		n[2]*=mag;

		Buffer[4*i+0]=(unsigned short)(65535.0f*(n[0]*0.5f+0.5f));
		Buffer[4*i+1]=(unsigned short)(65535.0f*(n[1]*0.5f+0.5f));
		Buffer[4*i+2]=(unsigned short)(65535.0f*(n[2]*0.5f+0.5f));

		if(Channels==4)
			Buffer[4*i+3]=(unsigned short)(Image->Data[4*i+3]<<8);
		else
			Buffer[4*i+3]=65535;
	}

	Image->Depth=64;

	FREE(Image->Data);
	Image->Data=(unsigned char *)Buffer;
}

void _Resample(Image_t *Src, Image_t *Dst)
{
	float fx, fy, hx, hy, lx, ly, sx, sy;
	float xPercent, yPercent, Percent;
	float Total[4], Sum;
	int x, y, iy, ix, Index;

	if(Dst->Data==NULL)
		return;

	sx=(float)Src->Width/Dst->Width;
	sy=(float)Src->Height/Dst->Height;

	for(y=0;y<Dst->Height;y++)
	{
		if(Src->Height>Dst->Height)
		{
			fy=((float)y+0.5f)*sy;
			hy=fy+(sy*0.5f);
			ly=fy-(sy*0.5f);
		}
		else
		{
			fy=(float)y*sy;
			hy=fy+0.5f;
			ly=fy-0.5f;
		}

		for(x=0;x<Dst->Width;x++)
		{
			if(Src->Width>Dst->Width)
			{
				fx=((float)x+0.5f)*sx;
				hx=fx+(sx*0.5f);
				lx=fx-(sx*0.5f);
			}
			else
			{
				fx=(float)x*sx;
				hx=fx+0.5f;
				lx=fx-0.5f;
			}

			Total[0]=Total[1]=Total[2]=Total[3]=Sum=0.0f;

			fy=ly;
			iy=(int)fy;

			while(fy<hy)
			{
				if(hy<iy+1)
					yPercent=hy-fy;
				else
					yPercent=(iy+1)-fy;

				fx=lx;
				ix=(int)fx;

				while(fx<hx)
				{
					if(hx<ix+1)
						xPercent=hx-fx;
					else
						xPercent=(ix+1)-fx;

					Percent=xPercent*xPercent;
					Sum+=Percent;

					Index=min(Src->Height-1, iy)*Src->Width+min(Src->Width-1, ix);

					switch(Src->Depth)
					{
						case 128:
							Total[0]+=((float *)Src->Data)[4*Index+0]*Percent;
							Total[1]+=((float *)Src->Data)[4*Index+1]*Percent;
							Total[2]+=((float *)Src->Data)[4*Index+2]*Percent;
							Total[3]+=((float *)Src->Data)[4*Index+3]*Percent;
							break;

						case 96:
							Total[0]+=((float *)Src->Data)[3*Index+0]*Percent;
							Total[1]+=((float *)Src->Data)[3*Index+1]*Percent;
							Total[2]+=((float *)Src->Data)[3*Index+2]*Percent;
							break;

						case 64:
							Total[0]+=((unsigned short *)Src->Data)[4*Index+0]*Percent;
							Total[1]+=((unsigned short *)Src->Data)[4*Index+1]*Percent;
							Total[2]+=((unsigned short *)Src->Data)[4*Index+2]*Percent;
							Total[3]+=((unsigned short *)Src->Data)[4*Index+3]*Percent;
							break;

						case 48:
							Total[0]+=((unsigned short *)Src->Data)[3*Index+0]*Percent;
							Total[1]+=((unsigned short *)Src->Data)[3*Index+1]*Percent;
							Total[2]+=((unsigned short *)Src->Data)[3*Index+2]*Percent;
							break;

						case 32:
							Total[0]+=(float)Src->Data[4*Index+0]*Percent;
							Total[1]+=(float)Src->Data[4*Index+1]*Percent;
							Total[2]+=(float)Src->Data[4*Index+2]*Percent;
							Total[3]+=(float)Src->Data[4*Index+3]*Percent;
							break;

						case 24:
							Total[0]+=Src->Data[3*Index+0]*Percent;
							Total[1]+=Src->Data[3*Index+1]*Percent;
							Total[2]+=Src->Data[3*Index+2]*Percent;
							break;

						case 16:
							Total[0]+=((((unsigned short *)Src->Data)[Index]>>0x0)&0x1F)*Percent;
							Total[1]+=((((unsigned short *)Src->Data)[Index]>>0x5)&0x1F)*Percent;
							Total[2]+=((((unsigned short *)Src->Data)[Index]>>0xA)&0x1F)*Percent;
							break;

						case 8:
							Total[0]+=Src->Data[Index]*Percent;
					}

					ix++;
					fx=(float)ix;
				}

				iy++;
				fy=(float)iy;
			}

			Index=y*Dst->Width+x;
			Sum=1.0f/Sum;

			switch(Dst->Depth)
			{
				case 128:
					((float *)Dst->Data)[4*Index+0]=(float)(Total[0]*Sum);
					((float *)Dst->Data)[4*Index+1]=(float)(Total[1]*Sum);
					((float *)Dst->Data)[4*Index+2]=(float)(Total[2]*Sum);
					((float *)Dst->Data)[4*Index+3]=(float)(Total[3]*Sum);
					break;

				case 96:
					((float *)Dst->Data)[3*Index+0]=(float)(Total[0]*Sum);
					((float *)Dst->Data)[3*Index+1]=(float)(Total[1]*Sum);
					((float *)Dst->Data)[3*Index+2]=(float)(Total[2]*Sum);
					break;

				case 64:
					((unsigned short *)Dst->Data)[4*Index+0]=(unsigned short)(Total[0]*Sum);
					((unsigned short *)Dst->Data)[4*Index+1]=(unsigned short)(Total[1]*Sum);
					((unsigned short *)Dst->Data)[4*Index+2]=(unsigned short)(Total[2]*Sum);
					((unsigned short *)Dst->Data)[4*Index+3]=(unsigned short)(Total[3]*Sum);
					break;

				case 48:
					((unsigned short *)Dst->Data)[3*Index+0]=(unsigned short)(Total[0]*Sum);
					((unsigned short *)Dst->Data)[3*Index+1]=(unsigned short)(Total[1]*Sum);
					((unsigned short *)Dst->Data)[3*Index+2]=(unsigned short)(Total[2]*Sum);
					break;

				case 32:
					((unsigned char *)Dst->Data)[4*Index+0]=(unsigned char)(Total[0]*Sum);
					((unsigned char *)Dst->Data)[4*Index+1]=(unsigned char)(Total[1]*Sum);
					((unsigned char *)Dst->Data)[4*Index+2]=(unsigned char)(Total[2]*Sum);
					((unsigned char *)Dst->Data)[4*Index+3]=(unsigned char)(Total[3]*Sum);
					break;

				case 24:
					((unsigned char *)Dst->Data)[3*Index+0]=(unsigned char)(Total[0]*Sum);
					((unsigned char *)Dst->Data)[3*Index+1]=(unsigned char)(Total[1]*Sum);
					((unsigned char *)Dst->Data)[3*Index+2]=(unsigned char)(Total[2]*Sum);
					break;

				case 16:
					((unsigned short *)Dst->Data)[Index]=((unsigned short)((Total[0]*Sum))&0x1F)<<0x0|((unsigned short)(Total[1]*Sum)&0x1F)<<0x5|((unsigned short)(Total[2]*Sum)&0x1F)<<0xA;
					break;

				case 8:
					((unsigned char *)Dst->Data)[Index]=(unsigned char)(Total[0]*Sum);
					break;
			}
		}
	}
}

void ColorMipmapLevel(Image_t *Image, int miplevel)
{
	int i;
	int color=miplevel%4;

	switch(Image->Depth)
	{
		case 128:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((float *)Image->Data)[4*i+1]=0.0f;
						((float *)Image->Data)[4*i+2]=0.0f;
						break;

					case 2:
						((float *)Image->Data)[4*i+0]=0.0f;
						((float *)Image->Data)[4*i+2]=0.0f;
						break;

					case 3:
						((float *)Image->Data)[4*i+1]=0.0f;
						((float *)Image->Data)[4*i+1]=0.0f;
						break;
				}
			}
			break;

		case 96:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((float *)Image->Data)[3*i+1]=0.0f;
						((float *)Image->Data)[3*i+2]=0.0f;
						break;

					case 2:
						((float *)Image->Data)[3*i+0]=0.0f;
						((float *)Image->Data)[3*i+2]=0.0f;
						break;

					case 3:
						((float *)Image->Data)[3*i+0]=0.0f;
						((float *)Image->Data)[3*i+1]=0.0f;
						break;
				}
			}
			break;

		case 64:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((unsigned short *)Image->Data)[4*i+1]=0;
						((unsigned short *)Image->Data)[4*i+2]=0;
						break;

					case 2:
						((unsigned short *)Image->Data)[4*i+0]=0;
						((unsigned short *)Image->Data)[4*i+2]=0;
						break;

					case 3:
						((unsigned short *)Image->Data)[4*i+1]=0;
						((unsigned short *)Image->Data)[4*i+1]=0;
						break;
				}
			}
			break;

		case 48:
			for(i=0;i<Image->Width*Image->Height;i+=3)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((unsigned short *)Image->Data)[3*i+1]=0;
						((unsigned short *)Image->Data)[3*i+2]=0;
						break;

					case 2:
						((unsigned short *)Image->Data)[3*i+0]=0;
						((unsigned short *)Image->Data)[3*i+2]=0;
						break;

					case 3:
						((unsigned short *)Image->Data)[3*i+1]=0;
						((unsigned short *)Image->Data)[3*i+1]=0;
						break;
				}
			}
			break;

		case 32:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((unsigned long *)Image->Data)[i]&=0xFFFF0000;
						break;

					case 2:
						((unsigned long *)Image->Data)[i]&=0xFF00FF00;
						break;

					case 3:
						((unsigned long *)Image->Data)[i]&=0xFF0000FF;
						break;
				}
			}
			break;

		case 24:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						Image->Data[3*i+1]=0;
						Image->Data[3*i+0]=0;
						break;

					case 2:
						Image->Data[3*i+2]=0;
						Image->Data[3*i+0]=0;
						break;

					case 3:
						Image->Data[3*i+1]=0;
						Image->Data[3*i+2]=0;
						break;
				}
			}
			break;

		case 16:
			for(i=0;i<Image->Width*Image->Height;i++)
			{
				switch(color)
				{
					case 0:
						break;

					case 1:
						((unsigned short *)Image->Data)[i]&=0xFC00;
						break;

					case 2:
						((unsigned short *)Image->Data)[i]&=0x83E0;
						break;

					case 3:
						((unsigned short *)Image->Data)[i]&=0x801F;
						break;
				}
			}
			break;

		default:
			break;
	}
}

void _BuildMipmaps(Image_t *Image, unsigned int Target, int ColorMipmap)
{
	int i=0;
	unsigned int MaxSize;
	Image_t Dst;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxSize);

	Dst.Depth=Image->Depth;
	Dst.Width=min(MaxSize, _NextPower2(Image->Width));
	Dst.Height=min(MaxSize, _NextPower2(Image->Height));

	while(Dst.Width&&Dst.Height)
	{
		Dst.Data=(unsigned char *)malloc(Dst.Width*Dst.Height*(Dst.Depth>>3));

		_Resample(Image, &Dst);

		if(ColorMipmap)
			ColorMipmapLevel(&Dst, i);

		switch(Dst.Depth)
		{
			case 128:
				glTexImage2D(Target, i, GL_RGBA16, Dst.Width, Dst.Height, 0, GL_RGBA, GL_FLOAT, Dst.Data);
				break;

			case 96:
				glTexImage2D(Target, i, GL_RGB16, Dst.Width, Dst.Height, 0, GL_RGB, GL_FLOAT, Dst.Data);
				break;

			case 64:
				glTexImage2D(Target, i, GL_RGBA16, Dst.Width, Dst.Height, 0, GL_RGBA, GL_UNSIGNED_SHORT, Dst.Data);
				break;

			case 48:
				glTexImage2D(Target, i, GL_RGB16, Dst.Width, Dst.Height, 0, GL_RGB, GL_UNSIGNED_SHORT, Dst.Data);
				break;

			case 32:
				glTexImage2D(Target, i, GL_RGBA8, Dst.Width, Dst.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV_EXT, Dst.Data);
				break;

			case 24:
				glTexImage2D(Target, i, GL_RGB8, Dst.Width, Dst.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Dst.Data);
				break;

			case 16:
				glTexImage2D(Target, i, GL_RGB5, Dst.Width, Dst.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT, Dst.Data);
				break;

			case 8:
				glTexImage2D(Target, i, GL_ALPHA8, Dst.Width, Dst.Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Dst.Data);
				break;
		}

		FREE(Dst.Data);

		Dst.Width>>=1;
		Dst.Height>>=1;
		i++;
	}
}

void _FlipImage(Image_t *Image)
{
	int x, y, i;
	int Channels=Image->Depth>>3;
	unsigned char *Buffer=NULL, *Ptr;

	Buffer=(unsigned char *)malloc(Image->Width*Image->Height*Channels);

	if(Buffer==NULL)
		return;

	Ptr=Buffer;

	for(y=0;y<Image->Height;y++)
	{
		int Flipy=(Image->Height-1)-y;

		for(x=0;x<Image->Width;x++)
		{
			int Flipx=(Image->Width-1)-x;

			for(i=0;i<Channels;i++)
				*Ptr++=Image->Data[Channels*(Flipy*Image->Width+Flipx)+i];
		}
	}

	FREE(Image->Data);
	Image->Data=Buffer;
}

void _GetBlock(Image_t *Image, int sx, int sy, int Width, int Height, Image_t *Block)
{
	int x, y, i, Channels=Image->Depth>>3;

	Block->Width=Width;
	Block->Height=Height;
	FREE(Block->Data);
	Block->Data=(unsigned char *)malloc(Block->Width*Block->Height*Channels);

	for(y=0;y<Block->Height;y++)
	{
		for(x=0;x<Block->Width;x++)
		{
			for(i=0;i<Channels;i++)
				Block->Data[Channels*(y*Block->Width+x)+i]=Image->Data[Channels*((sy+y)*Image->Width+(sx+x))+i];
		}
	}
}

unsigned int Image_Upload(char *Filename, unsigned long Flags)
{
	unsigned int TextureID=0;
	unsigned int Target=GL_TEXTURE_2D;
	int ColorMipmap=0;
	Image_t Image;
	char *Extension=strrchr(Filename, '.');

	if(Extension!=NULL)
	{
		if(!stricmp(Extension, ".dds"))
		{
			if(!DDS_Load(Filename, &Image))
				return 0;
		}
		else
		if(!stricmp(Extension, ".tga"))
		{
			if(!TGA_Load(Filename, &Image))
				return 0;
		}
		else
		if(!stricmp(Extension, ".hdr"))
		{
			if(!HDR_Load(Filename, &Image))
				return 0;
		}
		else
			return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if(Flags&IMAGE_CUBEMAP_CROSS)
	{
		if(GL_ARB_texture_cube_map_Flag)
			Target=GL_TEXTURE_CUBE_MAP_ARB;
		else
			return 0;
	}

	if(Flags&IMAGE_RECTANGLE)
	{
		if(GL_EXT_texture_rectangle_Flag)
			Target=GL_TEXTURE_RECTANGLE_EXT;
		else
			return 0;

		if(Flags&IMAGE_AUTOMIPMAP)
			Flags^=IMAGE_AUTOMIPMAP;

		if(Flags&IMAGE_MIPMAP)
			Flags^=IMAGE_MIPMAP;

		if(Flags&IMAGE_REPEAT)
		{
			Flags^=IMAGE_REPEAT;
			Flags|=IMAGE_CLAMP;
		}
	}

	glGenTextures(1, &TextureID);
	glBindTexture(Target, TextureID);

	if(Flags&IMAGE_AUTOMIPMAP)
	{
		if(GL_SGIS_generate_mipmap_Flag)
		{
			glTexParameteri(Target, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);

			if(!(Flags&IMAGE_MIPMAP))
				Flags|=IMAGE_MIPMAP;
		}
	}

	glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(Flags&IMAGE_MIPMAP)
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else
		glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(Flags&IMAGE_NEAREST)
	{
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if(Flags&IMAGE_MIPMAP)
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if(Flags&IMAGE_BILINEAR)
	{
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(Flags&IMAGE_MIPMAP)
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if(Flags&IMAGE_TRILINEAR)
	{
		glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(Flags&IMAGE_MIPMAP)
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if(Flags&IMAGE_CLAMP_U)
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP);

	if(Flags&IMAGE_CLAMP_V)
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if(Flags&IMAGE_CLAMP)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	if(Flags&IMAGE_CLAMP_EDGE_U)
	{
		if(GL_EXT_texture_edge_clamp_Flag)
			glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
		else
			glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP);
	}

	if(Flags&IMAGE_CLAMP_EDGE_V)
	{
		if(GL_EXT_texture_edge_clamp_Flag)
			glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
		else
			glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	if(Flags&IMAGE_CLAMP_EDGE)
	{
		if(GL_EXT_texture_edge_clamp_Flag)
		{
			glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
			glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
		}
		else
		{
			glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
	}

	if(Flags&IMAGE_REPEAT_U)
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if(Flags&IMAGE_REPEAT_V)
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if(Flags&IMAGE_REPEAT)
	{
		glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if(Flags&IMAGE_NORMALMAP)
		_MakeNormalMap(&Image);

	if(Flags&IMAGE_NORMALIZE)
		_Normalize(&Image);

	if(Flags&IMAGE_COLORMIPMAP)
		ColorMipmap=1;

	if(Flags&IMAGE_CUBEMAP_CROSS)
	{
		Image_t Face;
		unsigned long Internal, External, Type;
		int FaceWidth, FaceHeight;

		if(!GL_ARB_texture_cube_map_Flag)
		{
			FREE(Image.Data);
			return 0;
		}

		switch(Image.Depth)
		{
			case 128:
				Internal=GL_RGBA16;
				External=GL_RGBA;
				Type=GL_FLOAT;
				break;

			case 96:
				Internal=GL_RGB16;
				External=GL_RGB;
				Type=GL_FLOAT;
				break;

			case 32:
				Internal=GL_RGBA8;
				External=GL_BGRA_EXT;
				Type=GL_UNSIGNED_INT_8_8_8_8_REV_EXT;
				break;

			case 24:
				Internal=GL_RGB8;
				External=GL_BGR_EXT;
				Type=GL_UNSIGNED_BYTE;
				break;

			case 8:
				Internal=GL_ALPHA8;
				External=GL_ALPHA;
				Type=GL_UNSIGNED_BYTE;
				break;

			default:
				FREE(Image.Data);
				return 0;
		}

		FaceWidth=Image.Width/3;
		FaceHeight=Image.Height/4;

		Face.Depth=Image.Depth;
		Face.Width=FaceWidth;
		Face.Height=FaceHeight;
		Face.Data=(unsigned char *)malloc(Face.Width*Face.Height*(Face.Depth>>3));

		if(Face.Data==NULL)
		{
			FREE(Image.Data);
			return 0;
		}

		_GetBlock(&Image, 0, Face.Height*2, FaceWidth, FaceHeight, &Face);
		_FlipImage(&Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, ColorMipmap);

		_GetBlock(&Image, Face.Width*2, Face.Height*2, FaceWidth, FaceHeight, &Face);
		_FlipImage(&Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, ColorMipmap);

		_GetBlock(&Image, Face.Width, Face.Height*3, FaceWidth, FaceHeight, &Face);
		_FlipImage(&Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, ColorMipmap);

		_GetBlock(&Image, Face.Width, Face.Height, FaceWidth, FaceHeight, &Face);
		_FlipImage(&Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, ColorMipmap);

		_GetBlock(&Image, Face.Width, Face.Height*2, FaceWidth, FaceHeight, &Face);
		_FlipImage(&Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, ColorMipmap);

		_GetBlock(&Image, Face.Width, 0, FaceWidth, FaceHeight, &Face);

		if(Flags&IMAGE_AUTOMIPMAP)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, Internal, Face.Width, Face.Height, 0, External, Type, Face.Data);
		else
			_BuildMipmaps(&Face, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, ColorMipmap);

		FREE(Face.Data);
		FREE(Image.Data);

		return TextureID;
	}

	switch(Image.Depth)
	{
		case IMAGE_DXT5:
			if(GL_ARB_texture_compression_Flag&&GL_EXT_texture_compression_s3tc_Flag)
				glCompressedTexImage2DARB(Target, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*16, Image.Data);
			break;

		case IMAGE_DXT3:
			if(GL_ARB_texture_compression_Flag&&GL_EXT_texture_compression_s3tc_Flag)
				glCompressedTexImage2DARB(Target, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*16, Image.Data);
			break;

		case IMAGE_DXT1:
			if(GL_ARB_texture_compression_Flag&&GL_EXT_texture_compression_s3tc_Flag)
				glCompressedTexImage2DARB(Target, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, Image.Width, Image.Height, 0, ((Image.Width+3)/4)*((Image.Height+3)/4)*8, Image.Data);
			break;

		case 128:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGBA16, Image.Width, Image.Height, 0, GL_RGBA, GL_FLOAT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 96:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGB16, Image.Width, Image.Height, 0, GL_RGB, GL_FLOAT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 64:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGBA16, Image.Width, Image.Height, 0, GL_RGBA, GL_UNSIGNED_SHORT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 48:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGB16, Image.Width, Image.Height, 0, GL_RGB, GL_UNSIGNED_SHORT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 32:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGBA8, Image.Width, Image.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV_EXT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 24:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGB8, Image.Width, Image.Height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 16:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_RGB5, Image.Width, Image.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		case 8:
			if(Flags&IMAGE_AUTOMIPMAP)
				glTexImage2D(Target, 0, GL_ALPHA8, Image.Width, Image.Height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Image.Data);
			else
				_BuildMipmaps(&Image, Target, ColorMipmap);
			break;

		default:
			FREE(Image.Data);
			return 0;
	}

	FREE(Image.Data);

	return TextureID;
}
