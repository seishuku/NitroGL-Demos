#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <commdlg.h>
#include "glati.h"
#include "overlay.h"
#include "font.h"
#include "3ds.h"
#include "camera.h"
#include "image.h"
#include "quat.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256], ModelLoaded=FALSE;

LARGE_INTEGER timerFrequency, startTime, currentTime;
float fps;
int nFrames=0;

LARGE_INTEGER time_freq, time_counter, time_current;
float time;

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

Camera_t Camera;

unsigned int FontTexture, Base[1], Bump[1], CubeMap, DiffuseCubeMap;
unsigned int FragmentShader[1], VertexShader[1];
unsigned int Eye, c0;

TDS_Model_t Model[1];

float BumpScale=0.5f;

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

	TDS_Delete(&Model[0]);
	glDeleteTextures(1, Base);
	glDeleteTextures(1, Bump);
	glDeleteTextures(1, &CubeMap);
	glDeleteTextures(1, &DiffuseCubeMap);
	glDeleteFragmentShaderATI(FragmentShader[0]);
	glDeleteVertexShaderEXT(VertexShader[0]);

	Destroy();

	DestroyWindow(hWnd);

	return msg.wParam;
}

void OpenTexture(void)
{
	OPENFILENAME ofn;
	char filename[255+MAX_PATH];

	filename[0]='\0';

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=hWnd;
	ofn.lpstrFile=filename;
	ofn.nMaxFile=MAX_PATH;;
	ofn.lpstrFilter="Images (*.tga;*.jpg)\0*.tga;*.jpg\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.lpstrFileTitle=NULL;
	ofn.lpstrTitle="Open Base Texture";
	ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		glDeleteTextures(1, &Base[0]);
		Base[0]=IMAGE_UploadImage(filename);
	}

	ofn.lpstrTitle="Open Bump Texture";

	if(GetOpenFileName(&ofn))
	{
		glDeleteTextures(1, &Bump[0]);
		Bump[0]=IMAGE_UploadDOT3(filename);
	}
}

void OpenModel(void)
{
	OPENFILENAME ofn;
	char filename[255+MAX_PATH];

	filename[0]='\0';

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=hWnd;
	ofn.lpstrFile=filename;
	ofn.nMaxFile=MAX_PATH;;
	ofn.lpstrFilter="3D Studio Models (*.3ds)\0*.3ds\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.lpstrFileTitle="Open Model";
	ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		TDS_Delete(&Model[0]);
		TDS_LoadModel(&Model[0], filename);
	}
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

				case 'T':
					OpenTexture();
					break;

				case 'M':
					OpenModel();
					break;

				case VK_ADD:
					BumpScale+=0.01f;

					if(BumpScale>=2.0f)
						BumpScale=2.0f;
					break;

				case VK_SUBTRACT:
					BumpScale-=0.01f;

					if(BumpScale<=0.0f)
						BumpScale=0.0f;
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

void MatrixInvert(float matrix[16])
{
	float res[16];

	res[0]=matrix[0];
	res[1]=matrix[4];
	res[2]=matrix[8];
	res[3]=0.0f;
	res[4]=matrix[1];
	res[5]=matrix[5];
	res[6]=matrix[9];
	res[7]=0.0f;
	res[8]=matrix[2];
	res[9]=matrix[6];
	res[10]=matrix[10];
	res[11]=0.0f;
	res[12]=-(matrix[12]*matrix[0])-(matrix[13]*matrix[1])-(matrix[14]*matrix[2]);
	res[13]=-(matrix[12]*matrix[4])-(matrix[13]*matrix[5])-(matrix[14]*matrix[6]);
	res[14]=-(matrix[12]*matrix[8])-(matrix[13]*matrix[9])-(matrix[14]*matrix[10]);
	res[15]=1.0f;

	matrix[0]=res[0];
	matrix[1]=res[1];
	matrix[2]=res[2];
	matrix[3]=res[3];
	matrix[4]=res[4];
	matrix[5]=res[5];
	matrix[6]=res[6];
	matrix[7]=res[7];
	matrix[8]=res[8];
	matrix[9]=res[9];
	matrix[10]=res[10];
	matrix[11]=res[11];
	matrix[12]=res[12];
	matrix[13]=res[13];
	matrix[14]=res[14];
	matrix[15]=res[15];
}

void MatrixVecMultiply(float matrix[16], float vec[4])
{
	float res[4];

	res[0]=matrix[0]*vec[0]+matrix[4]*vec[1]+matrix[8]*vec[2]+matrix[12]*vec[3];
	res[1]=matrix[1]*vec[0]+matrix[5]*vec[1]+matrix[9]*vec[2]+matrix[13]*vec[3];
	res[2]=matrix[2]*vec[0]+matrix[6]*vec[1]+matrix[10]*vec[2]+matrix[14]*vec[3];
	res[3]=matrix[3]*vec[0]+matrix[7]*vec[1]+matrix[11]*vec[2]+matrix[15]*vec[3];

	vec[0]=res[0];
	vec[1]=res[1];
	vec[2]=res[2];
	vec[3]=res[3];
}

void DrawSkyBox(void)
{
	float size=1024.0f;

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glEnd();
}

void Render(void)
{
	float ModelViewMatrix[16], EyeVec[4]={ 0.0f, 0.0f, 0.0f, 1.0f };
	float QuatX[4], QuatY[4];

	time_current=time_counter;
	QueryPerformanceFrequency(&time_freq);
	QueryPerformanceCounter(&time_counter);
	time=(float)(time_counter.QuadPart-time_current.QuadPart)/time_freq.QuadPart;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
//	CameraUpdate(&Camera, time);
	glTranslatef(0.0f, 0.0f, Zoom-250.0f);
	QuatAngle(QuatX, RotateX, 0.0f, 1.0f, 0.0f);
	QuatAngle(QuatY, RotateY, 1.0f, 0.0f, 0.0f);
	QuatMultiply(QuatX, QuatY, quat);
	QuatMatrix(quat, matrix);
	glMultMatrixf(matrix);

	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrix);
	MatrixInvert(ModelViewMatrix);
	MatrixVecMultiply(ModelViewMatrix, EyeVec);
	glSetInvariantEXT(Eye, GL_FLOAT, EyeVec);

	glSetInvariantEXT(c0, GL_FLOAT, &BumpScale);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glPushMatrix();
	DrawSkyBox();
	glPopMatrix();
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glBindVertexShaderEXT(VertexShader[0]);
	glEnable(GL_VERTEX_SHADER_EXT);

	glBindFragmentShaderATI(FragmentShader[0]);
	glEnable(GL_FRAGMENT_SHADER_ATI);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump[0]);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, DiffuseCubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glBindTexture(GL_TEXTURE_2D, Base[0]);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();

	if(ModelLoaded)
		TDS_Render(&Model[0]);

	glPopMatrix();

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glDisable(GL_FRAGMENT_SHADER_ATI);

	glDisable(GL_VERTEX_SHADER_EXT);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Font_Print(0, 32, FontTexture, "Bump scale: %0.2f\n%s\nFPS: %0.1f", BumpScale, gluErrorString(glGetError()), fps);
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

	Base[0]=IMAGE_UploadImage("nitroglbase.tga");
	Bump[0]=IMAGE_UploadDOT3("nitroglbump.tga");

	CubeMap=IMAGE_UploadCubeVirtCross("gracecubemap.jpg");
	DiffuseCubeMap=IMAGE_UploadCubeVirtCross("gracediffusecubemap.tga");

	if(TDS_LoadModel(&Model[0], "sphere.3ds"))
		ModelLoaded=TRUE;

	BuildVertexShader(VertexShader);
	BuildFragmentShader(FragmentShader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CameraInit(&Camera);

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

void BuildVertexShader(unsigned int *ShaderID)
{
	unsigned int ModelView, Projection;
	unsigned int Vertex, Tangent, Binormal, Normal, TexCoord, Color, Light;
	unsigned int EyePos, EyeVertex, Mag, r0, r1, r2, r3, T, B;

	ModelView=glBindParameterEXT(GL_MODELVIEW_MATRIX);
	Projection=glBindParameterEXT(GL_PROJECTION_MATRIX);
	Vertex=glBindParameterEXT(GL_CURRENT_VERTEX_EXT);
	Normal=glBindParameterEXT(GL_CURRENT_NORMAL);
	Color=glBindParameterEXT(GL_CURRENT_COLOR);
	Light=glBindLightParameterEXT(GL_LIGHT0, GL_POSITION);
	TexCoord=glBindTextureUnitParameterEXT(GL_TEXTURE0_ARB, GL_CURRENT_TEXTURE_COORDS);
	Tangent=glBindTextureUnitParameterEXT(GL_TEXTURE1_ARB, GL_CURRENT_TEXTURE_COORDS);
	Binormal=glBindTextureUnitParameterEXT(GL_TEXTURE2_ARB, GL_CURRENT_TEXTURE_COORDS);

	ShaderID[0]=glGenVertexShadersEXT(1);
	glBindVertexShaderEXT(ShaderID[0]);
	glBeginVertexShaderEXT();

	EyeVertex=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Mag=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	EyePos=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r1=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r2=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r3=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	T=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	B=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);

	Eye=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);

	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, EyeVertex, ModelView, Vertex);
	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, GL_OUTPUT_VERTEX_EXT, Projection, EyeVertex);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_COLOR0_EXT, Color);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD0_EXT, TexCoord);

	glShaderOp2EXT(GL_OP_MUL_EXT, T, Tangent, c0);
	glShaderOp2EXT(GL_OP_MUL_EXT, B, Binormal, c0);

	glExtractComponentEXT(r0, T, 0);
	glExtractComponentEXT(r1, B, 0);
	glExtractComponentEXT(r2, Normal, 0);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD1_EXT, r0, 0);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD1_EXT, r1, 1);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD1_EXT, r2, 2);

	glExtractComponentEXT(r0, T, 1);
	glExtractComponentEXT(r1, B, 1);
	glExtractComponentEXT(r2, Normal, 1);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD2_EXT, r0, 0);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD2_EXT, r1, 1);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD2_EXT, r2, 2);

	glExtractComponentEXT(r0, T, 2);
	glExtractComponentEXT(r1, B, 2);
	glExtractComponentEXT(r2, Normal, 2);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD3_EXT, r0, 0);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD3_EXT, r1, 1);
	glInsertComponentEXT(GL_OUTPUT_TEXTURE_COORD3_EXT, r2, 2);

	glShaderOp2EXT(GL_OP_SUB_EXT, EyePos, Eye, Vertex);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, EyePos, EyePos);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, GL_OUTPUT_TEXTURE_COORD4_EXT, EyePos, Mag);

	glEndVertexShaderEXT();
}

void BuildFragmentShader(unsigned int *ShaderID)
{
	ShaderID[0]=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID[0]);
	glBeginFragmentShaderATI();

	{
		float c0[4]={ 0.9f, 0.0f, 0.0f, 0.0f };
		glSetFragmentShaderConstantATI(GL_CON_0_ATI, c0);
	}

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_4_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_3_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_1_ATI, GL_TEXTURE4_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_4_ATI, GL_RED_BIT_ATI, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_4_ATI, GL_GREEN_BIT_ATI, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_4_ATI, GL_BLUE_BIT_ATI, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_BIAS_BIT_ATI|GL_2X_BIT_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_2X_BIT_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NEGATE_BIT_ATI);

	glColorFragmentOp3ATI(GL_LERP_ATI,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_RED, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NEGATE_BIT_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NEGATE_BIT_ATI);

	glPassTexCoordATI(GL_REG_0_ATI, GL_REG_0_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_1_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_2_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_3_ATI, GL_REG_4_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_4_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp1ATI(GL_MOV_ATI,
		GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_0_ATI, GL_NONE, GL_COMP_BIT_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_ADD_ATI,
		GL_REG_1_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glEndFragmentShaderATI();
}
