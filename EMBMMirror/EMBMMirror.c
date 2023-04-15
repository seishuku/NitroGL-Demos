#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "overlay.h"
#include "tga.h"
#include "font.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="EMBM Mirror";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE;

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames;
char OutBuffer[255];

BOOL Key[255];

unsigned int Font_Base, Font_Texture, Base, Bump, Envmap;
float x, y, z, i;
float EMBMMatrix[4]={ 0.2f, 0.0f, 0.0f, -0.2f };

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void RenderTexture(unsigned int TextureID, int x, int y, int width, int height);
void DrawMesh(void);
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
				z+=0.02f;

			if(Key[VK_NEXT])
				z-=0.02f;

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
	i+=0.5f;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 64, 64);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glPushMatrix();
	glPushMatrix();
		glTranslatef(-1.0f, 0.0f, -3.0f);
		glRotatef(-i, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawTorus(1.0f, 0.75f, 32, 32);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glRotatef(-i, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		DrawTorus(1.0f, 0.75f, 32, 32);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.0f, 0.0f, -3.0f);
		glRotatef(-i, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		DrawTorus(1.0f, 0.75f, 32, 32);
	glPopMatrix();
	glPopMatrix();
	RenderTexture(Envmap, 0, 0, 64, 64);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Width, Height);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -4.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

	glPushMatrix();
		glTranslatef(-1.0f, 0.5f, 0.0f);
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawTorus(0.75f, 0.5f, 32, 32);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0f, 0.5f, 0.0f);
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		DrawTorus(0.75f, 0.5f, 32, 32);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(1.0f, 0.5f, 0.0f);
		glRotatef(i, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		DrawTorus(0.75f, 0.5f, 32, 32);
	glPopMatrix();
	glDisable(GL_LIGHTING);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Envmap);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_ADD);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT,  GL_BUMP_ENVMAP_ATI);
	glTexEnvi(GL_TEXTURE_ENV, GL_BUMP_TARGET_ATI, GL_TEXTURE1_ARB);
	glTexBumpParameterfvATI(GL_BUMP_ROT_MATRIX_ATI, EMBMMatrix);

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -2.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		DrawMesh();
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 16, Font_Base, Font_Texture, "%s", gluErrorString(glGetError()));
		Font_Print(0, 0, Font_Base, Font_Texture, OutBuffer);
	EndOverlay();
}

BOOL Init(void)
{
	if(!IsExtensionSupported("GL_ARB_multitexture"))
	{
		MessageBox(hWnd, "GL_ARB_multitexture not found!", "OpenGL", MB_OK);
		return 0;
	}
	else
	{
		glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	}

	if(!IsExtensionSupported("GL_EXT_texture_env_combine"))
	{
		MessageBox(hWnd, "GL_EXT_texture_env_combine not found!", "OpenGL", MB_OK);
		return 0;
	}

	if(!IsExtensionSupported("GL_ATI_envmap_bumpmap"))
	{
		MessageBox(hWnd, "GL_ATI_envmap_bumpmap not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
		glTexBumpParameterfvATI=(PFNGLTEXBUMPPARAMETERFVATIPROC)wglGetProcAddress("glTexBumpParameterfvATI");

	Font_Base=Font_Build();
	Font_Texture=TGA_UploadImage("font.tga");

	Base=TGA_UploadImage("base.tga");
	Bump=TGA_UploadDUDV("bump.tga");

	glGenTextures(1, &Envmap);
	glBindTexture(GL_TEXTURE_2D, Envmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_COLOR_MATERIAL);

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

void RenderTexture(unsigned int TextureID, int x, int y, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, x, y, width, height);
//	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, x, y, width, height, 0);
}

void DrawMesh(void)
{
	glBegin(GL_TRIANGLE_STRIP);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 1.0f);
			glVertex2f(-1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 0.0f, 0.0f);
			glVertex2f(-1.0f, -1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 1.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE2_ARB, 1.0f, 0.0f);
			glVertex2f(1.0f, -1.0f);
	glEnd();
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

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)i/Slice*Texrep, (float)j/Slice*Texrep);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)i/Slice*Texrep, (float)j/Slice*Texrep);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (float)i/Slice*Texrep, (float)j/Slice*Texrep);
			glNormal3f(x0*c, y0*c, z/InnerRadius);
			glVertex3f(x0*r, y0*r, z);

			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)(i+1)/Slice*Texrep, (float)j/Slice*Texrep);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (float)(i+1)/Slice*Texrep, (float)j/Slice*Texrep);
			glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (float)(i+1)/Slice*Texrep, (float)j/Slice*Texrep);
			glNormal3f(x1*c, y1*c, z/InnerRadius);
			glVertex3f(x1*r, y1*r, z);
		}

		glEnd();
	}
}
