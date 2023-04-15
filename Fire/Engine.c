#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdlib.h>
#include "glati.h"
#include "image.h"
#include "overlay.h"
#include "font.h"
#include "quat.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256];

LARGE_INTEGER timerFrequency, startTime, currentTime;
float fps;
int nFrames=0;

typedef struct
{
	POINT pt;
	int x, y;
	int oldx, oldy;
	int dx, dy;
} mouse_t;

mouse_t mouse;

float RotateX, RotateY, PanX, PanY, Zoom;
float quat[4], matrix[16];

unsigned int FontTexture, Base, Transfer;
unsigned int FragmentShader[2], VertexShader[1];
unsigned int c0, c1, c2, c3, c4;

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
void BuildVertexShader(unsigned int *ShaderID);
void BuildFragmentShader(unsigned int *ShaderID);

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
				fps=(float)nFrames/((float)(currentTime.QuadPart - startTime.QuadPart)/(float)timerFrequency.QuadPart);
				nFrames=0;
				QueryPerformanceCounter(&startTime);
			}
		}
	}

	glDeleteTextures(1, &Base);
	glDeleteTextures(1, &Transfer);
	glDeleteFragmentShaderATI(FragmentShader[0]);
	glDeleteVertexShaderEXT(VertexShader[0]);

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

			if(wParam==MK_MBUTTON)
			{
				PanX+=mouse.dx;
				PanY+=mouse.dy;
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

void matrixInvert(float in[16], float out[16])
{
	out[0]=in[0];
	out[1]=in[4];
	out[2]=in[8];
	out[3]=0.0f;
	out[4]=in[1];
	out[5]=in[5];
	out[6]=in[9];
	out[7]=0.0f;
	out[8]=in[2];
	out[9]=in[6];
	out[10]=in[10];
	out[11]=0.0f;
	out[12]=-(in[12]*in[0])-(in[13]*in[1])-(in[14]*in[2]);
	out[13]=-(in[12]*in[4])-(in[13]*in[5])-(in[14]*in[6]);
	out[14]=-(in[12]*in[8])-(in[13]*in[9])-(in[14]*in[10]);
	out[15]=1.0f;
}

void Quad(void);

void Render(void)
{
	float QuatX[4], QuatY[4], i;
	float TexelS=1.0f/(float)Width;
	float TexelT=1.0f/(float)Height;
	float offset[5][2]=
	{
		{ 0.0f, -TexelT },
		{ -TexelS, -TexelT },
		{ 0.0f, +TexelT },
		{ +TexelS, -TexelT },
		{ 0.0f, -TexelT/2 }
	};

	glSetInvariantEXT(c0, GL_FLOAT, offset[0]);
	glSetInvariantEXT(c1, GL_FLOAT, offset[1]);
	glSetInvariantEXT(c2, GL_FLOAT, offset[2]);
	glSetInvariantEXT(c3, GL_FLOAT, offset[3]);
	glSetInvariantEXT(c4, GL_FLOAT, offset[4]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(PanX, PanY, Zoom-100.0f);
	QuatAngle(QuatX, RotateX, 0.0f, 1.0f, 0.0f);
	QuatAngle(QuatY, RotateY, 1.0f, 0.0f, 0.0f);
	QuatMultiply(QuatX, QuatY, quat);
	QuatMatrix(quat, matrix);
	glMultMatrixf(matrix);

	glBegin(GL_POINTS);
	for(i=-50.0f;i<50.0f;i+=0.25f)
	{
		float r=(float)(rand()%255)/255.0f;

		if(r>0.75f)
		{
			glColor3f(r, r, r);
			glVertex3f(i, 0.0f, 0.0f);
		}
	}
	glEnd();

	glPopMatrix();

	glBindVertexShaderEXT(VertexShader[0]);
	glEnable(GL_VERTEX_SHADER_EXT);

	glBindFragmentShaderATI(FragmentShader[0]);
	glEnable(GL_FRAGMENT_SHADER_ATI);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);

	Quad();

	glDisable(GL_BLEND);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_FRAGMENT_SHADER_ATI);

	glDisable(GL_VERTEX_SHADER_EXT);

	glBindTexture(GL_TEXTURE_2D, Base);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindFragmentShaderATI(FragmentShader[1]);
	glEnable(GL_FRAGMENT_SHADER_ATI);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Transfer);
	glEnable(GL_TEXTURE_2D);

	Quad();

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_FRAGMENT_SHADER_ATI);

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Font_Print(0, 16, FontTexture, "%s\nFPS: %0.1f", gluErrorString(glGetError()), fps);
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

	FontTexture=IMAGE_UploadImage("font.tga");

	Transfer=IMAGE_UploadImage("transfer.tga");

	BuildVertexShader(VertexShader);
	BuildFragmentShader(FragmentShader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPointSize(3.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glGenTextures(1, &Base);
	glBindTexture(GL_TEXTURE_2D, Base);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

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

void BuildVertexShader(unsigned int *ShaderID)
{
	unsigned int MVP, ModelView;
	unsigned int Vertex, TexCoord;

	MVP=glBindParameterEXT(GL_MVP_MATRIX_EXT);
	ModelView=glBindParameterEXT(GL_MODELVIEW_MATRIX);
	Vertex=glBindParameterEXT(GL_CURRENT_VERTEX_EXT);
	TexCoord=glBindTextureUnitParameterEXT(GL_TEXTURE0_ARB, GL_CURRENT_TEXTURE_COORDS);

	ShaderID[0]=glGenVertexShadersEXT(1);
	glBindVertexShaderEXT(ShaderID[0]);
	glBeginVertexShaderEXT();

	c0=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c1=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c2=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c3=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c4=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);

	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, GL_OUTPUT_VERTEX_EXT, MVP, Vertex);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_COLOR0_EXT, GL_ONE_EXT);

	glShaderOp2EXT(GL_OP_ADD_EXT, GL_OUTPUT_TEXTURE_COORD0_EXT, TexCoord, c0);
	glShaderOp2EXT(GL_OP_ADD_EXT, GL_OUTPUT_TEXTURE_COORD1_EXT, TexCoord, c1);
	glShaderOp2EXT(GL_OP_ADD_EXT, GL_OUTPUT_TEXTURE_COORD2_EXT, TexCoord, c2);
	glShaderOp2EXT(GL_OP_ADD_EXT, GL_OUTPUT_TEXTURE_COORD3_EXT, TexCoord, c3);
	glShaderOp2EXT(GL_OP_ADD_EXT, GL_OUTPUT_TEXTURE_COORD4_EXT, TexCoord, c4);

	glEndVertexShaderEXT();
}

void BuildFragmentShader(unsigned int *ShaderID)
{
	ShaderID[0]=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID[0]);
	glBeginFragmentShaderATI();

	{
		float c0[4]={ 0.244f, 0.0f, 0.0f, 0.0f };

		glSetFragmentShaderConstantATI(GL_CON_0_ATI, c0);
	}

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_3_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_4_ATI, GL_TEXTURE4_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_RED, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_RED, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_RED, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_RED, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glEndFragmentShaderATI();

	ShaderID[1]=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID[1]);
	glBeginFragmentShaderATI();

	{
		float c0[4]={ 0.3f, 0.59f, 0.11f, 1.0f };

		glSetFragmentShaderConstantATI(GL_CON_0_ATI, c0);
	}

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glSampleMapATI(GL_REG_1_ATI, GL_REG_0_ATI, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glEndFragmentShaderATI();
}

void Quad(void)
{
	float s=(float)Width/2048.0f;
	float t=(float)Height/2048.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 2.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, t);
			glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(s, t);
			glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(s, 0.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (float)Width/Height, 0.01f, 4096.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
