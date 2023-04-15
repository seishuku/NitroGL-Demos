#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include "glati.h"
#include "overlay.h"
#include "tga.h"
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

char *szAppName="EMBM";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE, ATIX=FALSE;

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames;
char OutBuffer[255];

BOOL Key[255];

unsigned int Font_Base, Font_Texture, Base, Bump, Envmap;
float x=15.0f, y=-15.0f, z, i;
float EMBMMatrix[4]={ 0.5f, 0.0f, 0.0f, -0.5f };

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
void DrawMesh(void);

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

	hWnd=CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

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
			if(Key[VK_RIGHT])
				x+=0.5f;

			if(Key[VK_LEFT])
				x-=0.5f;

			if(Key[VK_UP])
				y-=0.5f;

			if(Key[VK_DOWN])
				y+=0.5f;

			if(Key[VK_PRIOR])
				z+=0.005f;

			if(Key[VK_NEXT])
				z-=0.005f;

			Render();
			SwapBuffers(hDC);

			nFrames++;
			if(nFrames>10)
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

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
			}
			Key[wParam]=TRUE;
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
	glTranslatef(0.0f, 0.0f, z-0.5f);
	glRotatef(x, 0.0f, 1.0f, 0.0f);
	glRotatef(y, 1.0f, 0.0f, 0.0f);

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
		DrawMesh();
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 0, Font_Base, Font_Texture, OutBuffer);
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

	if(!IsExtensionSupported("GL_EXT_texture_env_combine"))
	{
		MessageBox(hWnd, "GL_EXT_texture_env_combine not found!", "OpenGL", MB_OK);
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


	Font_Base=Font_Build();
	Font_Texture=TGA_UploadImage("font.tga");

	Envmap=TGA_UploadImage("envmap.tga");
	Base=TGA_UploadImage("base.tga");
	Bump=TGA_UploadDUDV("bump.tga");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

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

	glViewport(0, 0, Width, Height);
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

void DrawMesh(void)
{
	int col=16, row=16, i, j;
	float mWidth=1.0f, mHeight=1.0f, stepx=mWidth/col, stepy=mHeight/row;

	float vertex[3], texcoord[2];

	for(i=0;i<row;i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		for(j=0;j<=col;j++)
		{
			vertex[0]=j*stepx-mWidth/2;
			vertex[1]=(i+1)*stepy-mHeight/2;
			vertex[2]=0.0f;

			texcoord[0]=(float)j/col;
			texcoord[1]=(float)(i+1)/row;

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texcoord[0], texcoord[1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, texcoord[0], texcoord[1]);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, texcoord[0], texcoord[1]);
			glVertex3f(vertex[0], vertex[1], vertex[2]);

			vertex[1]-=stepy;
			texcoord[1]=(float)i/row;

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, texcoord[0], texcoord[1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, texcoord[0], texcoord[1]);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, texcoord[0], texcoord[1]);
			glVertex3f(vertex[0], vertex[1], vertex[2]);
		}

		glEnd();
	}
}
