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
#include "obj.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[255], pause=0;

LARGE_INTEGER timerFrequency, startTime, currentTime;
float fps;
int nFrames=0;

LARGE_INTEGER time_freq, time_counter, time;

LARGE_INTEGER SYNC_startTime, SYNC_currentTime;
float sync;

unsigned int FontBase, FontTexture, Base, Bump, Base_Box, Bump_Box, VectorMap, Falloff;

float light0[3], light1[3], light0color[3]={ 1.0f, 0.5f, 0.5f }, light1color[3]={ 0.5f, 0.5f, 1.0f };
float i;

unsigned int FragmentShader, VertexShader;

unsigned int Tangent, Binormal;

OBJ_Model_t Model;

typedef struct
{
	float camPos[3];
	float camYaw;
	float camPitch;
} camera_t;

camera_t camera;

PFNGLACTIVETEXTUREARBPROC glActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB=NULL;
PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB=NULL;

PFNGLBEGINVERTEXSHADEREXTPROC glBeginVertexShaderEXT=NULL;
PFNGLENDVERTEXSHADEREXTPROC glEndVertexShaderEXT=NULL;
PFNGLBINDVERTEXSHADEREXTPROC glBindVertexShaderEXT=NULL;
PFNGLGENVERTEXSHADERSEXTPROC glGenVertexShadersEXT=NULL;
PFNGLDELETEVERTEXSHADEREXTPROC glDeleteVertexShaderEXT=NULL;
PFNGLSHADEROP1EXTPROC glShaderOp1EXT=NULL;
PFNGLSHADEROP2EXTPROC glShaderOp2EXT=NULL;
PFNGLSHADEROP3EXTPROC glShaderOp3EXT=NULL;
PFNGLSWIZZLEEXTPROC glSwizzleEXT=NULL;
PFNGLWRITEMASKEXTPROC glWriteMaskEXT=NULL;
PFNGLINSERTCOMPONENTEXTPROC glInsertComponentEXT=NULL;
PFNGLEXTRACTCOMPONENTEXTPROC glExtractComponentEXT=NULL;
PFNGLGENSYMBOLSEXTPROC glGenSymbolsEXT=NULL;
PFNGLSETINVARIANTEXTPROC glSetInvariantEXT=NULL;
PFNGLSETLOCALCONSTANTEXTPROC glSetLocalConstantEXT=NULL;
PFNGLVARIANTBVEXTPROC glVariantbvEXT=NULL;
PFNGLVARIANTSVEXTPROC glVariantsvEXT=NULL;
PFNGLVARIANTIVEXTPROC glVariantivEXT=NULL;
PFNGLVARIANTFVEXTPROC glVariantfvEXT=NULL;
PFNGLVARIANTDVEXTPROC glVariantdvEXT=NULL;
PFNGLVARIANTUBVEXTPROC glVariantubvEXT=NULL;
PFNGLVARIANTUSVEXTPROC glVariantusvEXT=NULL;
PFNGLVARIANTUIVEXTPROC glVariantuivEXT=NULL;
PFNGLVARIANTPOINTEREXTPROC glVariantPointerEXT=NULL;
PFNGLENABLEVARIANTCLIENTSTATEEXTPROC glEnableVariantClientStateEXT=NULL;
PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC glDisableVariantClientStateEXT=NULL;
PFNGLBINDLIGHTPARAMETEREXTPROC glBindLightParameterEXT=NULL;
PFNGLBINDMATERIALPARAMETEREXTPROC glBindMaterialParameterEXT=NULL;
PFNGLBINDTEXGENPARAMETEREXTPROC glBindTexGenParameterEXT=NULL;
PFNGLBINDTEXTUREUNITPARAMETEREXTPROC glBindTextureUnitParameterEXT=NULL;
PFNGLBINDPARAMETEREXTPROC glBindParameterEXT=NULL;
PFNGLISVARIANTENABLEDEXTPROC glIsVariantEnabledEXT=NULL;
PFNGLGETVARIANTBOOLEANVEXTPROC glGetVariantBooleanvEXT=NULL;
PFNGLGETVARIANTINTEGERVEXTPROC glGetVariantIntegervEXT=NULL;
PFNGLGETVARIANTFLOATVEXTPROC glGetVariantFloatvEXT=NULL;
PFNGLGETVARIANTPOINTERVEXTPROC glGetVariantPointervEXT=NULL;
PFNGLGETINVARIANTBOOLEANVEXTPROC glGetInvariantBooleanvEXT=NULL;
PFNGLGETINVARIANTINTEGERVEXTPROC glGetInvariantIntegervEXT=NULL;
PFNGLGETINVARIANTFLOATVEXTPROC glGetInvariantFloatvEXT=NULL;
PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC glGetLocalConstantBooleanvEXT=NULL;
PFNGLGETLOCALCONSTANTINTEGERVEXTPROC glGetLocalConstantIntegervEXT=NULL;
PFNGLGETLOCALCONSTANTFLOATVEXTPROC glGetLocalConstantFloatvEXT=NULL;

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
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
unsigned int GenFalloffMap(void);
unsigned int GenVectorMap(void);
unsigned int BuildVertexShader(void);
unsigned int BuildFragmentShader(void);

void clampCamera(camera_t *cam)
{
	if(cam->camPitch>90.0f)
		cam->camPitch=90.0f;
	else
		if(cam->camPitch<-90.0f)
			cam->camPitch=-90.0f;

	while(cam->camYaw<0.0f)
		cam->camYaw+=360.0f;

	while(cam->camYaw>=360.0f)
		cam->camYaw-=360.0f;
}

void dollyCamera(camera_t *cam, float dollyBy, float dollyAngle)
{
	float Angle=((cam->camYaw+dollyAngle)*3.1415f/180.0f);

	cam->camPos[0]-=(float)sin(Angle)*dollyBy;
	cam->camPos[2]-=(float)cos(Angle)*dollyBy;
}

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
			QueryPerformanceCounter(&SYNC_currentTime);
			sync=((float)(SYNC_currentTime.QuadPart-SYNC_startTime.QuadPart)/(float)timerFrequency.QuadPart)*150;
			QueryPerformanceCounter(&SYNC_startTime);

			if(Key['W'])
				dollyCamera(&camera, sync, 0.0f);

			if(Key['S'])
				dollyCamera(&camera, sync, 180.0f);

			if(Key[VK_LEFT])
				camera.camYaw+=sync;

			if(Key[VK_RIGHT])
				camera.camYaw-=sync;

			if(Key['A'])
				dollyCamera(&camera, sync, 90.0f);

			if(Key['D'])
				dollyCamera(&camera, sync, 270.0);

			if(Key[VK_PRIOR])
				camera.camPitch+=sync;

			if(Key[VK_NEXT])
				camera.camPitch-=sync;

			if(Key[VK_END])
				camera.camPitch=0.0f;

			if(Key['V'])
				camera.camPos[1]+=sync;

			if(Key['C'])
				camera.camPos[1]-=sync;

			clampCamera(&camera);

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

				case VK_SPACE:
					pause^=1;
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

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	if(!pause)
	{
		i+=(float)(time_counter.QuadPart-time.QuadPart)/time_freq.QuadPart;

		light0[0]=(float)(sin(i)*(cos(i*2)*32))*6.0f;
		light0[1]=(float)(cos(i)*(cos(i*2)*32))+2.0f;
		light0[2]=(float)(cos(i)*(sin(i*2)*32))*6.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, light0);

		light1[0]=(float)(cos(i)*(sin(i*2)*32))*6.0f;
		light1[1]=(float)(cos(i)*(cos(i*2)*32))+2.0f;
		light1[2]=(float)(sin(i)*(sin(i*2)*32))*6.0f;
		glLightfv(GL_LIGHT1, GL_POSITION, light1);
	}

	glPushMatrix();
	glRotatef(-camera.camPitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-camera.camYaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camera.camPos[0], -camera.camPos[1], -camera.camPos[2]);

	glBegin(GL_POINTS);
	glColor3fv(light0color);
	glVertex3fv(light0);
	glColor3fv(light1color);
	glVertex3fv(light1);
	glEnd();

	glBindFragmentShaderATI(FragmentShader);
	glEnable(GL_FRAGMENT_SHADER_ATI);
	glSetFragmentShaderConstantATI(GL_CON_0_ATI, light0color);
	glSetFragmentShaderConstantATI(GL_CON_1_ATI, light1color);

	glBindVertexShaderEXT(VertexShader);
	glEnable(GL_VERTEX_SHADER_EXT);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_2D, Falloff);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE5_ARB);
	glBindTexture(GL_TEXTURE_2D, Falloff);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		OBJ_Draw(&Model);
	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE5_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_VERTEX_SHADER_EXT);

	glDisable(GL_FRAGMENT_SHADER_ATI);

//	glPushMatrix();
//		OBJ_DrawVectors(&Model);
//	glPopMatrix();

	glPopMatrix();

	glCullFace(GL_BACK);

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 16, FontBase, FontTexture, "%s", gluErrorString(glGetError()));
		Font_Print(0, 0, FontBase, FontTexture, "FPS: %0.1f", fps);
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
		glMultiTexCoord3fARB=(PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3fARB");
	}

	if(!IsExtensionSupported("GL_EXT_vertex_shader"))
	{
		MessageBox(hWnd, "GL_EXT_vertex_shader not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
	{
		glBeginVertexShaderEXT=(PFNGLBEGINVERTEXSHADEREXTPROC)wglGetProcAddress("glBeginVertexShaderEXT");
		glEndVertexShaderEXT=(PFNGLENDVERTEXSHADEREXTPROC)wglGetProcAddress("glEndVertexShaderEXT");
		glBindVertexShaderEXT=(PFNGLBINDVERTEXSHADEREXTPROC)wglGetProcAddress("glBindVertexShaderEXT");
		glGenVertexShadersEXT=(PFNGLGENVERTEXSHADERSEXTPROC)wglGetProcAddress("glGenVertexShadersEXT");
		glDeleteVertexShaderEXT=(PFNGLDELETEVERTEXSHADEREXTPROC)wglGetProcAddress("glDeleteVertexShaderEXT");
		glShaderOp1EXT=(PFNGLSHADEROP1EXTPROC)wglGetProcAddress("glShaderOp1EXT");
		glShaderOp2EXT=(PFNGLSHADEROP2EXTPROC)wglGetProcAddress("glShaderOp2EXT");
		glShaderOp3EXT=(PFNGLSHADEROP3EXTPROC)wglGetProcAddress("glShaderOp3EXT");
		glSwizzleEXT=(PFNGLSWIZZLEEXTPROC)wglGetProcAddress("glSwizzleEXT");
		glWriteMaskEXT=(PFNGLWRITEMASKEXTPROC)wglGetProcAddress("glWriteMaskEXT");
		glInsertComponentEXT=(PFNGLINSERTCOMPONENTEXTPROC)wglGetProcAddress("glInsertComponentEXT");
		glExtractComponentEXT=(PFNGLEXTRACTCOMPONENTEXTPROC)wglGetProcAddress("glExtractComponentEXT");
		glGenSymbolsEXT=(PFNGLGENSYMBOLSEXTPROC)wglGetProcAddress("glGenSymbolsEXT");
		glSetInvariantEXT=(PFNGLSETINVARIANTEXTPROC)wglGetProcAddress("glSetInvariantEXT");
		glSetLocalConstantEXT=(PFNGLSETLOCALCONSTANTEXTPROC)wglGetProcAddress("glSetLocalConstantEXT");
		glVariantbvEXT=(PFNGLVARIANTBVEXTPROC)wglGetProcAddress("glVariantbvEXT");
		glVariantsvEXT=(PFNGLVARIANTSVEXTPROC)wglGetProcAddress("glVariantsvEXT");
		glVariantivEXT=(PFNGLVARIANTIVEXTPROC)wglGetProcAddress("glVariantivEXT");
		glVariantfvEXT=(PFNGLVARIANTFVEXTPROC)wglGetProcAddress("glVariantfvEXT");
		glVariantdvEXT=(PFNGLVARIANTDVEXTPROC)wglGetProcAddress("glVariantdvEXT");
		glVariantubvEXT=(PFNGLVARIANTUBVEXTPROC)wglGetProcAddress("glVariantubvEXT");
		glVariantusvEXT=(PFNGLVARIANTUSVEXTPROC)wglGetProcAddress("glVariantusvEXT");
		glVariantuivEXT=(PFNGLVARIANTUIVEXTPROC)wglGetProcAddress("glVariantuivEXT");
		glVariantPointerEXT=(PFNGLVARIANTPOINTEREXTPROC)wglGetProcAddress("glVariantPointerEXT");
		glEnableVariantClientStateEXT=(PFNGLENABLEVARIANTCLIENTSTATEEXTPROC)wglGetProcAddress("glEnableVariantClientStateEXT");
		glDisableVariantClientStateEXT=(PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC)wglGetProcAddress("glDisableVariantClientStateEXT");
		glBindLightParameterEXT=(PFNGLBINDLIGHTPARAMETEREXTPROC)wglGetProcAddress("glBindLightParameterEXT");
		glBindMaterialParameterEXT=(PFNGLBINDMATERIALPARAMETEREXTPROC)wglGetProcAddress("glBindMaterialParameterEXT");
		glBindTexGenParameterEXT=(PFNGLBINDTEXGENPARAMETEREXTPROC)wglGetProcAddress("glBindTexGenParameterEXT");
		glBindTextureUnitParameterEXT=(PFNGLBINDTEXTUREUNITPARAMETEREXTPROC)wglGetProcAddress("glBindTextureUnitParameterEXT");
		glBindParameterEXT=(PFNGLBINDPARAMETEREXTPROC)wglGetProcAddress("glBindParameterEXT");
		glIsVariantEnabledEXT=(PFNGLISVARIANTENABLEDEXTPROC)wglGetProcAddress("glIsVariantEnabledEXT");
		glGetVariantBooleanvEXT=(PFNGLGETVARIANTBOOLEANVEXTPROC)wglGetProcAddress("glGetVariantBooleanvEXT");
		glGetVariantIntegervEXT=(PFNGLGETVARIANTINTEGERVEXTPROC)wglGetProcAddress("glGetVariantIntegervEXT");
		glGetVariantFloatvEXT=(PFNGLGETVARIANTFLOATVEXTPROC)wglGetProcAddress("glGetVariantFloatvEXT");
		glGetVariantPointervEXT=(PFNGLGETVARIANTPOINTERVEXTPROC)wglGetProcAddress("glGetVariantPointervEXT");
		glGetInvariantBooleanvEXT=(PFNGLGETINVARIANTBOOLEANVEXTPROC)wglGetProcAddress("glGetInvariantBooleanvEXT");
		glGetInvariantIntegervEXT=(PFNGLGETINVARIANTINTEGERVEXTPROC)wglGetProcAddress("glGetInvariantIntegervEXT");
		glGetInvariantFloatvEXT=(PFNGLGETINVARIANTFLOATVEXTPROC)wglGetProcAddress("glGetInvariantFloatvEXT");
		glGetLocalConstantBooleanvEXT=(PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC)wglGetProcAddress("glGetLocalConstantBooleanvEXT");
		glGetLocalConstantIntegervEXT=(PFNGLGETLOCALCONSTANTINTEGERVEXTPROC)wglGetProcAddress("glGetLocalConstantIntegervEXT");
		glGetLocalConstantFloatvEXT=(PFNGLGETLOCALCONSTANTFLOATVEXTPROC)wglGetProcAddress("glGetLocalConstantFloatvEXT");
	}

	if(!IsExtensionSupported("GL_ATI_fragment_shader"))
	{
		MessageBox(hWnd, "GL_ATI_fragment_shader not found!", "OpenGL", MB_OK);
		return FALSE;
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
	}

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	Base=TGA_UploadImage("base.tga");
	Bump=TGA_UploadDOT3("bump.tga");

	VectorMap=GenVectorMap();
	Falloff=GenFalloffMap();

	OBJ_LoadModel(&Model, "room.obj");

	VertexShader=BuildVertexShader();
	FragmentShader=BuildFragmentShader();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glPointSize(16.0f);

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
	gluPerspective(90.0f, (float)Width/Height, 0.1f, 4096.0f);
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

float Function(float in)
{
	float d2=in*16.0f+0.25f;

	if(d2==0)
		return 0.0f;

	return min(1.0f, max(0.0f, (1.0f/d2)/2.0f));
}

unsigned int GenFalloffMap(void)
{
	unsigned int TextureID;
	int size=256, i;
	unsigned char *buffer;

	buffer=malloc(size);

	for(i=0;i<size;i++)
		buffer[(0*size+i)]=(unsigned char)(255.0f*Function((float)i/size));

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, size, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
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

unsigned int BuildVertexShader(void)
{
	unsigned int ShaderID;

	unsigned int ModelView, Projection;
	unsigned int EyeVertex, Vertex, Normal, Binormal, TexCoord, Color, Light0, Light1;
	unsigned int Mag, r0, r1, r2, r3, r4, c0;
	float c0f=1.0f/384.0f;

	ModelView=glBindParameterEXT(GL_MODELVIEW_MATRIX);
	Projection=glBindParameterEXT(GL_PROJECTION_MATRIX);
	Vertex=glBindParameterEXT(GL_CURRENT_VERTEX_EXT);
	Normal=glBindParameterEXT(GL_CURRENT_NORMAL);
	Color=glBindParameterEXT(GL_CURRENT_COLOR);
	Light0=glBindLightParameterEXT(GL_LIGHT0, GL_POSITION);
	Light1=glBindLightParameterEXT(GL_LIGHT1, GL_POSITION);
	TexCoord=glBindTextureUnitParameterEXT(GL_TEXTURE0_ARB, GL_CURRENT_TEXTURE_COORDS);

	ShaderID=glGenVertexShadersEXT(1);
	glBindVertexShaderEXT(ShaderID);
	glBeginVertexShaderEXT();

	c0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_CONSTANT_EXT, GL_FULL_RANGE_EXT, 1);
	glSetLocalConstantEXT(c0, GL_FLOAT, &c0f);

	EyeVertex=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Binormal=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r0=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r1=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r2=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r3=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r4=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Mag=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);

	Tangent=glGenSymbolsEXT(GL_VECTOR_EXT, GL_VARIANT_EXT, GL_FULL_RANGE_EXT, 1);

	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, EyeVertex, ModelView, Vertex);
	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, GL_OUTPUT_VERTEX_EXT, Projection, EyeVertex);

	glShaderOp2EXT(GL_OP_CROSS_PRODUCT_EXT, Binormal, Tangent, Normal);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, Binormal, Binormal);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, Binormal, Binormal, Mag);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_COLOR0_EXT, Color);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD0_EXT, TexCoord);

	glShaderOp2EXT(GL_OP_SUB_EXT, r0, Light0, Vertex);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, r0, r0);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, r0, r0, Mag);

	glShaderOp2EXT(GL_OP_DOT3_EXT, r1, Tangent, r0);
	glShaderOp2EXT(GL_OP_DOT3_EXT, r2, Binormal, r0);
	glShaderOp2EXT(GL_OP_DOT3_EXT, r3, Normal, r0);

	glInsertComponentEXT(r4, r1, 0);
	glInsertComponentEXT(r4, r2, 1);
	glInsertComponentEXT(r4, r3, 2);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD1_EXT, r4);

	glShaderOp2EXT(GL_OP_SUB_EXT, r0, Light0, Vertex);
	glShaderOp2EXT(GL_OP_MUL_EXT, GL_OUTPUT_TEXTURE_COORD2_EXT, r0, c0);

	glShaderOp2EXT(GL_OP_SUB_EXT, r0, Light1, Vertex);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, r0, r0);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, r0, r0, Mag);

	glShaderOp2EXT(GL_OP_DOT3_EXT, r1, Tangent, r0);
	glShaderOp2EXT(GL_OP_DOT3_EXT, r2, Binormal, r0);
	glShaderOp2EXT(GL_OP_DOT3_EXT, r3, Normal, r0);

	glInsertComponentEXT(r4, r1, 0);
	glInsertComponentEXT(r4, r2, 1);
	glInsertComponentEXT(r4, r3, 2);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD3_EXT, r4);

	glShaderOp2EXT(GL_OP_SUB_EXT, r0, Light1, Vertex);
	glShaderOp2EXT(GL_OP_MUL_EXT, GL_OUTPUT_TEXTURE_COORD4_EXT, r0, c0);

	glEndVertexShaderEXT();

	return ShaderID;
}

unsigned int BuildFragmentShader(void)
{
	unsigned int ShaderID;
	float c0[4]={ 0.0f, 0.0f, 0.0f, 1.0f };

	ShaderID=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID);
	glBeginFragmentShaderATI();

	glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STQ_ATI);
	glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_3_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_4_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_5_ATI, GL_TEXTURE4_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_2_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI,
		GL_REG_2_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_4_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI,
		GL_REG_4_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_CON_1_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_5_ATI, GL_NONE, GL_NONE,
		GL_REG_5_ATI, GL_NONE, GL_NONE,
		GL_REG_5_ATI, GL_NONE, GL_NONE);

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STQ_ATI);
	glPassTexCoordATI(GL_REG_2_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_3_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_4_ATI, GL_REG_4_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_5_ATI, GL_REG_5_ATI, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_5_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_ADD_ATI,
		GL_REG_1_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glEndFragmentShaderATI();

	return ShaderID;
}