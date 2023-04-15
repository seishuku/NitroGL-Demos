#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include "font.h"
#include "resource.h"

unsigned int Font_Build(void)
{
	int loop;
	float cx;
	float cy;
	unsigned int base;

	base=glGenLists(256);

	for(loop=0;loop<256;loop++)
	{
		cx=loop/16.0f;
		cy=loop/16/16.0f;

		glNewList(base+loop, GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx, 1-cy-0.0625f);
					glVertex2i(0, 0);
				glTexCoord2f(cx+0.068f, 1-cy-0.0625f);
					glVertex2i(16, 0);
				glTexCoord2f(cx+0.068f, 1-cy);
					glVertex2i(16, 16);
				glTexCoord2f(cx, 1-cy);
					glVertex2i(0, 16);
			glEnd();
			glTranslated(12,0,0);
		glEndList();
	}

	return base;

}

void Font_Print(int x, int y, unsigned int base, unsigned int texture, char *string, ...)
{
	char text[1024];
	va_list	ap;

	if(string==NULL)
		return;

	va_start(ap, string);
		vsprintf(text, string, ap);
	va_end(ap);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslated(x, y, 0);
	glListBase(base);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
