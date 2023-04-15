#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "tga.h"
#include "font.h"
#include "overlay.h"
#include "glati.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="Vertex Blend";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE;

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames=0;
int nLastFrames=0;
char OutBuffer[255];

unsigned int Font_Base, Font_Texture, Base;
int mx, my;
float i, x, y;

typedef struct
{
	float s, t;
} TexCoord2f_t;

typedef struct
{
	float x, y, z;
} Vertex3f_t;

typedef struct
{
	int NumVertices;
	Vertex3f_t *Weights;
	Vertex3f_t *Normals;
	TexCoord2f_t *TexCoords;
	Vertex3f_t *Vertices;
} Vertices_t;

Vertices_t Torus;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void InitTorus(Vertices_t *Torus, float OuterRadius, float InnerRadius, int Slice, int Stack);

PFNGLWEIGHTPOINTERARBPROC glWeightPointerARB=NULL;
PFNGLVERTEXBLENDARBPROC glVertexBlendARB=NULL;

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
			Render();
			SwapBuffers(hDC);

			nFrames++;
			if(nFrames>60)
			{
				QueryPerformanceCounter(&currentTime);
				fps=(float)nFrames/((float)(currentTime.QuadPart - startTime.QuadPart)/(float)timerFrequency.QuadPart);
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
			x=(float)(mx-(Width/2));
			y=(float)(my-(Height/2));
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				default:
					break;
			}
			break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Render(void)
{
	float matrix[16];

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_VERTEX_BLEND_ARB);
	glWeightPointerARB(3, GL_FLOAT, 0, Torus.Weights);
	glEnableClientState(GL_WEIGHT_ARRAY_ARB);
	glNormalPointer(GL_FLOAT, 0, Torus.Normals);
	glEnableClientState(GL_NORMAL_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, Torus.TexCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Torus.Vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glPushMatrix();
		glMatrixMode(GL_MODELVIEW0_ARB);
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

		glMatrixMode(GL_MODELVIEW1_ARB);
		glPushMatrix();
		glMultMatrixf(matrix);
		glRotatef(x, 0.0f, 1.0f, 0.0f);
		glRotatef(y, 1.0f, 0.0f, 0.0f);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, Torus.NumVertices);

		glMatrixMode(GL_MODELVIEW1_ARB);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW0_ARB);
		glPopMatrix();
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_WEIGHT_ARRAY_ARB);
	glDisable(GL_VERTEX_BLEND_ARB);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 0, Font_Base, Font_Texture, "FPS: %0.1f", fps);
		Font_Print(0, 16, Font_Base, Font_Texture, "%s", gluErrorString(glGetError()));
	EndOverlay();
}


BOOL Init(void)
{
	if(!IsExtensionSupported("GL_ARB_vertex_blend"))
	{
		MessageBox(hWnd, "GL_ARB_vertex_blend is not supported!", szAppName, MB_OK);
		return FALSE;
	}
	else
	{
		glWeightPointerARB=(PFNGLWEIGHTPOINTERARBPROC)wglGetProcAddress("glWeightPointerARB");
		glVertexBlendARB=(PFNGLVERTEXBLENDARBPROC)wglGetProcAddress("glVertexBlendARB");
	}

	Font_Base=Font_Build();
	Font_Texture=TGA_UploadImage("font.tga");

	Base=TGA_UploadImage("base.tga");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	InitTorus(&Torus, 1.0f, 0.75f, 64, 64);

	glVertexBlendARB(2);
	glEnable(GL_WEIGHT_SUM_UNITY_ARB);

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
	gluPerspective(90.0f, (float)Width/Height, 1.0f, 4096.0f);
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

void InitTorus(Vertices_t *Torus, float OuterRadius, float InnerRadius, int Slice, int Stack)
{
	int i, j, Texrep=8, NumVertices=0;
	float SliceStep=2.0f*3.1415f/Slice, StackStep=2.0f*3.1415f/Stack;
	float MinX=99999.0f, MaxX=-99999.0f, MinY=99999.0f, MaxY=-99999.0f, MinZ=99999.0f, MaxZ=-99999.0f;

	Torus->Weights=malloc(Slice*Stack*8*sizeof(float));
	Torus->Normals=malloc(Slice*Stack*8*sizeof(float));
	Torus->TexCoords=malloc(Slice*Stack*6*sizeof(float));
	Torus->Vertices=malloc(Slice*Stack*8*sizeof(float));

	for(i=0;i<=Slice;i++)
	{
		float a=i*SliceStep;
		float b=a+StackStep;
		float x0=(float)cos(a);
		float x1=(float)cos(b);
		float y0=(float)sin(a);
		float y1=(float)sin(b);

		for(j=0;j<=Stack;j++)
		{
			float c=(float)cos(j*StackStep);
			float r=InnerRadius*c+OuterRadius;
			float z=InnerRadius*(float)sin(j*StackStep);

			Torus->Normals[NumVertices].x=x0*c;
			Torus->Normals[NumVertices].y=y0*c;
			Torus->Normals[NumVertices].z=z/InnerRadius;

			Torus->TexCoords[NumVertices].s=(float)i/Slice*Texrep;
			Torus->TexCoords[NumVertices].t=(float)j/Slice*Texrep;

			Torus->Vertices[NumVertices].x=x0*r;
			Torus->Vertices[NumVertices].y=y0*r;
			Torus->Vertices[NumVertices].z=z;

			NumVertices++;

			Torus->Normals[NumVertices].x=x1*c;
			Torus->Normals[NumVertices].y=y1*c;
			Torus->Normals[NumVertices].z=z/InnerRadius;

			Torus->TexCoords[NumVertices].s=(float)(i+1)/Slice*Texrep;
			Torus->TexCoords[NumVertices].t=(float)j/Slice*Texrep;

			Torus->Vertices[NumVertices].x=x1*r;
			Torus->Vertices[NumVertices].y=y1*r;
			Torus->Vertices[NumVertices].z=z;

			NumVertices++;
		}
	}

	for(i=0;i<NumVertices;i++)
	{
		MinX=min(Torus->Vertices[i].x, MinX);
		MaxX=max(Torus->Vertices[i].x, MaxX);
		MinY=min(Torus->Vertices[i].y, MinY);
		MaxY=max(Torus->Vertices[i].y, MaxY);
		MinZ=min(Torus->Vertices[i].z, MinZ);
		MaxZ=max(Torus->Vertices[i].z, MaxZ);
	}

	for(i=0;i<NumVertices;i++)
	{
		float a=(Torus->Vertices[i].x-MinX)/(MaxX-MinX);
		float b=(Torus->Vertices[i].y-MinY)/(MaxY-MinY);
		float c=(Torus->Vertices[i].z-MinZ)/(MaxZ-MinZ);

		Torus->Weights[i].x=(1.0f-(float)sin(a*3.1415926f*1.0f));
		Torus->Weights[i].y=(1.0f-(float)sin(b*3.1415926f*1.0f));
		Torus->Weights[i].z=(1.0f-(float)sin(c*3.1415926f*1.0f));
	}

	Torus->NumVertices=NumVertices;
}
