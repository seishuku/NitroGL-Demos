#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include "glati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="Cubic Texture Environment Mapping";

RECT WindowRect;

int Width=512, Height=512;
BOOL Done=FALSE, Key[255];

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames=0;
int nLastFrames=0;
char OutBuffer[255];

float z;

int mx, my;

unsigned int FontBase, FontTexture, Envmap, Glare;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawTorus(float OuterRadius, float InnerRadius, int Slice, int Stack);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	wc.style=CS_VREDRAW|CS_HREDRAW|CS_OWNDC;
	wc.lpfnWndProc=WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor=LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground=GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=szAppName;

	RegisterClass(&wc);

	WindowRect.left=0;
	WindowRect.right=Width;
	WindowRect.top=0;
	WindowRect.bottom=Height;

	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd=CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	ShowCursor(FALSE);

	if(!Create())
		Done=TRUE;

	if(Done==FALSE)
		Resize(Width, Height);

	if(Done==FALSE)
		if(!Init())
			Done=TRUE;

	while(!Done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				Done=TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(Key[VK_PRIOR])
				z+=0.01f;

			if(Key[VK_NEXT])
				z-=0.01f;

			Render();
			SwapBuffers(hDC);

			nFrames++;
			if(nFrames>20)
			{
				QueryPerformanceCounter(&currentTime);
				fps=(float)nFrames/((float)(currentTime.QuadPart - startTime.QuadPart)/(float)timerFrequency.QuadPart);
				sprintf(OutBuffer, "FPS: %0.1f", fps);
				nFrames=0;
				QueryPerformanceCounter(&startTime);
			}
		}
	}

	Destroy();
	DestroyWindow(hWnd);
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			QueryPerformanceFrequency(&timerFrequency);
			QueryPerformanceCounter(&startTime);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_DESTROY:
			break;

		case WM_SIZE:
			Width=LOWORD(lParam);
			Height=HIWORD(lParam);
			Resize(Width, Height);
			break;

		case WM_MOUSEMOVE:
			mx=LOWORD(lParam);
			my=HIWORD(lParam);
			break;

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				default:
					break;
			}
			break;

		case WM_KEYUP:
			Key[wParam]=FALSE;
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Envmap);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, z-2.0f);
		glRotatef((float)mx-(Width/2), 0.0f, 1.0f, 0.0f);
		glRotatef((float)my-(Height/2), 1.0f, 0.0f, 0.0f);
		DrawTorus(1.0f, 0.5f, 64, 64);
	glPopMatrix();
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, Glare);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 512, 512);

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBindTexture(GL_TEXTURE_2D, Glare);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(0.0f, (float)Height, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f((float)Width, (float)Height, -1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f((float)Width, 0.0f, -1.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		Font_Print(mx, -my+Height, FontBase, FontTexture, "#");
		Font_Print(0, 0, FontBase, FontTexture, OutBuffer);
	EndOverlay();
}

BOOL Init(void)
{
//	if(!IsExtensionSupported("GL_ARB_texture_cube_map"))
	{
		MessageBox(hWnd, "GL_ARB_texture_cube_map not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	Envmap=TGA_UploadCube("right.tga", "left.tga", "top.tga", "bottom.tga", "front.tga", "back.tga");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glGenTextures(1, &Glare);
	glBindTexture(GL_TEXTURE_2D, Glare);
 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL_SGIS, 0.0f);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	return TRUE;
}

BOOL Create(void)
{
	PIXELFORMATDESCRIPTOR pfd;
	unsigned int PixelFormat;

	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=32;
	pfd.cDepthBits=24;

	hDC=GetDC(hWnd);

	if(!(PixelFormat=ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(hWnd, "ChoosePixelFormat Failed.", "ERROR", MB_OK);
		return FALSE;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		MessageBox(hWnd, "SetPixelFormat Failed.", "ERROR", MB_OK);
		return FALSE;
	}

	if(!(hRC=wglCreateContext(hDC)))
	{
		MessageBox(hWnd, "wglCreateContext Failed.", "ERROR", MB_OK);
		return FALSE;
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
		MessageBox(hWnd, "wglMakeCurrent Failed.", "ERROR", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void Destroy(void)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

void Resize(int Width, int Height)
{
	if(Height==0)
		Height=1;

	glViewport(0,0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)Width/Height, 0.001f, 400.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

BOOL IsExtensionSupported(const char *extension)
{
	const unsigned char *extensions=NULL, *start, *where, *terminator;

	where=strchr(extension, ' ');

	if(where||*extension=='\0')
		return FALSE;

	extensions=glGetString(GL_EXTENSIONS);
	start=extensions;

	for(;;)
	{
		where=strstr(start, extension);

		if(!where)
			break;

		terminator=where+strlen(extension);

		if(where==start||*(where-1)==' ')
			if(*terminator==' '||*terminator=='\0')
				return TRUE;

		start=terminator;
	}

	return FALSE;
}

void DrawTorus(float OuterRadius, float InnerRadius, int Slice, int Stack)
{
	int i, j, Texrep=8;
	float SliceStep=2.0f*3.1415f/Slice, StackStep=2.0f*3.1415f/Stack;

	for(i=0;i<=Slice;i++)
	{
		float a=i*SliceStep;
		float b=a+StackStep;
		float x0=(float)cos(a);
		float x1=(float)cos(b);
		float y0=(float)sin(a);
		float y1=(float)sin(b);

		glBegin(GL_TRIANGLE_STRIP);

		for(j=0;j<=Stack;j++)
		{
			float c=(float)cos(j*StackStep);
			float r=InnerRadius*c+OuterRadius;
			float z=InnerRadius*(float)sin(j*StackStep);

			glNormal3f(x0*c, y0*c, z/InnerRadius);
			glVertex3f(x0*r, y0*r, z);

			glNormal3f(x1*c, y1*c, z/InnerRadius);
			glVertex3f(x1*r, y1*r, z);
		}

		glEnd();
	}
}
