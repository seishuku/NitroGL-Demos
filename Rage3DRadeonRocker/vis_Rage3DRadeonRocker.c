#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "glati.h"
#include "tga.h"
#include "vis.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="Rage3D Radeon Rocker";

RECT WindowRect;

int Width=800, Height=600;

unsigned int Base, Bump, Envmap, Background;
float j, k, l;
float EMBMMatrix[4]={ 0.5f, 0.0f, 0.0f, 0.5f };

char Path[255], temp[255];

winampVisModule	*GetModule(int which);
void Config(winampVisModule *this_mod);
int AppMain(winampVisModule *this_mod);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int Render(winampVisModule *this_mod);
BOOL Init(void);
BOOL Create(void);
void Destroy(winampVisModule *this_mod);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawQuad(void);

winampVisHeader	hdr={ VIS_HDRVER, "Rage3d Radeon Rocker", GetModule };
winampVisModule Test={ "Radeon Rocker", NULL, NULL, 0, 0, 0, 12, 2, 2, { 0, }, { 0, }, Config, AppMain, Render, Destroy };

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLTEXBUMPPARAMETERFVATIPROC glTexBumpParameterfvATI=NULL;

#ifdef __cplusplus
extern "C" {
#endif
__declspec( dllexport ) winampVisHeader *winampVisGetHeader()
{
	return &hdr;
}
#ifdef __cplusplus
}
#endif

winampVisModule	*GetModule(int which)
{
	switch(which)
	{
		case 0:
			return &Test;
		default:
			return NULL;
	}
}

void Config(struct winampVisModule *this_mod)
{
	MessageBox(hWnd, "This Plug-In is made by NitroGL Productions.\n\nSome content is copyright by Rage3d.com and ATI.", "Rage3d Radeon Rocker", MB_OK);
}

int AppMain(struct winampVisModule *this_mod)
{
	WNDCLASS wc;
	char *temp;

	wc.style=CS_VREDRAW|CS_HREDRAW|CS_OWNDC;
	wc.lpfnWndProc=WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=this_mod->hDllInstance;
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

	hWnd=CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, this_mod->hwndParent, NULL, this_mod->hDllInstance, NULL);

	SetWindowLong(hWnd, GWL_USERDATA, (LONG)this_mod);
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);

	GetModuleFileName(this_mod->hDllInstance, Path, MAX_PATH);
	temp=Path+strlen(Path);

	while(temp>=Path&&*temp!='\\')
		temp--;

	if(temp++>=Path)*temp=0;

	if(!Create())
	{
		MessageBox(hWnd, "Failed to create OpenGL context!", "OpenGL", MB_OK);
		return 1;
	}

	if(!Init())
	{
		MessageBox(hWnd, "Failed to init!", "OpenGL", MB_OK);
		Destroy(this_mod);
		return 1;
	}

	Resize(Width, Height);


	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
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
			{
				winampVisModule *this_mod=(winampVisModule *)GetWindowLong(hWnd, GWL_USERDATA);
				PostMessage(this_mod->hwndParent, uMsg, wParam, lParam);
			}
			break;

		case WM_KEYUP:
			{
				winampVisModule *this_mod=(winampVisModule *)GetWindowLong(hWnd, GWL_USERDATA);
				PostMessage(this_mod->hwndParent, uMsg, wParam, lParam);
			}
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int Render(struct winampVisModule *this_mod)
{
	int i;
	float low=(float)this_mod->spectrumData[0][0]/576;
	float mid=(float)this_mod->spectrumData[0][72]/576*2;
	float high=(float)this_mod->spectrumData[0][144]/576*2;

	EMBMMatrix[0]=low/1.75f;
	EMBMMatrix[3]=low/1.75f;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, Background);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glScalef(4.0f, 3.0f, 3.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		DrawQuad();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(-0.2f, 0.2f, -1.75f);

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
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT,  GL_BUMP_ENVMAP_ATI);
	glTexEnvi(GL_TEXTURE_ENV, GL_BUMP_TARGET_ATI, GL_TEXTURE1_ARB);
	glTexBumpParameterfvATI(GL_BUMP_ROT_MATRIX_ATI, EMBMMatrix);

	glPushMatrix();
		k+=0.05f;
		j=(float)sin(k)*4.0f;
		l=(float)cos(k)*4.0f;
		glScalef(1.0f+mid, 1.0f+high, 1.0f);
		glRotatef(j, 1.0f, 0.0f, 0.0f);
		glRotatef(l, 0.0f, 1.0f, 0.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
		DrawQuad();
		glDisable(GL_BLEND);
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glTranslatef(-0.9f, -0.7f, -0.75f);
	glPushMatrix();
		glBegin(GL_LINES);
		for(i=0;i<576;i++)
		{
			float level=(float)this_mod->spectrumData[0][i]/576*2;

			glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
			glVertex2f((float)i/576, 0.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			glVertex2f((float)i/576, level);
		}
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_LINES);
		for(i=0;i<576;i++)
		{
			float level=(float)this_mod->spectrumData[1][i]/576*2;

			glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
			glVertex2f(-(float)i/576+1.8f, 0.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
			glVertex2f(-(float)i/576+1.8f, level);
		}
		glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);

	glPopMatrix();

	SwapBuffers(hDC);

	return 0;
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

	if(!IsExtensionSupported("GL_ATI_envmap_bumpmap"))
	{
		MessageBox(hWnd, "GL_ATI_envmap_bumpmap not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
		glTexBumpParameterfvATI=(PFNGLTEXBUMPPARAMETERFVATIPROC)wglGetProcAddress("glTexBumpParameterfvATI");

	sprintf(temp, "%sbase.tga", Path);
	Base=TGA_UploadImage(temp);

	sprintf(temp, "%sbump.tga", Path);
	Bump=TGA_UploadDUDV(temp);

	sprintf(temp, "%senvmap.tga", Path);
	Envmap=TGA_UploadImage(temp);

	sprintf(temp, "%sbackground.tga", Path);
	Background=TGA_UploadImage(temp);

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

void Destroy(struct winampVisModule *this_mod)
{
	glDeleteTextures(1, &Base);
	glDeleteTextures(1, &Bump);
	glDeleteTextures(1, &Envmap);
	glDeleteTextures(1, &Background);

	wglMakeCurrent(hDC, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
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

void DrawQuad(void)
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
