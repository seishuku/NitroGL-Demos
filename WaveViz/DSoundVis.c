#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmreg.h>
#include <dsound.h>
#include <stdio.h>
#include <math.h>
#include "glati.h"
#include "tga.h"

#define SAFE_RELEASE(p) { if(p) { (p)->lpVtbl->Release(p); (p)=NULL; } }

LPDIRECTSOUNDCAPTURE lpDSCapture=NULL;
LPDIRECTSOUNDCAPTUREBUFFER lpDSBCapture=NULL;
WAVEFORMATEX waveformat;

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="Wave Viz";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256];

LARGE_INTEGER timerFrequency, startTime, currentTime;
float fps;
int nFrames=0;

unsigned int Base, Base1, TransferMap, FragmentShader;

int texwidth=32, texheight=32;
int size;

BOOL IsFragmentShaderSupported=FALSE;

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB=NULL;
PFNGLMULTITEXCOORD1DARBPROC glMultiTexCoord1dARB=NULL;
PFNGLMULTITEXCOORD1DVARBPROC glMultiTexCoord1dvARB=NULL;
PFNGLMULTITEXCOORD1FARBPROC glMultiTexCoord1fARB=NULL;
PFNGLMULTITEXCOORD1FVARBPROC glMultiTexCoord1fvARB=NULL;
PFNGLMULTITEXCOORD1IARBPROC glMultiTexCoord1iARB=NULL;
PFNGLMULTITEXCOORD1IVARBPROC glMultiTexCoord1ivARB=NULL;
PFNGLMULTITEXCOORD1SARBPROC glMultiTexCoord1sARB=NULL;
PFNGLMULTITEXCOORD1SVARBPROC glMultiTexCoord1svARB=NULL;
PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB=NULL;
PFNGLMULTITEXCOORD2DVARBPROC glMultiTexCoord2dvARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB=NULL;
PFNGLMULTITEXCOORD2IARBPROC glMultiTexCoord2iARB=NULL;
PFNGLMULTITEXCOORD2IVARBPROC glMultiTexCoord2ivARB=NULL;
PFNGLMULTITEXCOORD2SARBPROC glMultiTexCoord2sARB=NULL;
PFNGLMULTITEXCOORD2SVARBPROC glMultiTexCoord2svARB=NULL;
PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB=NULL;
PFNGLMULTITEXCOORD3DVARBPROC glMultiTexCoord3dvARB=NULL;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB=NULL;
PFNGLMULTITEXCOORD3FVARBPROC glMultiTexCoord3fvARB=NULL;
PFNGLMULTITEXCOORD3IARBPROC glMultiTexCoord3iARB=NULL;
PFNGLMULTITEXCOORD3IVARBPROC glMultiTexCoord3ivARB=NULL;
PFNGLMULTITEXCOORD3SARBPROC glMultiTexCoord3sARB=NULL;
PFNGLMULTITEXCOORD3SVARBPROC glMultiTexCoord3svARB=NULL;
PFNGLMULTITEXCOORD4DARBPROC glMultiTexCoord4dARB=NULL;
PFNGLMULTITEXCOORD4DVARBPROC glMultiTexCoord4dvARB=NULL;
PFNGLMULTITEXCOORD4FARBPROC glMultiTexCoord4fARB=NULL;
PFNGLMULTITEXCOORD4FVARBPROC glMultiTexCoord4fvARB=NULL;
PFNGLMULTITEXCOORD4IARBPROC glMultiTexCoord4iARB=NULL;
PFNGLMULTITEXCOORD4IVARBPROC glMultiTexCoord4ivARB=NULL;
PFNGLMULTITEXCOORD4SARBPROC glMultiTexCoord4sARB=NULL;
PFNGLMULTITEXCOORD4SVARBPROC glMultiTexCoord4svARB=NULL;

PFNGLGENFRAGMENTSHADERSATIPROC glGenFragmentShadersATI=NULL;
PFNGLBINDFRAGMENTSHADERATIPROC glBindFragmentShaderATI=NULL;
PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI=NULL;
PFNGLBEGINFRAGMENTSHADERATIPROC glBeginFragmentShaderATI=NULL;
PFNGLENDFRAGMENTSHADERATIPROC glEndFragmentShaderATI=NULL;
PFNGLPASSTEXCOORDATIPROC glPassTexCoordATI=NULL;
PFNGLSAMPLEMAPATIPROC glSampleMapATI=NULL;
PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI=NULL;
PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI=NULL;
PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI=NULL;
PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI=NULL;
PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI=NULL;
PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI=NULL;
PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL CreateDirectSound(void);
void DestroyDirectSound(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
unsigned int BuildFragmentShader(void);

extern BOOL IsExtensionSupported(const char *extension);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	size=texwidth*texheight*2;

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

//	AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

//	hWnd=CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

	hWnd=CreateWindow(szAppName, szAppName, WS_POPUP|WS_CLIPSIBLINGS, 0, 0, 1280, 1024, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);

	if(!Create())
		Done=TRUE;

	if(Done==FALSE)
		Resize(Width, Height);

	if(Done==FALSE)
		if(!Init())
			Done=TRUE;

	if(Done==FALSE)
		if(!CreateDirectSound())
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
				nFrames=0;
				QueryPerformanceCounter(&startTime);
			}
		}
	}

	Destroy();
	DestroyDirectSound();

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
	VOID *pbCaptureData=NULL;
	DWORD dwCaptureLength;
	unsigned char *buffer, *left, *right;
	int i, k;

	left=malloc(size);
	right=malloc(size);
	ZeroMemory(left, size);
	ZeroMemory(right, size);

	if(IDirectSoundCaptureBuffer_Lock(lpDSBCapture, 0, size, &pbCaptureData, &dwCaptureLength, NULL, 0, 0L)!=DS_OK)
	{
		MessageBox(hWnd, "Lock failed", "NULL", MB_OK);
		return;
	}

	buffer=pbCaptureData;

	k=0;
	for(i=0;i<size;i+=2)
	{
		left[k]=buffer[i];
		k++;
	}

	k=0;
	for(i=0;i<size;i+=2)
	{
		right[k]=buffer[i+1];
		k++;
	}

	if(IDirectSoundCaptureBuffer_Unlock(lpDSBCapture, pbCaptureData,  dwCaptureLength, NULL, 0)!=DS_OK)
	{
		MessageBox(hWnd, "Unlock failed", "NULL", MB_OK);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, Base);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texwidth, texheight, GL_LUMINANCE, GL_UNSIGNED_BYTE, left);

	glBindTexture(GL_TEXTURE_2D, Base1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texwidth, texheight, GL_LUMINANCE, GL_UNSIGNED_BYTE, right);

	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);

	if(IsFragmentShaderSupported)
	{
		glBindFragmentShaderATI(FragmentShader);
		glEnable(GL_FRAGMENT_SHADER_ATI);
	}

	glPushMatrix();
	glTranslatef(-1.25f, 0.0f, 0.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	if(IsFragmentShaderSupported)
	{
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, TransferMap);
		glEnable(GL_TEXTURE_2D);
	}

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -2.0f);
		glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, -2.0f);
		glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -2.0f);
		glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, -2.0f);
	glEnd();

	glTranslatef(2.5f, 0.0f, 0.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base1);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -2.0f);
		glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, -2.0f);
		glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -2.0f);
		glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, -2.0f);
	glEnd();
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	if(IsFragmentShaderSupported)
		glDisable(GL_FRAGMENT_SHADER_ATI);

	glTranslatef(-2.5f, -0.5f, 0.0f);

	glBegin(GL_LINE_STRIP);
	for(i=0;i<k;i++)
	{
		float end=(float)left[i]/255;
		float start=(float)i/(k/5);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(start, end+1.5f, -2.0f);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for(i=0;i<k;i++)
	{
		float end=(float)right[i]/255;
		float start=(float)i/(k/5);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(start, end-1.5f, -2.0f);
	}
	glEnd();

	glPopMatrix();

	free(left);
	free(right);
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
		glClientActiveTextureARB=(PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
		glMultiTexCoord1dARB=(PFNGLMULTITEXCOORD1DARBPROC)wglGetProcAddress("glMultiTexCoord1dARB");
		glMultiTexCoord1dvARB=(PFNGLMULTITEXCOORD1DVARBPROC)wglGetProcAddress("glMultiTexCoord1dvARB");
		glMultiTexCoord1fARB=(PFNGLMULTITEXCOORD1FARBPROC)wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord1fvARB=(PFNGLMULTITEXCOORD1FVARBPROC)wglGetProcAddress("glMultiTexCoord1fvARB");
		glMultiTexCoord1iARB=(PFNGLMULTITEXCOORD1IARBPROC)wglGetProcAddress("glMultiTexCoord1iARB");
		glMultiTexCoord1ivARB=(PFNGLMULTITEXCOORD1IVARBPROC)wglGetProcAddress("glMultiTexCoord1ivARB");
		glMultiTexCoord1sARB=(PFNGLMULTITEXCOORD1SARBPROC)wglGetProcAddress("glMultiTexCoord1sARB");
		glMultiTexCoord1svARB=(PFNGLMULTITEXCOORD1SVARBPROC)wglGetProcAddress("glMultiTexCoord1svARB");
		glMultiTexCoord2dARB=(PFNGLMULTITEXCOORD2DARBPROC)wglGetProcAddress("glMultiTexCoord2dARB");
		glMultiTexCoord2dvARB=(PFNGLMULTITEXCOORD2DVARBPROC)wglGetProcAddress("glMultiTexCoord2dvARB");
		glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord2fvARB=(PFNGLMULTITEXCOORD2FVARBPROC)wglGetProcAddress("glMultiTexCoord2fvARB");
		glMultiTexCoord2iARB=(PFNGLMULTITEXCOORD2IARBPROC)wglGetProcAddress("glMultiTexCoord2iARB");
		glMultiTexCoord2ivARB=(PFNGLMULTITEXCOORD2IVARBPROC)wglGetProcAddress("glMultiTexCoord2ivARB");
		glMultiTexCoord2sARB=(PFNGLMULTITEXCOORD2SARBPROC)wglGetProcAddress("glMultiTexCoord2sARB");
		glMultiTexCoord2svARB=(PFNGLMULTITEXCOORD2SVARBPROC)wglGetProcAddress("glMultiTexCoord2svARB");
		glMultiTexCoord3dARB=(PFNGLMULTITEXCOORD3DARBPROC)wglGetProcAddress("glMultiTexCoord3dARB");
		glMultiTexCoord3dvARB=(PFNGLMULTITEXCOORD3DVARBPROC)wglGetProcAddress("glMultiTexCoord3dvARB");
		glMultiTexCoord3fARB=(PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord3fvARB=(PFNGLMULTITEXCOORD3FVARBPROC)wglGetProcAddress("glMultiTexCoord3fvARB");
		glMultiTexCoord3iARB=(PFNGLMULTITEXCOORD3IARBPROC)wglGetProcAddress("glMultiTexCoord3iARB");
		glMultiTexCoord3ivARB=(PFNGLMULTITEXCOORD3IVARBPROC)wglGetProcAddress("glMultiTexCoord3ivARB");
		glMultiTexCoord3sARB=(PFNGLMULTITEXCOORD3SARBPROC)wglGetProcAddress("glMultiTexCoord3sARB");
		glMultiTexCoord3svARB=(PFNGLMULTITEXCOORD3SVARBPROC)wglGetProcAddress("glMultiTexCoord3svARB");
		glMultiTexCoord4dARB=(PFNGLMULTITEXCOORD4DARBPROC)wglGetProcAddress("glMultiTexCoord4dARB");
		glMultiTexCoord4dvARB=(PFNGLMULTITEXCOORD4DVARBPROC)wglGetProcAddress("glMultiTexCoord4dvARB");
		glMultiTexCoord4fARB=(PFNGLMULTITEXCOORD4FARBPROC)wglGetProcAddress("glMultiTexCoord4fARB");
		glMultiTexCoord4fvARB=(PFNGLMULTITEXCOORD4FVARBPROC)wglGetProcAddress("glMultiTexCoord4fvARB");
		glMultiTexCoord4iARB=(PFNGLMULTITEXCOORD4IARBPROC)wglGetProcAddress("glMultiTexCoord4iARB");
		glMultiTexCoord4ivARB=(PFNGLMULTITEXCOORD4IVARBPROC)wglGetProcAddress("glMultiTexCoord4ivARB");
		glMultiTexCoord4sARB=(PFNGLMULTITEXCOORD4SARBPROC)wglGetProcAddress("glMultiTexCoord4sARB");
		glMultiTexCoord4svARB=(PFNGLMULTITEXCOORD4SVARBPROC)wglGetProcAddress("glMultiTexCoord4svARB");
	}

	if(!IsExtensionSupported("GL_ATI_fragment_shader"))
	{
		IsFragmentShaderSupported=FALSE;
	}
	else
	{
		glGenFragmentShadersATI=(PFNGLGENFRAGMENTSHADERSATIPROC)wglGetProcAddress("glGenFragmentShadersATI");
		glBindFragmentShaderATI=(PFNGLBINDFRAGMENTSHADERATIPROC)wglGetProcAddress("glBindFragmentShaderATI");
		glDeleteFragmentShaderATI=(PFNGLDELETEFRAGMENTSHADERATIPROC)wglGetProcAddress("glDeleteFragmentShaderATI");
		glBeginFragmentShaderATI=(PFNGLBEGINFRAGMENTSHADERATIPROC)wglGetProcAddress("glBeginFragmentShaderATI");
		glEndFragmentShaderATI=(PFNGLENDFRAGMENTSHADERATIPROC)wglGetProcAddress("glEndFragmentShaderATI");
		glPassTexCoordATI=(PFNGLPASSTEXCOORDATIPROC)wglGetProcAddress("glPassTexCoordATI");
		glSampleMapATI=(PFNGLSAMPLEMAPATIPROC)wglGetProcAddress("glSampleMapATI");
		glColorFragmentOp1ATI=(PFNGLCOLORFRAGMENTOP1ATIPROC)wglGetProcAddress("glColorFragmentOp1ATI");
		glColorFragmentOp2ATI=(PFNGLCOLORFRAGMENTOP2ATIPROC)wglGetProcAddress("glColorFragmentOp2ATI");
		glColorFragmentOp3ATI=(PFNGLCOLORFRAGMENTOP3ATIPROC)wglGetProcAddress("glColorFragmentOp3ATI");
		glAlphaFragmentOp1ATI=(PFNGLALPHAFRAGMENTOP1ATIPROC)wglGetProcAddress("glAlphaFragmentOp1ATI");
		glAlphaFragmentOp2ATI=(PFNGLALPHAFRAGMENTOP2ATIPROC)wglGetProcAddress("glAlphaFragmentOp2ATI");
		glAlphaFragmentOp3ATI=(PFNGLALPHAFRAGMENTOP3ATIPROC)wglGetProcAddress("glAlphaFragmentOp3ATI");
		glSetFragmentShaderConstantATI=(PFNGLSETFRAGMENTSHADERCONSTANTATIPROC)wglGetProcAddress("glSetFragmentShaderConstantATI");
		IsFragmentShaderSupported=TRUE;
	}

	if(!IsExtensionSupported("GL_EXT_texture_edge_clamp"))
	{
		MessageBox(hWnd, "GL_EXT_texture_edge_clamp not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	TransferMap=TGA_UploadImage("transfer0.tga");

	glGenTextures(1, &Base);
	glBindTexture(GL_TEXTURE_2D, Base);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, texwidth, texheight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &Base1);
	glBindTexture(GL_TEXTURE_2D, Base1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, texwidth, texheight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);

	if(IsFragmentShaderSupported)
		FragmentShader=BuildFragmentShader();

	return TRUE;
}

BOOL CreateDirectSound(void)
{
    DSCBUFFERDESC dscbd;

	if(DirectSoundCaptureCreate(NULL, &lpDSCapture, NULL)!=DS_OK)
		return FALSE;

	ZeroMemory(&waveformat, sizeof(waveformat));
	waveformat.wFormatTag=WAVE_FORMAT_PCM;
	waveformat.nSamplesPerSec=44100;
	waveformat.wBitsPerSample=8;
	waveformat.nChannels=2;
	waveformat.nBlockAlign=waveformat.nChannels*(waveformat.wBitsPerSample/8);
	waveformat.nAvgBytesPerSec=waveformat.nBlockAlign*waveformat.nSamplesPerSec;

	SAFE_RELEASE(lpDSBCapture);

	ZeroMemory(&dscbd, sizeof(dscbd));
	dscbd.dwSize=sizeof(dscbd);
	dscbd.dwBufferBytes=size;
	dscbd.lpwfxFormat=&waveformat;

	if(IDirectSoundCapture_CreateCaptureBuffer(lpDSCapture, &dscbd, &lpDSBCapture, NULL))
		return FALSE;

	if(IDirectSoundCaptureBuffer_Start(lpDSBCapture, DSCBSTART_LOOPING)!=DS_OK)
	{
		MessageBox(hWnd, "Start failed", "NULL", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void DestroyDirectSound(void)
{
	if(lpDSBCapture)
		if(IDirectSoundCaptureBuffer_Stop(lpDSBCapture)!=DS_OK)
			MessageBox(hWnd, "Stop failed", "NULL", MB_OK);

	SAFE_RELEASE(lpDSBCapture);
	SAFE_RELEASE(lpDSCapture);
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

unsigned int BuildFragmentShader(void)
{
	unsigned int ShaderID;

	ShaderID=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID);
	glBeginFragmentShaderATI();

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STQ_ATI);

	glColorFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);
	glAlphaFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_1_ATI, GL_NONE,
		GL_REG_0_ATI, GL_ALPHA, GL_NONE);

	glSampleMapATI(GL_REG_1_ATI, GL_REG_1_ATI, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);
	glAlphaFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_0_ATI, GL_NONE,
		GL_REG_1_ATI, GL_ALPHA, GL_NONE);

	glEndFragmentShaderATI();

	return ShaderID;
}