#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "opengl.h"
#include "math.h"
#include "font.h"

void Font_Print(int x, int y, char *String, ...)
{
	char *Text=NULL, *Ptr=NULL;
	HDC hDC=NULL;
	HFONT hFont=NULL;
	HBITMAP hBitmap=NULL;
	BITMAPINFO BitmapInfo;
	unsigned char *Bitmap=NULL;
	RECT Rect;
	SIZE Size;
	int i;
	va_list ap;

	// Allocate memory for the formatted string (original string length *2 should be enough)
	Text=(char *)malloc(strlen(String)*2);

	if(Text==NULL)
		return;

	// Format the string
	va_start(ap, String);
	vsprintf(Text, String, ap);
	va_end(ap);

	// Create a memory DC
	hDC=CreateCompatibleDC(NULL);

	// Create and select the font
	hFont=CreateFont(32, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, NULL);

	SelectObject(hDC, hFont);

	// Accumulate the total width/height of the string
	memset(&Size, 0, sizeof(SIZE));

	for(Ptr=Text, i=0;;Ptr++)
	{
		if(*Ptr=='\n'||*Ptr=='\0')
		{
			SIZE StringSize;

			if(i!=0)
				GetTextExtentPoint32(hDC, Ptr-i, i, &StringSize);

			if(StringSize.cx>=Size.cx)
				Size.cx=StringSize.cx;

			Size.cy+=StringSize.cy;

			if(*Ptr=='\0')
				break;

			i=0;
		}
		else
			i++;
	}

	// Set the info needed to create the bitmap pointer to the DC
	memset(&BitmapInfo, 0, sizeof(BITMAPINFO));
	BitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biWidth=Size.cx;
	BitmapInfo.bmiHeader.biHeight=Size.cy;
	BitmapInfo.bmiHeader.biBitCount=8;
	BitmapInfo.bmiHeader.biPlanes=1;

	// Create and select it
	hBitmap=CreateDIBSection(hDC, &BitmapInfo, DIB_PAL_COLORS, (void *)&Bitmap, NULL, 0);
	SelectObject(hDC, hBitmap);

	// Set the colors for the font (not really colors, actually 8 bit index for an alpha image)
	SetBkColor(hDC, 0x10ff0000);
	SetTextColor(hDC, 0x10ff00ff);

	// Set the draw area (rectangle), and draw the string
	SetRect(&Rect, 0, 0, Size.cx, Size.cy);
	DrawText(hDC, Text, -1, &Rect, DT_NOCLIP);

	GdiFlush();

	// Stupid windows bitmaps...
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// No texture object, just texture OpenGL 1.0 style!
	// If rectangle textures aren't supported, use the subtexture hack
	if(GL_EXT_texture_rectangle_Flag)
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_EXT, 0);
		glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0, GL_INTENSITY8, Size.cx, Size.cy, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, Bitmap);
		glEnable(GL_TEXTURE_RECTANGLE_EXT);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2i(0, Size.cy);
				glVertex2i(x, y+Size.cy);
			glTexCoord2i(0, 0);
				glVertex2i(x, y);
			glTexCoord2i(Size.cx, Size.cy);
				glVertex2i(x+Size.cx, y+Size.cy);
			glTexCoord2i(Size.cx, 0);
				glVertex2i(x+Size.cx, y);
		glEnd();
		glDisable(GL_TEXTURE_RECTANGLE_EXT);
	}
	else
	{
		int Pow2SizeX=_NextPower2(Size.cx);
		int Pow2SizeY=_NextPower2(Size.cy);
		float s=(float)Size.cx/Pow2SizeX, t=(float)Size.cy/Pow2SizeY;

		glBindTexture(GL_TEXTURE_2D, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY8, Pow2SizeX, Pow2SizeY, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Size.cx, Size.cy, GL_LUMINANCE, GL_UNSIGNED_BYTE, Bitmap);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, t);
				glVertex2i(x, y+Size.cy);
			glTexCoord2f(0.0f, 0.0f);
				glVertex2i(x, y);
			glTexCoord2f(s, t);
				glVertex2i(x+Size.cx, y+Size.cy);
			glTexCoord2f(s, 0.0f);
				glVertex2i(x+Size.cx, y);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	// Delete the objects (since we don't need them anymore)
	DeleteObject(hFont);
	DeleteObject(hBitmap);
	DeleteDC(hDC);

	// And free the formatted string
	free(Text);
}
