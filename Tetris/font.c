#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "opengl.h"
#include "image.h"
#include "font.h"

void Font_Print(int x, int y, char *string, ...)
{
	static unsigned int TextureID=0;
	char *ptr, text[4096];
	va_list	ap;
	int sx=x;
	float cx, cy;

	if(!TextureID)
		TextureID=Image_Upload("font.tga", IMAGE_NONE);

	if(string==NULL)
		return;

	va_start(ap, string);
	vsprintf(text, string, ap);
	va_end(ap);

	for(ptr=text;*ptr!='\0';ptr++)
	{
		if(*ptr=='\n'||*ptr=='\r')
			y+=28;
	}

	glBindTexture(GL_TEXTURE_2D, TextureID);
	glEnable(GL_TEXTURE_2D);

	for(ptr=text;*ptr!='\0';ptr++)
	{
		if(*ptr=='\n'||*ptr=='\r')
		{
			x=sx;
			y-=28;
			continue;
		}

		cx=     (float)(*ptr%16)*0.0625f;
		cy=1.0f-(float)(*ptr/16)*0.0625f;

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(cx, cy);
			glVertex2i(x, y+32);

			glTexCoord2f(cx, cy-0.0625f);
			glVertex2i(x, y);

			glTexCoord2f(cx+0.0625f, cy);
			glVertex2i(x+32, y+32);

			glTexCoord2f(cx+0.0625f, cy-0.0625f);
			glVertex2i(x+32, y);
		glEnd();

		x+=16;
	}

	glDisable(GL_TEXTURE_2D);
}

