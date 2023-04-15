#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "image.h"
#include "overlay.h"
#include "font.h"
#include "3ds.h"
#include "quat.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256];

LARGE_INTEGER timer_freq, startTime, currentTime, time_counter, time_current;
float fps=0.0f, fTime=0.0f;
int nFrames=0;

typedef struct
{
	POINT pt;
	int x, y;
	int oldx, oldy;
	int dx, dy;
} mouse_t;

mouse_t mouse;

float RotateX, RotateY, Zoom;
float quat[4], matrix[16];

unsigned int FontTexture, Base;
unsigned int VertexShader[1];

TDS_Model_t Model[1];

PFNGLVERTEXATTRIB1SARBPROC glVertexAttrib1sARB=NULL;
PFNGLVERTEXATTRIB1FARBPROC glVertexAttrib1fARB=NULL;
PFNGLVERTEXATTRIB1DARBPROC glVertexAttrib1dARB=NULL;
PFNGLVERTEXATTRIB2SARBPROC glVertexAttrib2sARB=NULL;
PFNGLVERTEXATTRIB2FARBPROC glVertexAttrib2fARB=NULL;
PFNGLVERTEXATTRIB2DARBPROC glVertexAttrib2dARB=NULL;
PFNGLVERTEXATTRIB3SARBPROC glVertexAttrib3sARB=NULL;
PFNGLVERTEXATTRIB3FARBPROC glVertexAttrib3fARB=NULL;
PFNGLVERTEXATTRIB3DARBPROC glVertexAttrib3dARB=NULL;
PFNGLVERTEXATTRIB4SARBPROC glVertexAttrib4sARB=NULL;
PFNGLVERTEXATTRIB4FARBPROC glVertexAttrib4fARB=NULL;
PFNGLVERTEXATTRIB4DARBPROC glVertexAttrib4dARB=NULL;
PFNGLVERTEXATTRIB4NUBARBPROC glVertexAttrib4NubARB=NULL;
PFNGLVERTEXATTRIB1SVARBPROC glVertexAttrib1svARB=NULL;
PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB=NULL;
PFNGLVERTEXATTRIB1DVARBPROC glVertexAttrib1dvARB=NULL;
PFNGLVERTEXATTRIB2SVARBPROC glVertexAttrib2svARB=NULL;
PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB=NULL;
PFNGLVERTEXATTRIB2DVARBPROC glVertexAttrib2dvARB=NULL;
PFNGLVERTEXATTRIB3SVARBPROC glVertexAttrib3svARB=NULL;
PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB=NULL;
PFNGLVERTEXATTRIB3DVARBPROC glVertexAttrib3dvARB=NULL;
PFNGLVERTEXATTRIB4BVARBPROC glVertexAttrib4bvARB=NULL;
PFNGLVERTEXATTRIB4SVARBPROC glVertexAttrib4svARB=NULL;
PFNGLVERTEXATTRIB4IVARBPROC glVertexAttrib4ivARB=NULL;
PFNGLVERTEXATTRIB4UBVARBPROC glVertexAttrib4ubvARB=NULL;
PFNGLVERTEXATTRIB4USVARBPROC glVertexAttrib4usvARB=NULL;
PFNGLVERTEXATTRIB4UIVARBPROC glVertexAttrib4uivARB=NULL;
PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB=NULL;
PFNGLVERTEXATTRIB4DVARBPROC glVertexAttrib4dvARB=NULL;
PFNGLVERTEXATTRIB4NBVARBPROC glVertexAttrib4NbvARB=NULL;
PFNGLVERTEXATTRIB4NSVARBPROC glVertexAttrib4NsvARB=NULL;
PFNGLVERTEXATTRIB4NIVARBPROC glVertexAttrib4NivARB=NULL;
PFNGLVERTEXATTRIB4NUBVARBPROC glVertexAttrib4NubvARB=NULL;
PFNGLVERTEXATTRIB4NUSVARBPROC glVertexAttrib4NusvARB=NULL;
PFNGLVERTEXATTRIB4NUIVARBPROC glVertexAttrib4NuivARB=NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointerARB=NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArrayARB=NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC glDisableVertexAttribArrayARB=NULL;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB=NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB=NULL;
PFNGLDELETEPROGRAMSARBPROC glDeleteProgramsARB=NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB=NULL;
PFNGLPROGRAMENVPARAMETER4DARBPROC glProgramEnvParameter4dARB=NULL;
PFNGLPROGRAMENVPARAMETER4DVARBPROC glProgramEnvParameter4dvARB=NULL;
PFNGLPROGRAMENVPARAMETER4FARBPROC glProgramEnvParameter4fARB=NULL;
PFNGLPROGRAMENVPARAMETER4FVARBPROC glProgramEnvParameter4fvARB=NULL;
PFNGLPROGRAMLOCALPARAMETER4DARBPROC glProgramLocalParameter4dARB=NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARBPROC glProgramLocalParameter4dvARB=NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB=NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB=NULL;
PFNGLGETPROGRAMENVPARAMETERDVARBPROC glGetProgramEnvParameterdvARB=NULL;
PFNGLGETPROGRAMENVPARAMETERFVARBPROC glGetProgramEnvParameterfvARB=NULL;
PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC glGetProgramLocalParameterdvARB=NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC glGetProgramLocalParameterfvARB=NULL;
PFNGLGETPROGRAMIVARBPROC glGetProgramivARB=NULL;
PFNGLGETPROGRAMSTRINGARBPROC glGetProgramStringARB=NULL;
PFNGLGETVERTEXATTRIBDVARBPROC glGetVertexAttribdvARB=NULL;
PFNGLGETVERTEXATTRIBFVARBPROC glGetVertexAttribfvARB=NULL;
PFNGLGETVERTEXATTRIBIVARBPROC glGetVertexAttribivARB=NULL;
PFNGLGETVERTEXATTRIBPOINTERVARBPROC glGetVertexAttribPointervARB=NULL;
PFNGLISPROGRAMARBPROC glIsProgramARB=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void BuildVertexShader(unsigned int *ShaderID);

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
	wc.hbrBackground=GetStockObject(BLACK_BRUSH);
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
				fps=(float)nFrames/((float)(currentTime.QuadPart - startTime.QuadPart)/(float)timer_freq.QuadPart);
				nFrames=0;
				QueryPerformanceCounter(&startTime);
			}
		}
	}

	TDS_Delete(&Model[0]);

	Destroy();

	DestroyWindow(hWnd);

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			QueryPerformanceCounter(&startTime);
			QueryPerformanceCounter(&time_counter);
			QueryPerformanceFrequency(&timer_freq);
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

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			GetCursorPos(&mouse.pt);
			ShowCursor(FALSE);
			mouse.x=mouse.pt.x;
			mouse.y=mouse.pt.y;
			mouse.oldx=mouse.x;
			mouse.oldy=mouse.y;
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if(!(wParam&(MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
			{
				ShowCursor(TRUE);
				ReleaseCapture();
			}
			break;

		case WM_MOUSEMOVE:
			GetCursorPos(&mouse.pt);
			mouse.x=mouse.pt.x;
			mouse.y=mouse.pt.y;

			if(!wParam)
			{
				mouse.oldx=mouse.x;
				mouse.oldy=mouse.y;
				break;
			}

			mouse.dx=mouse.x-mouse.oldx;
			mouse.dy=mouse.oldy-mouse.y;

			if(!mouse.dx&&!mouse.dy)
				break;

			SetCursorPos(mouse.oldx, mouse.oldy);

			if(wParam==MK_LBUTTON)
			{
				RotateX+=mouse.dx;
				RotateY-=mouse.dy;
			}

			if(wParam==MK_RBUTTON)
				Zoom+=mouse.dy;
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
	float QuatX[4], QuatY[4];

	time_current=time_counter;
	QueryPerformanceCounter(&time_counter);
	fTime+=1.0f*(float)(time_counter.QuadPart-time_current.QuadPart)/timer_freq.QuadPart;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, Zoom-150.0f);
	QuatAngle(QuatX, RotateX, 0.0f, 1.0f, 0.0f);
	QuatAngle(QuatY, RotateY, 1.0f, 0.0f, 0.0f);
	QuatMultiply(QuatX, QuatY, quat);
	QuatMatrix(quat, matrix);
	glMultMatrixf(matrix);

	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexShader[0]);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, fTime, 0.0f, 0.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	TDS_Render(&Model[0]);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_VERTEX_PROGRAM_ARB);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Font_Print(0, 16, FontTexture, "%s\nFPS: %0.1f", gluErrorString(glGetError()), fps);
	EndOverlay();
}

BOOL Init(void)
{
	if(!IsExtensionSupported("GL_ARB_vertex_program"))
	{
		MessageBox(hWnd, "GL_ARB_vertex_program not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
	{
		glVertexAttrib1sARB=(PFNGLVERTEXATTRIB1SARBPROC)wglGetProcAddress("glVertexAttrib1sARB");
		glVertexAttrib1fARB=(PFNGLVERTEXATTRIB1FARBPROC)wglGetProcAddress("glVertexAttrib1fARB");
		glVertexAttrib1dARB=(PFNGLVERTEXATTRIB1DARBPROC)wglGetProcAddress("glVertexAttrib1dARB");
		glVertexAttrib2sARB=(PFNGLVERTEXATTRIB2SARBPROC)wglGetProcAddress("glVertexAttrib2sARB");
		glVertexAttrib2fARB=(PFNGLVERTEXATTRIB2FARBPROC)wglGetProcAddress("glVertexAttrib2fARB");
		glVertexAttrib2dARB=(PFNGLVERTEXATTRIB2DARBPROC)wglGetProcAddress("glVertexAttrib2dARB");
		glVertexAttrib3sARB=(PFNGLVERTEXATTRIB3SARBPROC)wglGetProcAddress("glVertexAttrib3sARB");
		glVertexAttrib3fARB=(PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");
		glVertexAttrib3dARB=(PFNGLVERTEXATTRIB3DARBPROC)wglGetProcAddress("glVertexAttrib3dARB");
		glVertexAttrib4sARB=(PFNGLVERTEXATTRIB4SARBPROC)wglGetProcAddress("glVertexAttrib4sARB");
		glVertexAttrib4fARB=(PFNGLVERTEXATTRIB4FARBPROC)wglGetProcAddress("glVertexAttrib4fARB");
		glVertexAttrib4dARB=(PFNGLVERTEXATTRIB4DARBPROC)wglGetProcAddress("glVertexAttrib4dARB");
		glVertexAttrib4NubARB=(PFNGLVERTEXATTRIB4NUBARBPROC)wglGetProcAddress("glVertexAttrib4NubARB");
		glVertexAttrib1svARB=(PFNGLVERTEXATTRIB1SVARBPROC)wglGetProcAddress("glVertexAttrib1svARB");
		glVertexAttrib1fvARB=(PFNGLVERTEXATTRIB1FVARBPROC)wglGetProcAddress("glVertexAttrib1fvARB");
		glVertexAttrib1dvARB=(PFNGLVERTEXATTRIB1DVARBPROC)wglGetProcAddress("glVertexAttrib1dvARB");
		glVertexAttrib2svARB=(PFNGLVERTEXATTRIB2SVARBPROC)wglGetProcAddress("glVertexAttrib2svARB");
		glVertexAttrib2fvARB=(PFNGLVERTEXATTRIB2FVARBPROC)wglGetProcAddress("glVertexAttrib2fvARB");
		glVertexAttrib2dvARB=(PFNGLVERTEXATTRIB2DVARBPROC)wglGetProcAddress("glVertexAttrib2dvARB");
		glVertexAttrib3svARB=(PFNGLVERTEXATTRIB3SVARBPROC)wglGetProcAddress("glVertexAttrib3svARB");
		glVertexAttrib3fvARB=(PFNGLVERTEXATTRIB3FVARBPROC)wglGetProcAddress("glVertexAttrib3fvARB");
		glVertexAttrib3dvARB=(PFNGLVERTEXATTRIB3DVARBPROC)wglGetProcAddress("glVertexAttrib3dvARB");
		glVertexAttrib4bvARB=(PFNGLVERTEXATTRIB4BVARBPROC)wglGetProcAddress("glVertexAttrib4bvARB");
		glVertexAttrib4svARB=(PFNGLVERTEXATTRIB4SVARBPROC)wglGetProcAddress("glVertexAttrib4svARB");
		glVertexAttrib4ivARB=(PFNGLVERTEXATTRIB4IVARBPROC)wglGetProcAddress("glVertexAttrib4ivARB");
		glVertexAttrib4ubvARB=(PFNGLVERTEXATTRIB4UBVARBPROC)wglGetProcAddress("glVertexAttrib4ubvARB");
		glVertexAttrib4usvARB=(PFNGLVERTEXATTRIB4USVARBPROC)wglGetProcAddress("glVertexAttrib4usvARB");
		glVertexAttrib4uivARB=(PFNGLVERTEXATTRIB4UIVARBPROC)wglGetProcAddress("glVertexAttrib4uivARB");
		glVertexAttrib4fvARB=(PFNGLVERTEXATTRIB4FVARBPROC)wglGetProcAddress("glVertexAttrib4fvARB");
		glVertexAttrib4dvARB=(PFNGLVERTEXATTRIB4DVARBPROC)wglGetProcAddress("glVertexAttrib4dvARB");
		glVertexAttrib4NbvARB=(PFNGLVERTEXATTRIB4NBVARBPROC)wglGetProcAddress("glVertexAttrib4NbvARB");
		glVertexAttrib4NsvARB=(PFNGLVERTEXATTRIB4NSVARBPROC)wglGetProcAddress("glVertexAttrib4NsvARB");
		glVertexAttrib4NivARB=(PFNGLVERTEXATTRIB4NIVARBPROC)wglGetProcAddress("glVertexAttrib4NivARB");
		glVertexAttrib4NubvARB=(PFNGLVERTEXATTRIB4NUBVARBPROC)wglGetProcAddress("glVertexAttrib4NubvARB");
		glVertexAttrib4NusvARB=(PFNGLVERTEXATTRIB4NUSVARBPROC)wglGetProcAddress("glVertexAttrib4NusvARB");
		glVertexAttrib4NuivARB=(PFNGLVERTEXATTRIB4NUIVARBPROC)wglGetProcAddress("glVertexAttrib4NuivARB");
		glVertexAttribPointerARB=(PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
		glEnableVertexAttribArrayARB=(PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
		glDisableVertexAttribArrayARB=(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");
		glProgramStringARB=(PFNGLPROGRAMSTRINGARBPROC)wglGetProcAddress("glProgramStringARB");
		glBindProgramARB=(PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
		glDeleteProgramsARB=(PFNGLDELETEPROGRAMSARBPROC)wglGetProcAddress("glDeleteProgramsARB");
		glGenProgramsARB=(PFNGLGENPROGRAMSARBPROC)wglGetProcAddress("glGenProgramsARB");
		glProgramEnvParameter4dARB=(PFNGLPROGRAMENVPARAMETER4DARBPROC)wglGetProcAddress("glProgramEnvParameter4dARB");
		glProgramEnvParameter4dvARB=(PFNGLPROGRAMENVPARAMETER4DVARBPROC)wglGetProcAddress("glProgramEnvParameter4dvARB");
		glProgramEnvParameter4fARB=(PFNGLPROGRAMENVPARAMETER4FARBPROC)wglGetProcAddress("glProgramEnvParameter4fARB");
		glProgramEnvParameter4fvARB=(PFNGLPROGRAMENVPARAMETER4FVARBPROC)wglGetProcAddress("glProgramEnvParameter4fvARB");
		glProgramLocalParameter4dARB=(PFNGLPROGRAMLOCALPARAMETER4DARBPROC)wglGetProcAddress("glProgramLocalParameter4dARB");
		glProgramLocalParameter4dvARB=(PFNGLPROGRAMLOCALPARAMETER4DVARBPROC)wglGetProcAddress("glProgramLocalParameter4dvARB");
		glProgramLocalParameter4fARB=(PFNGLPROGRAMLOCALPARAMETER4FARBPROC)wglGetProcAddress("glProgramLocalParameter4fARB");
		glProgramLocalParameter4fvARB=(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)wglGetProcAddress("glProgramLocalParameter4fvARB");
		glGetProgramEnvParameterdvARB=(PFNGLGETPROGRAMENVPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramEnvParameterdvARB");
		glGetProgramEnvParameterfvARB=(PFNGLGETPROGRAMENVPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramEnvParameterfvARB");
		glGetProgramLocalParameterdvARB=(PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC)wglGetProcAddress("glGetProgramLocalParameterdvARB");
		glGetProgramLocalParameterfvARB=(PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC)wglGetProcAddress("glGetProgramLocalParameterfvARB");
		glGetProgramivARB=(PFNGLGETPROGRAMIVARBPROC)wglGetProcAddress("glGetProgramivARB");
		glGetProgramStringARB=(PFNGLGETPROGRAMSTRINGARBPROC)wglGetProcAddress("glGetProgramStringARB");
		glGetVertexAttribdvARB=(PFNGLGETVERTEXATTRIBDVARBPROC)wglGetProcAddress("glGetVertexAttribdvARB");
		glGetVertexAttribfvARB=(PFNGLGETVERTEXATTRIBFVARBPROC)wglGetProcAddress("glGetVertexAttribfvARB");
		glGetVertexAttribivARB=(PFNGLGETVERTEXATTRIBIVARBPROC)wglGetProcAddress("glGetVertexAttribivARB");
		glGetVertexAttribPointervARB=(PFNGLGETVERTEXATTRIBPOINTERVARBPROC)wglGetProcAddress("glGetVertexAttribPointervARB");
		glIsProgramARB=(PFNGLISPROGRAMARBPROC)wglGetProcAddress("glIsProgramARB");
	}

	FontTexture=IMAGE_UploadImage("font.tga");

	Base=IMAGE_UploadImage("base.tga");

	TDS_LoadModel(&Model[0], "quad.3ds");

	BuildVertexShader(VertexShader);

	glEnable(GL_DEPTH_TEST);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
	pfd.cStencilBits=8;

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

	DescribePixelFormat(hDC, 0, PixelFormat, &pfd);

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
	gluPerspective(90.0f, (float)Width/Height, 0.01f, 4096.0f);
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

char *read_text_file(const char *filename)
{
	long bytes;
	char *buffer;
	FILE *stream;

	if(!(stream=fopen(filename, "rt")))
		return 0;

	fseek(stream, 0, SEEK_END);
	bytes=ftell(stream);
	fseek(stream, 0, SEEK_SET);

	buffer=(char *)malloc(sizeof(char)*bytes+1);

	bytes=fread(buffer, 1, bytes, stream);

	buffer[bytes]=0;

	fclose(stream);

	return buffer;
}

void CheckVertexProgramError(void)
{
	char errorstring[8192];
	int errorposition;

	sprintf(errorstring, "%s", glGetString(GL_PROGRAM_ERROR_STRING_ARB));
	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorposition);

	if(errorposition==-1)
		return;

	MessageBox(hWnd, errorstring, "NULL", MB_OK);
}

void BuildVertexShader(unsigned int *ShaderID)
{
	char *Program=read_text_file("vp.txt");

	glGenProgramsARB(1, &VertexShader[0]);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexShader[0]);
	glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	CheckVertexProgramError();

	free(Program);
}
