#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="DOT Product 3 Bump Mapping";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE, Key[255];

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames=0;
int nLastFrames=0;
char OutBuffer[255];

float x, y, z;

int i, mx, my;

float constcolor[]={ 0.75f, 0.75f, 0.75f };

unsigned int FontBase, FontTexture, Base, Bump, ColorNormal;

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawTorus(float OuterRadius, float InnerRadius, int Slice, int Stack);
unsigned int GenCubemap(void);
void Shader_DOT3(unsigned int Base, unsigned Bump, unsigned int Cubemap);
void Shader_DOT3Spec(unsigned int Bump, unsigned int Cubemap);
void Shader_Reset(void);

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

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, ColorNormal);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_DOT3_RGB_ARB);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, z-2.0f);
		glRotatef((float)mx-(Width/2), 0.0f, 1.0f, 0.0f);
		glRotatef((float)my-(Height/2), 1.0f, 0.0f, 0.0f);
		DrawTorus(1.0f, 0.5f, 32, 32);
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
	glDisable(GL_TEXTURE_GEN_R);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(mx, -my+Height, FontBase, FontTexture, "#");
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

	if(!IsExtensionSupported("GL_EXT_texture_env_combine"))
	{
		MessageBox(hWnd, "GL_EXT_texture_env_combine not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_ARB_texture_env_dot3"))
	{
		MessageBox(hWnd, "GL_EXT_texture_env_dot3 not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	ColorNormal=GenCubemap();
	Base=TGA_UploadImage("base.tga");
	Bump=TGA_UploadDOT3("bump.tga");

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

	glViewport(0,0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)Width/Height, 0.01f, 400.0f);
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

unsigned int GenCubemap(void)
{
	int x, y, z;
	int mapSize=256, halfMapSize=mapSize/2;
	unsigned int TextureID;
	unsigned char *cubemapPX, *cubemapNX, *cubemapPY, *cubemapNY, *cubemapPZ, *cubemapNZ;

	cubemapPX=malloc(mapSize*mapSize*3);
	cubemapNX=malloc(mapSize*mapSize*3);
	cubemapPY=malloc(mapSize*mapSize*3);
	cubemapNY=malloc(mapSize*mapSize*3);
	cubemapPZ=malloc(mapSize*mapSize*3);
	cubemapNZ=malloc(mapSize*mapSize*3);

	for(y=0;y<mapSize;y++)
	{
		for(z=0;z<mapSize;z++)
		{
			float fX=(float)halfMapSize;
			float fY=(float)halfMapSize-y;
			float fZ=(float)halfMapSize-z;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapPX[y*3*mapSize+z*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapPX[y*3*mapSize+z*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapPX[y*3*mapSize+z*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	for(y=0;y<mapSize;y++)
	{
		for(z=0;z<mapSize;z++)
		{
			float fX=(float)-halfMapSize;
			float fY=(float)halfMapSize-y;
			float fZ=(float)z-halfMapSize;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapNX[y*3*mapSize+z*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapNX[y*3*mapSize+z*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapNX[y*3*mapSize+z*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	for(z=0;z<mapSize;z++)
	{
		for(x=0;x<mapSize;x++)
		{
			float fX=(float)x-halfMapSize;
			float fY=(float)halfMapSize;
			float fZ=(float)z-halfMapSize;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapPY[z*3*mapSize+x*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapPY[z*3*mapSize+x*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapPY[z*3*mapSize+x*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	for(z=0;z<mapSize;z++)
	{
		for(x=0;x<mapSize;x++)
		{
			float fX=(float)x-halfMapSize;
			float fY=(float)-halfMapSize;
			float fZ=(float)halfMapSize-z;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapNY[z*3*mapSize+x*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapNY[z*3*mapSize+x*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapNY[z*3*mapSize+x*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	for(y=0;y<mapSize;y++)
	{
		for(x=0;x<mapSize;x++)
		{
			float fX=(float)x-halfMapSize;
			float fY=(float)halfMapSize-y;
			float fZ=(float)halfMapSize;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapPZ[y*3*mapSize+x*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapPZ[y*3*mapSize+x*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapPZ[y*3*mapSize+x*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	for(y=0;y<mapSize;y++)
	{
		for(x=0;x<mapSize;x++)
		{
			float fX=(float)halfMapSize-x;
			float fY=(float)halfMapSize-y;
			float fZ=(float)-halfMapSize;
			float oolen=1.0f/(float)sqrt(fX*fX+fY*fY+fZ*fZ);

			fX*=oolen;
			fY*=oolen;
			fZ*=oolen;
			cubemapNZ[y*3*mapSize+x*3+0]=(unsigned char)((((fX)+1.0f)/2.0f)*255.0f);
			cubemapNZ[y*3*mapSize+x*3+1]=(unsigned char)((((fY)+1.0f)/2.0f)*255.0f);
			cubemapNZ[y*3*mapSize+x*3+2]=(unsigned char)((((fZ)+1.0f)/2.0f)*255.0f);
		}
	}

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, TextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapPX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapNX);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapPY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapNY);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapPZ);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB8, mapSize, mapSize, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapNZ);

	free(cubemapPX);
	free(cubemapNX);
	free(cubemapPY);
	free(cubemapNY);
	free(cubemapPZ);
	free(cubemapNZ);

	return TextureID;
}
