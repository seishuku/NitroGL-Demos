#include <stdio.h>
#include <malloc.h>
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void TGA_GetPackets(unsigned char *data, int width, int height, int depth, FILE *stream)
{
	int current_byte=0, run_length, i;
	unsigned char header, buffer8[4];
	unsigned short buffer16=0;
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
					data[current_byte++]=(unsigned char)((buffer16&0x1F)<<3);
					data[current_byte++]=(unsigned char)(((buffer16>>5)&0x1F)<<3);
					data[current_byte++]=(unsigned char)(((buffer16>>10)&0x1F)<<3);
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
					data[current_byte++]=(unsigned char)((buffer16&0x1F)<<3);
					data[current_byte++]=(unsigned char)(((buffer16>>5)&0x1F)<<3);
					data[current_byte++]=(unsigned char)(((buffer16>>10)&0x1F)<<3);
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

int TGA_Load(char *filename, IMAGE_t *Image)
{
	FILE *stream;
	unsigned char IDLength;
	unsigned char ColorMapType, ImageType;
	unsigned short ColorMapStart, ColorMapLength;
	unsigned char ColorMapDepth;
	unsigned short XOffset, YOffset;
	unsigned short Width, Height;
	unsigned char Depth;
	unsigned char ImageDescriptor;
	unsigned char *ColorMap=NULL;

	if((stream=fopen(filename, "rb"))==NULL)
		return 0;

	fread(&IDLength, sizeof(unsigned char), 1, stream);
	fread(&ColorMapType, sizeof(unsigned char), 1, stream);
	fread(&ImageType, sizeof(unsigned char), 1, stream);
	fread(&ColorMapStart, sizeof(unsigned short), 1, stream);
	fread(&ColorMapLength, sizeof(unsigned short), 1, stream);
	fread(&ColorMapDepth, sizeof(unsigned char), 1, stream);
	fread(&XOffset, sizeof(unsigned short), 1, stream);
	fread(&YOffset, sizeof(unsigned short), 1, stream);
	fread(&Width, sizeof(unsigned short), 1, stream);
	fread(&Height, sizeof(unsigned short), 1, stream);
	fread(&Depth, sizeof(unsigned char), 1, stream);
	fread(&ImageDescriptor, sizeof(unsigned char), 1, stream);

	fseek(stream, IDLength, SEEK_CUR);

	switch(ImageType)
	{
		case 1:
			if(ColorMapType==1&&ColorMapDepth==24)
			{
				ColorMap=(unsigned char *)malloc(ColorMapLength*(ColorMapDepth/8));
				fread(ColorMap, sizeof(unsigned char), ColorMapLength*(ColorMapDepth/8), stream);
			}
			else
			{
				fclose(stream);
				return 0;
			}
			break;

		case 2:
			break;

		case 3:
			break;

		case 9:
			if(ColorMapType==1&&ColorMapDepth==24)
			{
				ColorMap=(unsigned char *)malloc(ColorMapLength*(ColorMapDepth/8));
				fread(ColorMap, sizeof(unsigned char), ColorMapLength*(ColorMapDepth/8), stream);
			}
			else
			{
				fclose(stream);
				return 0;
			}
			break;

		case 10:
			break;

		case 11:
			break;

		default:
			fclose(stream);
			return 0;
			break;
	}

	switch(Depth)
	{
		case 32:
			Image->Data=(unsigned char *)malloc(Width*Height*4);

			if(ImageType==2)
			{
				fread(Image->Data, sizeof(unsigned char), Width*Height*4, stream);
				break;
			}

			if(ImageType==10)
			{
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
				break;
			}
			break;

		case 24:
			Image->Data=(unsigned char *)malloc(Width*Height*3);

			if(ImageType==2)
			{
				fread(Image->Data, sizeof(unsigned char), Width*Height*3, stream);
				break;
			}

			if(ImageType==10)
			{
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
				break;
			}
			break;

		case 16:
			Image->Data=(unsigned char *)malloc(Width*Height*3);

			if(ImageType==2)
			{
				unsigned short *buffer=(unsigned short *)malloc(sizeof(unsigned short)*Width*Height);
				int i;

				fread(buffer, sizeof(unsigned short), Width*Height, stream);

				for(i=0;i<Width*Height;i++)
				{
					Image->Data[3*i]=(unsigned char)((buffer[i]&0x1F)<<3);
					Image->Data[3*i+1]=(unsigned char)(((buffer[i]>>5)&0x1F)<<3);
					Image->Data[3*i+2]=(unsigned char)(((buffer[i]>>10)&0x1F)<<3);
				}

				FREE(buffer);
				Depth=24;
				break;
			}

			if(ImageType==10)
			{
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
				Depth=24;
				break;
			}
			break;

		case 8:
			if(ColorMapType==1&&ColorMapDepth==24)
			{
				unsigned char *buffer;
				int i;

				Image->Data=(unsigned char *)malloc(Width*Height*3);
				buffer=(unsigned char *)malloc(Width*Height);

				if(ImageType==9)
					TGA_GetPackets(buffer, Width, Height, Depth, stream);
				else
					fread(buffer, sizeof(unsigned char), Width*Height, stream);

				for(i=0;i<Width*Height;i++)
				{
					Image->Data[3*i]=ColorMap[3*buffer[i]];
					Image->Data[3*i+1]=ColorMap[3*buffer[i]+1];
					Image->Data[3*i+2]=ColorMap[3*buffer[i]+2];
				}

				FREE(buffer);
				FREE(ColorMap);

				Depth=24;
				break;
			}

			if(ImageType==3)
			{
				Image->Data=(unsigned char *)malloc(Width*Height);
				fread(Image->Data, sizeof(unsigned char), Width*Height, stream);
				break;
			}

			if(ImageType==11)
			{
				Image->Data=(unsigned char *)malloc(Width*Height);
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
				break;
			}
			break;

		default:
			break;
	}

	fclose(stream);

	Image->Width=Width;
	Image->Height=Height;
	Image->Depth=Depth;

	return 1;
}
