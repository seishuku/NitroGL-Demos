#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

void rgbe2float(unsigned char rgbe[4], float *rgb)
{
	float f;
	int e=rgbe[3]-(128+8);

	if(e>0)
		f=(float)(1<<e);
	else
		f=1.0f/(1<<-e);

	rgb[0]=(float)rgbe[0]*f;
	rgb[1]=(float)rgbe[1]*f;
	rgb[2]=(float)rgbe[2]*f;
}

int HDR_ReadHeader(FILE *stream, int *width, int *height, float *exposure, float *gamma)
{
	char buf[256];
	int done=0;

	if(fgets(buf, 256, stream)==0)
		return 0;

	if((buf[0]!='#')||(buf[1]!='?'))
		return 0;

	while(!done)
	{
		if(fgets(buf, 256, stream)==0)
			return 0;

		switch(buf[0])
		{
			case 'F':
				if(strcmp(buf, "FORMAT=32-bit_rle_rgbe\n")!=0)
					return 0;
				break;

			case '-':
				if(sscanf(buf, "-Y %d +X %d\n", height, width)<2)
					return 0;
				done=1;
				break;

			case '#':
			default:
				break;
		}
	}

	return 1;
}

int HDR_ReadPixels(FILE *stream, float *data, int numpixels)
{
	unsigned char rgbe[4];

	while(numpixels-->0)
	{
		if(fread(rgbe, sizeof(unsigned char), 4, stream)<4)
			return 0;

		rgbe2float(rgbe, data);
		data+=3;
	}

	return 1;
}

int HDR_ReadRLEPixels(FILE *stream, float *data, int width, int height)
{
	unsigned char rgbe[4], buf[2], *buffer, *ptr, *ptr_end;
	int y, i, count;

	buffer=NULL;

	for(y=0;y<height;y++)
	{
		if(fread(rgbe, sizeof(unsigned char), 4, stream)<4)
			return 0;

		if((rgbe[0]!=2)||(rgbe[1]!=2)||(rgbe[2]&0x80))
		{
			rgbe2float(rgbe, data);
			data+=3;

			return HDR_ReadPixels(stream, data, width*y-1);
		}

		if((((int)rgbe[2])<<8|rgbe[3])!=width)
			return 0;

		if(buffer==NULL)
			buffer=(unsigned char *)malloc(sizeof(unsigned char)*width*4);

		if(buffer==NULL) 
			return 0;

		ptr=&buffer[0];

		for(i=0;i<4;i++)
		{
			ptr_end=&buffer[(i+1)*width];

			while(ptr<ptr_end)
			{
				if(fread(buf, sizeof(unsigned char), 2, stream)<2)
				{
					FREE(buffer);
					return 0;
				}

				if(buf[0]>128)
				{
					count=buf[0]-128;

					if((count==0)||(count>ptr_end-ptr))
					{
						FREE(buffer);
						return 0;
					}

					while(count-->0)
						*ptr++=buf[1];
				}
				else
				{
					count=buf[0];

					if((count==0)||(count>ptr_end-ptr))
					{
						FREE(buffer);
						return 0;
					}

					*ptr++=buf[1];

					if(--count>0)
					{
						if(fread(ptr, sizeof(unsigned char)*count, 1, stream)<1)
						{
							FREE(buffer);
							return 0;
						}

						ptr+=count;
					}
				}
			}
		}

		for(i=0;i<width;i++)
		{
			rgbe[0]=buffer[i];
			rgbe[1]=buffer[i+width];
			rgbe[2]=buffer[i+2*width];
			rgbe[3]=buffer[i+3*width];

			rgbe2float(rgbe, data);
			data+=3;
		}
	}

	FREE(buffer);

	return 1;
}

int HDR_Load(char *Filename, Image_t *Image)
{
	FILE *stream;
	unsigned char *Scanline;
	int BytesPerLine, i;

	if((stream=fopen(Filename, "rb"))==NULL)
		return 0;

	if(!HDR_ReadHeader(stream, &Image->Width, &Image->Height, NULL, NULL))
	{
		fclose(stream);
		return 0;
	}

	Image->Depth=96;
	Image->Data=(unsigned char *)malloc(sizeof(float)*Image->Width*Image->Height*3);

	if(Image->Data==NULL)
	{
		fclose(stream);
		return 0;
	}

	if(!HDR_ReadRLEPixels(stream, (float *)Image->Data, Image->Width, Image->Height))
	{
		FREE(Image->Data);
		fclose(stream);
		return 0;
	}

	fclose(stream);

	BytesPerLine=sizeof(float)*Image->Width*3;
	Scanline=(unsigned char *)malloc(BytesPerLine);

	for(i=0;i<(Image->Height>>1);i++)
	{
		unsigned char *topPtr=Image->Data+i*BytesPerLine;
		unsigned char *botPtr=Image->Data+(Image->Height-i-1)*BytesPerLine;

		memcpy(Scanline, topPtr, BytesPerLine);
		memcpy(topPtr, botPtr, BytesPerLine);
		memcpy(botPtr, Scanline, BytesPerLine);
	}

	FREE(Scanline);

	return 1;
}
