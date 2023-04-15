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
	int x1=x, y1=y;
	unsigned int i;
	float cx, cy;

	if(string==NULL)
		return;

	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	for(i=0;i<strlen(text);i++)
	{
		cx=(float)fmod(text[i]/16.0f, 1);
		cy=(text[i]/16)/16.0f;

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(cx, 1-cy);
			glVertex2i(x1, 16+y1);
			glTexCoord2f(cx, 1-cy-0.0625f);
			glVertex2i(x1, y1);
			glTexCoord2f(cx+0.0625f, 1-cy);
			glVertex2i(16+x1, 16+y1);
			glTexCoord2f(cx+0.0625f, 1-cy-0.0625f);
			glVertex2i(16+x1, y1);
		glEnd();

		x1+=16;

		if(text[i]=='\n'||text[i]=='\r')
		{
			x1=x;
			y1-=16;
		}
	}

	glDisable(GL_TEXTURE_2D);
}
