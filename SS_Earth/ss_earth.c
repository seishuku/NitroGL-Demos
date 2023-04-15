#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <scrnsave.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "resource.h"
#include "glati.h"
#include "tga.h"

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

HWND hChild=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

int Width, Height;

BOOL ATIX=FALSE;

unsigned int FontBase, FontTexture, Base, Bump, Envmap, Stars;

float EMBMMatrix[]={ 0.5f, 0.0f, 0.0f, -0.5f };

LARGE_INTEGER time_freq;
LARGE_INTEGER time_counter;
LARGE_INTEGER time;

float i;

char Path[255];

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLTEXBUMPPARAMETERFVATIXPROC glTexBumpParameterfvATIX=NULL;
PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI=NULL;

LRESULT WINAPI ScreenSaverChildProc(HWND  hWnd,UINT  msg, WPARAM  wParam, LPARAM  lParam);
LRESULT WINAPI ScreenSaverProc(HWND  hWnd,UINT  msg, WPARAM  wParam, LPARAM  lParam);
void Render(void);
BOOL Init(void);
BOOL Create(HWND hWnd);
void Destroy(HWND hWnd);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawSphere(float Radius, int Slice, int Stack);

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance = hMainInstance;
	WNDCLASS wc;
	RECT rect;
	char *temp;

	switch(msg)
	{
		case WM_CREATE:
			GetModuleFileName(hInstance, Path, MAX_PATH);
			temp=Path+strlen(Path);

			while(temp>=Path&&*temp!='\\')
				temp--;

			if(temp++>=Path)
				*temp=0;

			wc.style=CS_VREDRAW|CS_HREDRAW|CS_OWNDC;
			wc.lpfnWndProc=ScreenSaverChildProc;
			wc.cbClsExtra=0;
			wc.cbWndExtra=0;
			wc.hInstance=hInstance;
			wc.hIcon=NULL;
			wc.hCursor=NULL;
			wc.hbrBackground=NULL;
			wc.lpszMenuName=NULL;
			wc.lpszClassName="OpenGL";

			RegisterClass(&wc);

			GetClientRect(hWnd, &rect);

			hChild=CreateWindow(wc.lpszClassName, "OpenGL", WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, 0, 0, rect.right, rect.bottom, hWnd, NULL, hInstance, NULL);

			if(!Create(hChild))
			{
				PostQuitMessage(0);
				DestroyWindow(hChild);
				DestroyWindow(hWnd);
				return 1;
			}

			Resize(rect.right, rect.bottom);

			if(!Init())
			{
				PostQuitMessage(0);
				DestroyWindow(hChild);
				DestroyWindow(hWnd);
				return 1;
			}

			break;

		case WM_DESTROY:
			DestroyWindow(hChild);
			Destroy(hChild);
			break;

		default:
			return DefScreenSaverProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

LRESULT WINAPI ScreenSaverChildProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret;
	PAINTSTRUCT ps;

	switch(msg)
	{
		case WM_CREATE:
			QueryPerformanceFrequency(&time_freq);
			QueryPerformanceCounter(&time_counter);
			SetTimer(hWnd, 0, 33, NULL);
			break;

		case WM_DESTROY:
			KillTimer(hWnd, 0);
			PostQuitMessage(0);
			break;

		case WM_TIMER:
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_SIZE:
			Width=LOWORD(lParam);
			Height=HIWORD(lParam);
			Resize(Width, Height);
			break;

		case WM_PAINT:
			hDC=BeginPaint(hWnd, &ps);
			Render();
			SwapBuffers(hDC);
			EndPaint(hWnd, &ps);
			break;

		case WM_SYSCOMMAND:
		case WM_SETCURSOR:
		case WM_NCACTIVATE:
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
            ret=DefScreenSaverProc(hWnd, msg, wParam, lParam);
			DefScreenSaverProc(hMainWindow, msg, wParam, lParam);
			return ret;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInstance)
{
	return TRUE;
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
}

BOOL Init(void)
{
	int j;
	char temp[255];

	if(!IsExtensionSupported("GL_ARB_multitexture"))
	{
		MessageBox(NULL, "GL_ARB_multitexture not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
	{
		glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	}

	if(!IsExtensionSupported("GL_EXT_texture_env_combine"))
	{
		MessageBox(NULL, "GL_EXT_texture_env_combine not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_ATIX_envmap_bumpmap"))
		if(!IsExtensionSupported("GL_ATI_envmap_bumpmap"))
		{
			MessageBox(NULL, "GL_ATIX_envmap_bumpmap or GL_ATI_envmap_bumpmap not found!", "OpenGL", MB_OK);
			return FALSE;
		}
		else
			glTexBumpParameterfvATI=(PFNGLTEXBUMPPARAMETERFVATIPROC)wglGetProcAddress("glTexBumpParameterfvATI");
	else
	{
		glTexBumpParameterfvATIX=(PFNGLTEXBUMPPARAMETERFVATIXPROC)wglGetProcAddress("glTexBumpParameterfvATIX");
		ATIX=TRUE;
	}

	sprintf(temp, "%sbump.tga", Path);
	Bump=TGA_UploadDUDV(temp);
	sprintf(temp, "%sbase.tga", Path);
	Base=TGA_UploadImage(temp);
	sprintf(temp, "%senvmap.tga", Path);
	Envmap=TGA_UploadImage(temp);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(2.5f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

BOOL Create(HWND hWnd)
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

void Destroy(HWND hWnd)
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
