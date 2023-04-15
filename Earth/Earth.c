#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"

#ifndef GL_ATIX_envmap_bumpmap
#define GL_ATIX_envmap_bumpmap 1
#define GL_BUMP_ROT_MATRIX_ATIX 0x60A0
#define GL_BUMP_ROT_MATRIX_SIZE_ATIX 0x60A1
#define GL_BUMP_NUM_TEX_UNITS_ATIX 0x60A2
#define GL_BUMP_TEX_UNITS_ATIX 0x60A3
#define GL_DUDV_ATIX 0x60A4
#define GL_DU8DV8_ATIX 0x60A5
#define GL_BUMP_ENVMAP_ATIX 0x60A6
#define GL_BUMP_TARGET_ATIX 0x60A7
typedef void (APIENTRY *PFNGLTEXBUMPPARAMETERIVATIXPROC)(GLenum pname, GLint *param);
typedef void (APIENTRY *PFNGLTEXBUMPPARAMETERFVATIXPROC)(GLenum pname, GLfloat *param);
typedef void (APIENTRY *PFNGLGETTEXBUMPPARAMETERIVATIXPROC)(GLenum pname, GLint *param);
typedef void (APIENTRY *PFNGLGETTEXBUMPPARAMETERFVATIXPROC)(GLenum pname, GLfloat *param);
#endif

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="EMBM Earth";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE, Key[255], ATIX=FALSE;

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

float i;
int j;

unsigned int FontBase, FontTexture, Base, Bump, Envmap, Stars;

float EMBMMatrix[]={ 0.5f, 0.0f, 0.0f, -0.5f };

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLTEXBUMPPARAMETERFVATIXPROC glTexBumpParameterfvATIX=NULL;
PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawSphere(float Radius, int Slice, int Stack);

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

	i+=(float)(time_counter.QuadPart-time.QuadPart)/time_freq.QuadPart*16;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Envmap);
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_ADD);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump);
	glEnable(GL_TEXTURE_2D);

	if(ATIX)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT,  GL_BUMP_ENVMAP_ATIX);
		glTexEnvi(GL_TEXTURE_ENV, GL_BUMP_TARGET_ATIX, GL_TEXTURE1_ARB);
		glTexBumpParameterfvATIX(GL_BUMP_ROT_MATRIX_ATIX, EMBMMatrix);
	}
	else
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT,  GL_BUMP_ENVMAP_ATI);
		glTexEnvi(GL_TEXTURE_ENV, GL_BUMP_TARGET_ATI, GL_TEXTURE1_ARB);
		glTexBumpParameterfvATI(GL_BUMP_ROT_MATRIX_ATI, EMBMMatrix);
	}

	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 0.0f, -2.25f);
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		DrawSphere(1.0f, 32, 32);
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -2.25f);
		glRotatef(-i, 0.0f, 1.0f, 0.0f);
		glScalef(1.5f, 1.5f, 1.5f);
		glCallList(Stars);
	glPopMatrix();

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 0, FontBase, FontTexture, OutBuffer);
	EndOverlay();
}

BOOL Init(void)
{
	if(!IsExtensionSupported("GL_ARB_multitexture"))
	{
		MessageBox(hWnd, "GL_ARB_multitexture not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
	{
		glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	}

	if(!IsExtensionSupported("GL_ARB_texture_env_combine"))
	{
		MessageBox(hWnd, "GL_ARB_texture_env_combine not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_ATIX_envmap_bumpmap"))
		if(!IsExtensionSupported("GL_ATI_envmap_bumpmap"))
		{
			MessageBox(hWnd, "GL_ATIX_envmap_bumpmap or GL_ATI_envmap_bumpmap not found!", "OpenGL", MB_OK);
			return FALSE;
		}
		else
			glTexBumpParameterfvATI=(PFNGLTEXBUMPPARAMETERFVATIPROC)wglGetProcAddress("glTexBumpParameterfvATI");
	else
	{
		glTexBumpParameterfvATIX=(PFNGLTEXBUMPPARAMETERFVATIXPROC)wglGetProcAddress("glTexBumpParameterfvATIX");
		ATIX=TRUE;
	}

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	Base=TGA_UploadImage("base.tga");
	Envmap=TGA_UploadImage("envmap.tga");
	Bump=TGA_UploadDUDV("bump.tga");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(2.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);

	Stars=glGenLists(1);
	glNewList(Stars, GL_COMPILE);

	glBegin(GL_POINTS);
	for(j=0;j<2500;j++)
	{
		float x=(float)(rand()%255)/127-1.0f;
		float y=(float)(rand()%255)/127-1.0f;
		float z=(float)(rand()%255)/127-1.0f;

		glNormal3f(x, y, z);
		glVertex3f(x, y, z);
	}
	glEnd();

	glEndList();


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
	gluPerspective(60.0f, (float)Width/Height, 0.1f, 400.0f);
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

void DrawSphere(float Radius, int Slice, int Stack)
{
	float SliceStep=(3.1415f/Slice);
	float StackStep=(2.0f*3.1415f/Stack);
	int i, j;

	for(i=0;i<Stack;i++)
	{
		float r0=Radius*(float)sin(i*SliceStep);
		float r1=Radius*(float)sin((i+1)*SliceStep);
		float y0=Radius*(float)cos(i*SliceStep);
		float y1=Radius*(float)cos((i+1)*SliceStep);

		glBegin(GL_TRIANGLE_STRIP);

		for(j=0;j<(Slice+1);j++)
		{
			float x0=r0*(float)sin(j*StackStep);
			float z0=r0*(float)cos(j*StackStep);
			float x1=r1*(float)sin(j*StackStep);
			float z1=r1*(float)cos(j*StackStep);

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)j/Slice, -i/(float)Stack);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)j/Slice, -i/(float)Stack);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (float)j/Slice, -i/(float)Stack);
			glNormal3f(x0, y0, z0);
			glVertex3f(x0, y0, z0);

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)j/Slice, -(i+1)/(float)Stack);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)j/Slice, -(i+1)/(float)Stack);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (float)j/Slice, -(i+1)/(float)Stack);
			glNormal3f(x1, y1, z1);
			glVertex3f(x1, y1, z1);
		}

		glEnd();

	}
}
