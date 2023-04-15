#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void TGA_GetPackets(unsigned char *data, int width, int height, int depth, FILE *stream)
{
	int current_byte=0, run_length, i;
	unsigned char header, numbits;
	unsigned char buffer[4];
	unsigned short buffer16;

	numbits=(depth==16)?3:depth>>3;

	while(current_byte<width*height*numbits)
	{
		fread(&header, sizeof(unsigned char), 1, stream);

		run_length=(header&0x7F)+1;

		if(header&0x80)
		{
			if(depth!=16)
				fread(buffer, sizeof(unsigned char), numbits, stream);
			else
				fread(&buffer16, sizeof(unsigned short), 1, stream);

			for(i=0;i<run_length;i++)
			{
				if(depth!=16)
					memcpy(&data[current_byte], buffer, numbits);
				else
				{
					data[current_byte]=(unsigned char)((buffer16&0x1F)<<3);
					data[current_byte+1]=(unsigned char)(((buffer16>>5)&0x1F)<<3);
					data[current_byte+2]=(unsigned char)(((buffer16>>10)&0x1F)<<3);
				}

				current_byte+=numbits;
			}
		}
		else
		{
			for(i=0;i<run_length;i++)
			{
				if(depth!=16)
				{
					fread(buffer, sizeof(unsigned char), numbits, stream);
					memcpy(&data[current_byte], buffer, numbits);
				}
				else
				{
					fread(&buffer16, sizeof(unsigned short), 1, stream);
					data[current_byte]=(unsigned char)((buffer16&0x1F)<<3);
					data[current_byte+1]=(unsigned char)(((buffer16>>5)&0x1F)<<3);
					data[current_byte+2]=(unsigned char)(((buffer16>>10)&0x1F)<<3);
				}

				current_byte+=numbits;
			}
		}
	}
}

int TGA_Load(char *filename, IMAGE_t *Image)
{
	FILE *stream;
	int i;
	unsigned char IDLength;
	unsigned char ColorMapType, ImageType;
	unsigned short ColorMapStart, ColorMapLength;
	unsigned char ColorMapDepth;
	unsigned short XOffset, YOffset;
	unsigned short Width, Height;
	unsigned char Depth;
	unsigned char ImageDescriptor;

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

	if(!(ImageType==1||ImageType==2||ImageType==3||ImageType==9||ImageType==10||ImageType==11))
	{
		fclose(stream);
		return 0;
	}

	switch(Depth)
	{
		case 32:
			Image->Data=(unsigned char *)malloc(Width*Height*4);

			if(ImageType==2&&ImageType!=10)
				fread(Image->Data, sizeof(unsigned char), Width*Height*4, stream);
			else
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
			break;

		case 24:
			Image->Data=(unsigned char *)malloc(Width*Height*3);

			if(ImageType==2&&ImageType!=10)
				fread(Image->Data, sizeof(unsigned char), Width*Height*3, stream);
			else
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
			break;

		case 16:
			Image->Data=(unsigned char *)malloc(Width*Height*3);

			if(ImageType==2&&ImageType!=10)
			{
				unsigned short *buffer=(unsigned short *)malloc(sizeof(unsigned short)*Width*Height);

				fread(buffer, sizeof(unsigned short), Width*Height, stream);

				for(i=0;i<Width*Height;i++)
				{
					Image->Data[3*i]=(unsigned char)((buffer[i]&0x1F)<<3);
					Image->Data[3*i+1]=(unsigned char)(((buffer[i]>>5)&0x1F)<<3);
					Image->Data[3*i+2]=(unsigned char)(((buffer[i]>>10)&0x1F)<<3);
				}

				FREE(buffer);
			}
			else
				TGA_GetPackets(Image->Data, Width, Height, Depth, stream);

			Depth=24;
			break;

		case 8:
			if(ColorMapType==1&&ColorMapDepth==24)
			{
				unsigned char *ColorMap, *buffer;

				ColorMapDepth>>=3;

				ColorMap=(unsigned char *)malloc(ColorMapLength*ColorMapDepth);
				fread(ColorMap, sizeof(unsigned char), ColorMapLength*ColorMapDepth, stream);

				buffer=(unsigned char *)malloc(Width*Height);
				Image->Data=(unsigned char *)malloc(Width*Height*3);

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
			}
			else
			{
				Image->Data=(unsigned char *)malloc(Width*Height);

				if(ImageType==3&&ImageType!=10)
					fread(Image->Data, sizeof(unsigned char), Width*Height, stream);
				else
					TGA_GetPackets(Image->Data, Width, Height, Depth, stream);
			}
			break;

		default:
			fclose(stream);
			return 0;
	}

	fclose(stream);

	Image->Width=Width;
	Image->Height=Height;
	Image->Depth=Depth;

	return 1;
}
