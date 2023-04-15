#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "image.h"
#include "overlay.h"
#include "3ds.h"
#include "quat.h"
#include "camera.h"
#include "resource.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="Demo";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256], Auto=FALSE;

LARGE_INTEGER timer_freq, time_counter, time_current;
float fTimeStep, fTime=0.0f;

//int CubemapWidth=128, CubemapHeight=128; // Lower quality cube map, faster, but doesn't look as good.
int CubemapWidth=256, CubemapHeight=256;

float matrix[16];

float Radius=1/1024.0f;

float s[4]={ 1.0f, 0.0f, 0.0f, 0.0f };
float t[4]={ 0.0f, 1.0f, 0.0f, 0.0f };
float r[4]={ 0.0f, 0.0f, 1.0f, 0.0f };

unsigned int Area3DLogo, Logo, VectorMap, CubeMap, FalloffMap;

TDS_Model_t Model[2];

float light[3], light0[4]={ 0.0f, 0.0f, 0.0f, 1.0f };

Camera_t Camera;
CameraPath_t CameraPath;

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

PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
unsigned int GenVectorMap(void);
unsigned int GenFalloffMap(void);
void CreateDynamicCubeMap(unsigned int TextureID);
void DrawLogo(float x, float y, float Width, float Height, unsigned int TextureID);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	MSG msg;

	wc.style=CS_VREDRAW|CS_HREDRAW|CS_OWNDC;
	wc.lpfnWndProc=WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
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

	ShowCursor(FALSE);

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
		}
	}

	ShowCursor(TRUE);

	TDS_Delete(&Model[0]);
	TDS_Delete(&Model[1]);
	CameraDeletePath(&CameraPath);

	Destroy();

	DestroyWindow(hWnd);

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
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

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_SPACE:
					Auto^=TRUE;
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

void VectorMatrix3x3Multiply(float in[3], float m[16], float out[3])
{
	out[0]=(in[0]*m[0])+(in[1]*m[4])+(in[2]*m[8]);
	out[1]=(in[0]*m[1])+(in[1]*m[5])+(in[2]*m[9]);
	out[2]=(in[0]*m[2])+(in[1]*m[6])+(in[2]*m[10]);
}

void Render(void)
{
	time_current=time_counter;
	QueryPerformanceCounter(&time_counter);

	fTimeStep=(float)(time_counter.QuadPart-time_current.QuadPart)/timer_freq.QuadPart;
	fTime+=fTimeStep;

	CreateDynamicCubeMap(CubeMap);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	if(Auto)
		CameraInterpolatePath(&CameraPath, &Camera, fTimeStep);
	else
		CameraUpdate(&Camera, fTimeStep);

	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX_ARB, matrix);
	VectorMatrix3x3Multiply(light0, matrix, light);
	light[0]+=Camera.Position[0];
	light[1]+=Camera.Position[1];
	light[2]+=Camera.Position[2];

	//Main area

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_DOT3_RGB_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_3D_EXT, FalloffMap);
	glEnable(GL_TEXTURE_3D_EXT);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_R, GL_OBJECT_PLANE, r);
	glEnable(GL_TEXTURE_GEN_R);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
    glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(Radius, Radius, Radius);
	glTranslatef(-light[0], -light[1], -light[2]);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	TDS_Render(&Model[0], light);
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_S);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	//Cube mapped Xabre logo

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
	glEnable(GL_TEXTURE_GEN_R);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMultMatrixf(matrix);
	glMatrixMode(GL_MODELVIEW);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_3D_EXT, FalloffMap);
	glEnable(GL_TEXTURE_3D_EXT);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_R, GL_OBJECT_PLANE, r);
	glEnable(GL_TEXTURE_GEN_R);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
    glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(Radius, Radius, Radius);
	glTranslatef(-light[0], -light[1], -light[2]);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	TDS_Render(&Model[1], NULL);
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_S);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		DrawLogo(0, 0, 256, 128, Logo);
		DrawLogo((float)Width-256 , (float)Height-128, 256, 128, Area3DLogo);
	EndOverlay();
}

BOOL Init(void)
{
	int NumTextureUnits;

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

		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &NumTextureUnits);

		if(NumTextureUnits<4)
		{
			MessageBox(hWnd, "This demo requires at least 4 texturing units.", "OpenGL", MB_OK);
			return FALSE;
		}
	}

	if(!IsExtensionSupported("GL_EXT_texture3D"))
	{
		MessageBox(hWnd, "GL_EXT_texture3D not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
		glTexImage3DEXT=(PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress("glTexImage3DEXT");

	if(!IsExtensionSupported("GL_ARB_texture_env_dot3"))
	{
		MessageBox(hWnd, "GL_ARB_texture_env_dot3 not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_EXT_texture_env_combine"))
	{
		MessageBox(hWnd, "GL_EXT_texture_env_combine not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_ARB_texture_cube_map"))
	{
		MessageBox(hWnd, "GL_ARB_texture_cube_map not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_EXT_texture_edge_clamp"))
	{
		MessageBox(hWnd, "GL_EXT_texture_edge_clamp not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	if(!IsExtensionSupported("GL_ARB_transpose_matrix"))
	{
		MessageBox(hWnd, "GL_ARB_transpose_matrix not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	Logo=IMAGE_UploadImage("logo.tga");
	Area3DLogo=IMAGE_UploadImage("area3dlogo.tga");

	glGenTextures(1, &CubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB8, CubemapWidth, CubemapHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	VectorMap=GenVectorMap();
	FalloffMap=GenFalloffMap();

	TDS_LoadModel(&Model[0], "room.3ds");
	TDS_LoadModel(&Model[1], "xabre.3ds");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CameraInit(&Camera);

	if(!CameraLoadPath("path.txt", &CameraPath))
		return FALSE;

	Camera.Position[0]=CameraPath.Position[0];
	Camera.Position[1]=CameraPath.Position[1];
	Camera.Position[2]=CameraPath.Position[2];

	Camera.View[0]=CameraPath.View[0];
	Camera.View[1]=CameraPath.View[1];
	Camera.View[2]=CameraPath.View[2];

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
	gluPerspective(90.0f, (float)Width/Height, 0.01f, 1024.0f);
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

unsigned int GenVectorMap(void)
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

unsigned int GenFalloffMap(void)
{
	unsigned int TextureID;
	int x, y, z;
	int size=64, halfsize=size/2;
	unsigned char *buffer=malloc(size*size*size);

	for(z=0;z<size;z++)
	{
		float fz=(float)(z-halfsize)/halfsize;

		for(y=0;y<size;y++)
		{
			float fy=(float)(y-halfsize)/halfsize;

			for(x=0;x<size;x++)
			{
				float fx=(float)(x-halfsize)/halfsize;
				float falloff=fx*fx+fy*fy+fz*fz;

				buffer[z*size*size+y*size+x]=(unsigned char)(255.0f*min(1.0f, max(0.0f, 1-falloff)));
			}
		}
	}

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_3D_EXT, TextureID);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R_EXT, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE8, size, size, size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
}

unsigned int CubeMapFaces[6]=
{
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB
};

float CubeMapRotation[6][4]=
{
	{ -90.0f, 0.0f, 1.0f, 0.0f },
	{ 90.0f, 0.0f, 1.0f, 0.0f },
	{ -90.0f, 1.0f, 0.0f, 0.0f },
	{ 90.0f, 1.0f, 0.0f, 0.0f },
	{ 180.0f, 1.0f, 0.0f, 0.0f },
	{ 180.0f, 0.0f, 0.0f, 1.0f },
};

void CreateDynamicCubeMap(unsigned int TextureID)
{
	int i;

	Resize(CubemapWidth, CubemapHeight);

	for(i=0;i<6;i++)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		glRotatef(CubeMapRotation[i][0], CubeMapRotation[i][1], CubeMapRotation[i][2], CubeMapRotation[i][3]);

		if(i==0||i==1)
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_DOT3_RGB_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_PREVIOUS_EXT);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glEnable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_3D_EXT, FalloffMap);
		glEnable(GL_TEXTURE_3D_EXT);

		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_S, GL_OBJECT_PLANE, s);
		glEnable(GL_TEXTURE_GEN_S);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, t);
		glEnable(GL_TEXTURE_GEN_T);
		glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGenfv(GL_R, GL_OBJECT_PLANE, r);
		glEnable(GL_TEXTURE_GEN_R);

		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
	    glTranslatef(0.5f, 0.5f, 0.5f);
		glScalef(Radius, Radius, Radius);
		glTranslatef(-light[0], -light[1], -light[2]);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		TDS_Render(&Model[0], light);
		glPopMatrix();

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glDisable(GL_TEXTURE_3D_EXT);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_S);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glDisable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, TextureID);
		glCopyTexSubImage2D(CubeMapFaces[i], 0, 0, 0, 0, 0, CubemapWidth, CubemapHeight);

		glFlush();
	}

	Resize(Width, Height);
}

void DrawLogo(float x, float y, float Width, float Height, unsigned int TextureID)
{
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x, y+Height);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x, y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x+Width, y+Height);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x+Width, y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
