#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include "font.h"

void Font_Print(int x, int y, unsigned int texture, char *string, ...)
{
	char text[4096];
	va_list	ap;
	float x1=(float)x, y1=(float)y;
	unsigned int i;
	float cx, cy;

	if(string==NULL)
		return;

	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	for(i=0;i<strlen(text);i++)
	{
		cx=(float)fmod(text[i]/16.0f, 1);
		cy=(text[i]/16)/16.0f;

		glTexCoord2f(cx+0.0625f, (1-cy)-0.0625f);
		glVertex2f(x1+16, y1);

		glTexCoord2f(cx+0.0625f, 1-cy);
		glVertex2f(x1+16, y1+16);

		glTexCoord2f(cx, 1-cy);
		glVertex2f(x1, y1+16);

		glTexCoord2f(cx, (1-cy)-0.0625f);
		glVertex2f(x1, y1);

		x1+=16;

		if(text[i]=='\t')
			x1+=64;

		if(text[i]=='\n'||text[i]=='\r')
		{
			x1=(float)x;
			y1-=16;
		}
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}
