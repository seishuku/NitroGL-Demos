#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void TGA_GetRLE(unsigned char *data, int width, int height, int depth, FILE *stream)
{
	int current_byte=0, size, run_length, i;
	unsigned char header, numbytes, buffer[4];

	numbytes=depth>>3;
	size=width*height*numbytes;

	while(current_byte<size)
	{
		fread(&header, sizeof(unsigned char), 1, stream);

		run_length=(header&0x7F)+1;

		if(header&0x80)
		{
			fread(buffer, sizeof(unsigned char), numbytes, stream);

			for(i=0;i<run_length;i++)
			{
				memcpy(&data[current_byte], buffer, numbytes);
				current_byte+=numbytes;
			}
		}
		else
		{
			for(i=0;i<run_length;i++)
			{
				fread(&data[current_byte], sizeof(unsigned char), numbytes, stream);
				current_byte+=numbytes;
			}
		}
	}
}

int TGA_Load(char *Filename, Image_t *Image)
{
	FILE *stream=NULL;
	unsigned char IDLength;
	unsigned char ColorMapType, ImageType;
	unsigned short ColorMapStart, ColorMapLength;
	unsigned char ColorMapDepth;
	unsigned short XOffset, YOffset;
	unsigned short Width, Height;
	unsigned char Depth;
	unsigned char ImageDescriptor;
	int i;

	if((stream=fopen(Filename, "rb"))==NULL)
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
		case 11:
		case 10:
		case 9:
		case 3:
		case 2:
		case 1:
			break;

		default:
			fclose(stream);
			return 0;
	}

	switch(Depth)
	{
		case 32:
			Image->Data=(unsigned char *)malloc(Width*Height*4);

			if(Image->Data==NULL)
				return 0;

			if(ImageType==10)
				TGA_GetRLE(Image->Data, Width, Height, Depth, stream);
			else
				fread(Image->Data, sizeof(unsigned char), Width*Height*4, stream);
			break;

		case 24:
			Image->Data=(unsigned char *)malloc(Width*Height*3);

			if(Image->Data==NULL)
				return 0;

			if(ImageType==10)
				TGA_GetRLE(Image->Data, Width, Height, Depth, stream);
			else
				fread(Image->Data, sizeof(unsigned char), Width*Height*3, stream);
			break;

		case 16:
		case 15:
			Image->Data=(unsigned char *)malloc(sizeof(unsigned short)*Width*Height);

			if(Image->Data==NULL)
				return 0;

			if(ImageType==10)
				TGA_GetRLE(Image->Data, Width, Height, Depth, stream);
			else
				fread(Image->Data, sizeof(unsigned short), Width*Height, stream);

			Depth=16;
			break;

		case 8:
			if(ColorMapType)
			{
				unsigned char *ColorMap=NULL, *Buffer=NULL;

				// Ugh
				switch(ColorMapDepth)
				{
					case 32:
						ColorMap=(unsigned char *)malloc(ColorMapLength*4);

						if(ColorMap==NULL)
							return 0;

						fread(ColorMap, sizeof(unsigned char), ColorMapLength*4, stream);

						Buffer=(unsigned char *)malloc(Width*Height);
						
						if(Buffer==NULL)
						{
							FREE(Buffer);
							return 0;
						}
						
						if(ImageType==9)
							TGA_GetRLE(Buffer, Width, Height, Depth, stream);
						else
							fread(Buffer, sizeof(unsigned char), Width*Height, stream);

						Image->Data=(unsigned char *)malloc(Width*Height*4);

						if(Image->Data==NULL)
						{
							FREE(Buffer);
							FREE(ColorMap);

							return 0;
						}

						for(i=0;i<Width*Height;i++)
						{
							Image->Data[4*i+0]=ColorMap[4*Buffer[i]+0];
							Image->Data[4*i+1]=ColorMap[4*Buffer[i]+1];
							Image->Data[4*i+2]=ColorMap[4*Buffer[i]+2];
							Image->Data[4*i+4]=ColorMap[4*Buffer[i]+3];
						}

						Depth=32;
						break;

					case 24:
						ColorMap=(unsigned char *)malloc(ColorMapLength*3);

						if(ColorMap==NULL)
							return 0;

						fread(ColorMap, sizeof(unsigned char), ColorMapLength*3, stream);

						Buffer=(unsigned char *)malloc(Width*Height);

						if(Buffer==NULL)
						{
							FREE(ColorMap);
							return 0;
						}
						
						if(ImageType==9)
							TGA_GetRLE(Buffer, Width, Height, Depth, stream);
						else
							fread(Buffer, sizeof(unsigned char), Width*Height, stream);

						Image->Data=(unsigned char *)malloc(Width*Height*3);

						if(Image->Data==NULL)
						{
							FREE(Buffer);
							FREE(ColorMap);
							return 0;
						}

						for(i=0;i<Width*Height;i++)
						{
							Image->Data[3*i+0]=ColorMap[3*Buffer[i]+0];
							Image->Data[3*i+1]=ColorMap[3*Buffer[i]+1];
							Image->Data[3*i+2]=ColorMap[3*Buffer[i]+2];
						}

						Depth=24;
						break;

					case 16:
					case 15:
						ColorMap=(unsigned char *)malloc(sizeof(unsigned short)*ColorMapLength);

						if(ColorMap==NULL)
						{
							FREE(Buffer);
							return 0;
						}

						fread(ColorMap, sizeof(unsigned short), ColorMapLength, stream);

						Buffer=(unsigned char *)malloc(Width*Height);

						if(Buffer==NULL)
						{
							FREE(Buffer);
							return 0;
						}
						
						if(ImageType==9)
							TGA_GetRLE(Buffer, Width, Height, Depth, stream);
						else
							fread(Buffer, sizeof(unsigned char), Width*Height, stream);

						Image->Data=(unsigned char *)malloc(sizeof(unsigned short)*Width*Height);

						if(Image->Data==NULL)
						{
							FREE(Buffer);
							FREE(ColorMap);
							return 0;
						}

						for(i=0;i<Width*Height;i++)
							((unsigned short *)Image->Data)[i]=((unsigned short *)ColorMap)[Buffer[i]];

						Depth=16;
						break;
				}

				FREE(Buffer);
				FREE(ColorMap);
			}
			else
			{
				Image->Data=(unsigned char *)malloc(Width*Height);

				if(Image->Data==NULL)
					return 0;

				if(ImageType==11)
					TGA_GetRLE(Image->Data, Width, Height, Depth, stream);
				else
					fread(Image->Data, sizeof(unsigned char), Width*Height, stream);
			}
			break;

		default:
			fclose(stream);
			return 0;
	}

	fclose(stream);

	if(ImageDescriptor&0x20)
	{
		int Scanline=Width*(Depth>>3), Size=Scanline*Height;
		unsigned char *Buffer=(unsigned char *)malloc(Size);

		if(Buffer==NULL)
		{
			FREE(Image->Data);
			return 0;
		}

		for(i=0;i<Height;i++)
			memcpy(Buffer+(Size-(i+1)*Scanline), Image->Data+i*Scanline, Scanline);

		memcpy(Image->Data, Buffer, Size);

		FREE(Buffer);
	}

	Image->Width=Width;
	Image->Height=Height;
	Image->Depth=Depth;

	return 1;
}
