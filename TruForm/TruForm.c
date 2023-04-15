#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include "glati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"
#include "md2.h"

#ifndef GL_ATIX_pn_triangles
#define GL_ATIX_pn_triangles 1
#define GL_PN_TRIANGLES_ATIX 0x6090
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATIX 0x6091
#define GL_PN_TRIANGLES_POINT_MODE_ATIX 0x6092
#define GL_PN_TRIANGLES_NORMAL_MODE_ATIX 0x6093
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATIX 0x6094
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATIX 0x6095
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATIX 0x6096
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATIX 0x6097
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATIX 0x6098
typedef void (APIENTRY *PFNGLPNTRIANGLESIATIXPROC)(GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLPNTRIANGLESFATIXPROC)(GLenum pname, GLfloat param);
#endif

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="TruForm";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE, Key[255], TruForm, Wireframe, Lighting, ATIX=FALSE;

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames=0;
int nLastFrames=0;
char OutBuffer[255];

LARGE_INTEGER time_freq;
LARGE_INTEGER time_counter;
LARGE_INTEGER time;

unsigned int FontBase, FontTexture, MaleTex, FemaleTex, WeaponTex;
float i, j;
int Frame, Tess, MaxTess;

MD2_Model_t Male, Female, MaleWeapon, FemaleWeapon;

PFNGLPNTRIANGLESIATIXPROC glPNTrianglesiATIX=NULL;
PFNGLPNTRIANGLESIATIPROC glPNTrianglesiATI=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawSphere(float Radius, int Slice, int Stack);
void DrawTorus(float OuterRadius, float InnerRadius, int Slice, int Stack);
void DrawCylinder(float Radius, float Height, int Slice, int Stack);

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
			QueryPerformanceFrequency(&time_freq);
			QueryPerformanceCounter(&time_counter);
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

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_SPACE:
					TruForm^=TRUE;
					break;

				case 'W':
					Wireframe^=TRUE;
					break;

				case 'L':
					Lighting^=TRUE;
					break;

				case VK_UP:
					if(TruForm)
					{
						Tess++;

						if(Tess>=MaxTess)
							Tess=MaxTess;
					}
					break;

				case VK_DOWN:
					if(TruForm)
					{
						Tess--;

						if(Tess<=0)
							Tess=0;
					}
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
	time=time_counter;

	QueryPerformanceFrequency(&time_freq);
	QueryPerformanceCounter(&time_counter);

	i+=(float)(time_counter.QuadPart-time.QuadPart)/time_freq.QuadPart*8;
	j+=(float)(time_counter.QuadPart-time.QuadPart)/time_freq.QuadPart*32;

	if(i>1.0f)
	{
		i=0;
		Frame++;
	}

	if(Frame==39)
		Frame=0;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(ATIX)
	{
		if(TruForm)
		{
			glPNTrianglesiATIX(GL_PN_TRIANGLES_TESSELATION_LEVEL_ATIX, Tess);
			glEnable(GL_PN_TRIANGLES_ATIX);
		}
		else
			glDisable(GL_PN_TRIANGLES_ATIX);
	}
	else
	{
		if(TruForm)
		{
			glPNTrianglesiATI(GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI, Tess);
			glEnable(GL_PN_TRIANGLES_ATI);
		}
		else
			glDisable(GL_PN_TRIANGLES_ATI);
	}

	if(Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if(Lighting)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
		glDisable(GL_LIGHTING);


	glCullFace(GL_FRONT);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(-20.0f, 0.0f, -40.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(j, 0.0f, 0.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, MaleTex);
		MD2_DrawFramei(&Male, Frame, Frame+1, i);
		glBindTexture(GL_TEXTURE_2D, WeaponTex);
		MD2_DrawFramei(&MaleWeapon, Frame, Frame+1, i);
		if(ATIX)
		{
			if(TruForm)
				glEnable(GL_PN_TRIANGLES_ATIX);
		}
		else
		{
			if(TruForm)
				glEnable(GL_PN_TRIANGLES_ATI);
		}
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(20.0f, 0.0f, -40.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(j, 0.0f, 0.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, FemaleTex);
		MD2_DrawFramei(&Female, Frame, Frame+1, i);
		glBindTexture(GL_TEXTURE_2D, WeaponTex);
		MD2_DrawFramei(&FemaleWeapon, Frame, Frame+1, i);
		if(ATIX)
		{
			if(TruForm)
				glEnable(GL_PN_TRIANGLES_ATIX);
		}
		else
		{
			if(TruForm)
				glEnable(GL_PN_TRIANGLES_ATI);
		}
	glPopMatrix();

	glPopMatrix();

	if(ATIX)
		glDisable(GL_PN_TRIANGLES_ATIX);
	else
		glDisable(GL_PN_TRIANGLES_ATI);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		if(TruForm)
		{
			Font_Print(0, 64, FontBase, FontTexture, "Tesselation level: %d", Tess);
			Font_Print(0, 48, FontBase, FontTexture, "TruForm enabled");
		}
		else
			Font_Print(0, 48, FontBase, FontTexture, "TruForm disabled");
		if(Lighting)
			Font_Print(0, 32, FontBase, FontTexture, "Lighting enabled");
		else
			Font_Print(0, 32, FontBase, FontTexture, "Lighting disabled");
		Font_Print(0, 16, FontBase, FontTexture, "%s", gluErrorString(glGetError()));
		Font_Print(0, 0, FontBase, FontTexture, OutBuffer);
	EndOverlay();
}

BOOL Init(void)
{
	float L0Color[]={ 1.0f, 1.0f, 1.0f };
	float L0Pos[]={ 0.0f, 0.0f, 1.0f, 1.0f };

	if(!IsExtensionSupported("GL_ATIX_pn_triangles"))
		if(!IsExtensionSupported("GL_ATI_pn_triangles"))
		{
			MessageBox(hWnd, "GL_ATIX_pn_triangles or GL_ATI_pn_triangles not found!", "OpenGL", MB_OK);
			return FALSE;
		}
		else
			glPNTrianglesiATI=(PFNGLPNTRIANGLESIATIPROC)wglGetProcAddress("glPNTrianglesiATI");
	else
	{
		glPNTrianglesiATIX=(PFNGLPNTRIANGLESIATIXPROC)wglGetProcAddress("glPNTrianglesiATIX");
		ATIX=TRUE;
	}

	if(ATIX)
		glGetIntegerv(GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATIX, &MaxTess);
	else
		glGetIntegerv(GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI, &MaxTess);

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	MaleTex=TGA_UploadImage("male.tga");
	FemaleTex=TGA_UploadImage("female.tga");
	WeaponTex=TGA_UploadImage("weapon.tga");

	MD2_LoadModel("male.md2", &Male);
	MD2_LoadModel("maleweapon.md2", &MaleWeapon);
	MD2_LoadModel("female.md2", &Female);
	MD2_LoadModel("femaleweapon.md2", &FemaleWeapon);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, L0Color);
	glLightfv(GL_LIGHT0, GL_POSITION, L0Pos);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if(ATIX)
	{
		glPNTrianglesiATIX(GL_PN_TRIANGLES_NORMAL_MODE_ATIX, GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATIX);
		glPNTrianglesiATIX(GL_PN_TRIANGLES_POINT_MODE_ATIX, GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATIX);
	}
	else
	{
		glPNTrianglesiATI(GL_PN_TRIANGLES_NORMAL_MODE_ATI, GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI);
		glPNTrianglesiATI(GL_PN_TRIANGLES_POINT_MODE_ATI, GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI);
	}

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
	gluPerspective(90.0f, (float)Width/Height, 0.1f, 400.0f);
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
