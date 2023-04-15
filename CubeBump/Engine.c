#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <commdlg.h>
#include <mmsystem.h>
#include "glati.h"
#include "wglati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"
#include "obj.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

HPBUFFERARB hPbuffer=NULL;
HDC hPbufferDC=NULL;
HGLRC hPbufferRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

BOOL Done=FALSE, Key[256], ModelLoaded=FALSE, Wireframe=FALSE, Vectors=FALSE;

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

float RotateX, RotateY, Zoom;
float quat[4], matrix[16];

unsigned int FontBase, FontTexture, Base[1], Bump[1], CubeMap;
unsigned int FragmentShader[1], VertexShader[1];
unsigned int Tangent, ModelViewInverse, c0, VAO;

OBJ_Model_t Model[1];

float BumpScale=0.25f;

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

PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT=NULL;

PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI=NULL;
PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI=NULL;
PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI=NULL;
PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI=NULL;
PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI=NULL;
PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI=NULL;
PFNGLARRAYOBJECTATIPROC glArrayObjectATI=NULL;
PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI=NULL;
PFNGLGETARRAYOBJECTIVATIPROC glGetArrayObjectivATI=NULL;
PFNGLVARIANTARRAYOBJECTATIPROC glVariantArrayObjectATI=NULL;
PFNGLGETVARIANTARRAYOBJECTFVATIPROC glGetVariantArrayObjectfvATI=NULL;
PFNGLGETVARIANTARRAYOBJECTIVATIPROC glGetVariantArrayObjectivATI=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
unsigned int GenVectorMap(void);
void BuildVertexShader(unsigned int *ShaderID);
void BuildFragmentShader(unsigned int *ShaderID);
unsigned int BuildVertexBuffer(OBJ_Model_t *Model);
void DrawVertexBuffer(unsigned int ID, int tris);
void QuatRotate(float out[4], float angle, float x, float y, float z);
void QuatMultiply(float a[4], float b[4], float out[4]);
void QuatMatrix(float in[4], float out[16]);

float sinf(float x)
{
	__asm
	{
		fld x;
		fsin;
		fstp x;
	}

	return x;
}

float cosf(float x)
{
	__asm
	{
		fld x;
		fcos;
		fstp x;
	}

	return x;
}

float sqrtf(float x)
{
	__asm
	{
		fld x;
		fsqrt;
		fstp x;
	}

	return x;
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

	OBJ_FreeModel(&Model[0]);
	glFreeObjectBufferATI(VAO);

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
	ofn.lpstrFilter="Targa (*.tga)\0*.tga\0All Files (*.*)\0*.*\0";
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
		Base[0]=TGA_UploadImage(filename);
	}

	ofn.lpstrTitle="Open Bump Texture";

	if(GetOpenFileName(&ofn))
	{
		glDeleteTextures(1, &Bump[0]);
		Bump[0]=TGA_UploadDOT3(filename);
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
	ofn.lpstrFilter="Alias|Wavefront Model (*.obj)\0*.obj\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex=1;
	ofn.lpstrFileTitle=NULL;
	ofn.nMaxFileTitle=0;
	ofn.lpstrInitialDir=NULL;
	ofn.lpstrFileTitle="Open Model";
	ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;

	if(GetOpenFileName(&ofn))
	{
		OBJ_FreeModel(&Model[0]);
		glFreeObjectBufferATI(VAO);
		OBJ_LoadModel(&Model[0], filename);
		VAO=BuildVertexBuffer(&Model[0]);
	}
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

				case 'P':
					Wireframe^=TRUE;
					break;

				case 'L':
					Vectors^=TRUE;
					break;

				case VK_ADD:
					BumpScale+=0.01f;

					if(BumpScale>2.0f)
						BumpScale=2.0f;
					break;

				case VK_SUBTRACT:
					BumpScale-=0.01f;

					if(BumpScale<0.0f)
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

void DrawSkyBox(void)
{
	float size=128.0f;

	glBegin(GL_QUADS);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex3f(size, size, size);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex3f(size, -size, size);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-size, -size, size);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-size, size, size);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-size, size, -size);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-size, -size, -size);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex3f(size, -size, -size);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex3f(size, size, -size);
	glEnd();
}

void Render(void)
{
	float ModelViewMatrix[16], ModelViewInverseMatrix[16], QuatX[4], QuatY[4];

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, Zoom-48.0f);
	QuatRotate(QuatX, RotateX, 0.0f, 1.0f, 0.0f);
	QuatRotate(QuatY, RotateY, 1.0f, 0.0f, 0.0f);

	QuatMultiply(QuatX, QuatY, quat);
	QuatMatrix(quat, matrix);
	glMultMatrixf(matrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glPushMatrix();
	DrawSkyBox();
	glPopMatrix();
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glGetFloatv(GL_MODELVIEW_MATRIX, ModelViewMatrix);
	matrixInvert(ModelViewMatrix, ModelViewInverseMatrix);
	glSetInvariantEXT(ModelViewInverse, GL_FLOAT, ModelViewInverseMatrix);

	glSetInvariantEXT(c0, GL_FLOAT, &BumpScale);

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
	glBindTexture(GL_TEXTURE_2D, Base[0]);
	glEnable(GL_TEXTURE_2D);

	if(Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	DrawVertexBuffer(VAO, Model[0].numtriangles);
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_FRAGMENT_SHADER_ATI);

	glDisable(GL_VERTEX_SHADER_EXT);

	if(Vectors)
	{
		glPushMatrix();
			OBJ_DrawVectors(&Model[0]);
		glPopMatrix();
	}

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Font_Print(0, 64, FontTexture, "Bump scale: %0.2f\nTriangles: %d\nMTri/s: %0.1f\n%s\nFPS: %0.1f", BumpScale, Model[0].numtriangles, (fps*Model[0].numtriangles)/1000000, gluErrorString(glGetError()), fps);
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

	if(!IsExtensionSupported("GL_ATI_vertex_array_object"))
	{
		MessageBox(hWnd, "GL_ATI_vertex_array_object not found.", "OpenGL", MB_OK);
		return FALSE;
	}
	else
	{
		glNewObjectBufferATI=(PFNGLNEWOBJECTBUFFERATIPROC)wglGetProcAddress("glNewObjectBufferATI");
		glIsObjectBufferATI=(PFNGLISOBJECTBUFFERATIPROC)wglGetProcAddress("glIsObjectBufferATI");
		glUpdateObjectBufferATI=(PFNGLUPDATEOBJECTBUFFERATIPROC)wglGetProcAddress("glUpdateObjectBufferATI");
		glGetObjectBufferfvATI=(PFNGLGETOBJECTBUFFERFVATIPROC)wglGetProcAddress("glGetObjectBufferfvATI");
		glGetObjectBufferivATI=(PFNGLGETOBJECTBUFFERIVATIPROC)wglGetProcAddress("glGetObjectBufferivATI");
		glFreeObjectBufferATI=(PFNGLFREEOBJECTBUFFERATIPROC)wglGetProcAddress("glFreeObjectBufferATI");
		glArrayObjectATI=(PFNGLARRAYOBJECTATIPROC)wglGetProcAddress("glArrayObjectATI");
		glGetArrayObjectfvATI=(PFNGLGETARRAYOBJECTFVATIPROC)wglGetProcAddress("glGetArrayObjectfvATI");
		glGetArrayObjectivATI=(PFNGLGETARRAYOBJECTIVATIPROC)wglGetProcAddress("glGetArrayObjectivATI");
		glVariantArrayObjectATI=(PFNGLVARIANTARRAYOBJECTATIPROC)wglGetProcAddress("glVariantArrayObjectATI");
		glGetVariantArrayObjectfvATI=(PFNGLGETVARIANTARRAYOBJECTFVATIPROC)wglGetProcAddress("glGetVariantArrayObjectfvATI");
		glGetVariantArrayObjectivATI=(PFNGLGETVARIANTARRAYOBJECTIVATIPROC)wglGetProcAddress("glGetVariantArrayObjectivATI");
	}

	FontTexture=TGA_UploadImage("font.tga");

	Base[0]=TGA_UploadImage("nitroglbase.tga");
	Bump[0]=TGA_UploadDOT3("nitroglbump.tga");

	CubeMap=TGA_UploadCube("right.tga", "left.tga", "top.tga", "bottom.tga", "front.tga", "back.tga");

	if(OBJ_LoadModel(&Model[0], "sphere.obj"))
		ModelLoaded=TRUE;

	BuildVertexShader(VertexShader);
	BuildFragmentShader(FragmentShader);

	VAO=BuildVertexBuffer(&Model[0]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

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

void BuildVertexShader(unsigned int *ShaderID)
{
	unsigned int ModelView, Projection;
	unsigned int Vertex, Normal, TexCoord, Color, Texture;
	unsigned int EyePos, EyeVertex, Binormal, Mag, r0, r1, r2, T, B;

	ModelView=glBindParameterEXT(GL_MODELVIEW_MATRIX);
	Projection=glBindParameterEXT(GL_PROJECTION_MATRIX);
	Vertex=glBindParameterEXT(GL_CURRENT_VERTEX_EXT);
	Normal=glBindParameterEXT(GL_CURRENT_NORMAL);
	Color=glBindParameterEXT(GL_CURRENT_COLOR);
	TexCoord=glBindTextureUnitParameterEXT(GL_TEXTURE0_ARB, GL_CURRENT_TEXTURE_COORDS);
	Texture=glBindTextureUnitParameterEXT(GL_TEXTURE1_ARB, GL_TEXTURE_MATRIX);

	ShaderID[0]=glGenVertexShadersEXT(1);
	glBindVertexShaderEXT(ShaderID[0]);
	glBeginVertexShaderEXT();

	EyeVertex=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Binormal=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Mag=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	EyePos=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Binormal=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r1=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r2=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	T=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	B=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);

	Tangent=glGenSymbolsEXT(GL_VECTOR_EXT, GL_VARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	ModelViewInverse=glGenSymbolsEXT(GL_MATRIX_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);
	c0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);

	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, EyeVertex, ModelView, Vertex);
	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, EyeVertex, Projection, EyeVertex);
	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_VERTEX_EXT, EyeVertex);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_COLOR0_EXT, Color);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD0_EXT, TexCoord);

	glShaderOp2EXT(GL_OP_CROSS_PRODUCT_EXT, Binormal, Tangent, Normal);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, Binormal, Binormal);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, Binormal, Binormal, Mag);

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

	glSwizzleEXT(EyePos, EyePos, GL_ZERO_EXT, GL_ZERO_EXT, GL_ONE_EXT, GL_ONE_EXT);
	glShaderOp2EXT(GL_OP_MULTIPLY_MATRIX_EXT, EyePos, ModelViewInverse, EyePos);

	glShaderOp2EXT(GL_OP_SUB_EXT, EyePos, EyePos, Vertex);
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

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_1_ATI, GL_TEXTURE4_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_4_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_2_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_3_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);

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
		GL_REG_3_ATI, GL_NONE, GL_2X_BIT_ATI,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_3_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp3ATI(GL_MAD_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NEGATE_BIT_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_3_ATI, GL_NONE, GL_NONE);

	glSampleMapATI(GL_REG_1_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_ALPHA, GL_NONE,
		GL_REG_1_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_ADD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_HALF_BIT_ATI);

	glEndFragmentShaderATI();
}

void DrawVertexBuffer(unsigned int ID, int tris)
{
	if(ID==0)
		return;

	glArrayObjectATI(GL_VERTEX_ARRAY, 3, GL_FLOAT, sizeof(float)*11, ID, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glArrayObjectATI(GL_TEXTURE_COORD_ARRAY, 2, GL_FLOAT, sizeof(float)*11, ID, sizeof(float)*3);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVariantArrayObjectATI(Tangent, GL_FLOAT, sizeof(float)*11, ID, sizeof(float)*5);
	glEnableVariantClientStateEXT(Tangent);
	glArrayObjectATI(GL_NORMAL_ARRAY, 3, GL_FLOAT, sizeof(float)*11, ID, sizeof(float)*8);
	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, tris*3);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableVariantClientStateEXT(Tangent);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

unsigned int BuildVertexBuffer(OBJ_Model_t *Model)
{
	unsigned int index=0, tri, vert, size=0, ID;
	float *data=(float *)malloc(sizeof(float)*11*Model->numtriangles*3);

	if(data==NULL)
	{
		MessageBox(hWnd, "Data buffer malloc failed.", "OpenGL", MB_OK);
		return 0;
	}

	for(tri=0;tri<Model->numtriangles;tri++)
	{
		for(vert=0;vert<3;vert++)
		{
			data[index++]=Model->vertices[3*Model->triangles[tri].vindices[vert]];
			data[index++]=Model->vertices[3*Model->triangles[tri].vindices[vert]+1];
			data[index++]=Model->vertices[3*Model->triangles[tri].vindices[vert]+2];

			data[index++]=Model->texcoords[2*Model->triangles[tri].tindices[vert]];
			data[index++]=Model->texcoords[2*Model->triangles[tri].tindices[vert]+1];

			data[index++]=Model->tangents[3*Model->triangles[tri].vindices[vert]];
			data[index++]=Model->tangents[3*Model->triangles[tri].vindices[vert]+1];
			data[index++]=Model->tangents[3*Model->triangles[tri].vindices[vert]+2];

			data[index++]=Model->normals[3*Model->triangles[tri].vindices[vert]];
			data[index++]=Model->normals[3*Model->triangles[tri].vindices[vert]+1];
			data[index++]=Model->normals[3*Model->triangles[tri].vindices[vert]+2];
		}
	}

	ID=glNewObjectBufferATI(sizeof(float)*index, data, GL_STATIC_ATI);

	return ID;
}

void QuatRotate(float out[4], float angle, float x, float y, float z)
{
	float halfangle;
	float mag=1.0f/sqrtf(x*x+y*y+z*z);

	x*=mag;
	y*=mag;
	z*=mag;

	angle*=0.017453292f;

	halfangle=sinf(angle/2.0f);

	out[0]=cosf(angle/2.0f);
	out[1]=x*halfangle;
	out[2]=y*halfangle;
	out[3]=z*halfangle;
}

void QuatMultiply(float a[4], float b[4], float out[4])
{
	out[0]=b[0]*a[0]-b[1]*a[1]-b[2]*a[2]-b[3]*a[3];
	out[1]=b[0]*a[1]+b[1]*a[0]+b[2]*a[3]-b[3]*a[2];
	out[2]=b[0]*a[2]-b[1]*a[3]+b[2]*a[0]+b[3]*a[1];
	out[3]=b[0]*a[3]+b[1]*a[2]-b[2]*a[1]+b[3]*a[0];
}

void QuatMatrix(float in[4], float out[16])
{
	float xx, yy, zz;
	float mag=1.0f/sqrtf(in[0]*in[0]+in[1]*in[1]+in[2]*in[2]+in[3]*in[3]);

	in[0]*=mag;
	in[1]*=mag;
	in[2]*=mag;
	in[3]*=mag;

	xx=in[1]*in[1];
	yy=in[2]*in[2];
	zz=in[3]*in[3];

	out[0]=1.0f-2.0f*(yy+zz);
	out[1]=2.0f*(in[1]*in[2]+in[0]*in[3]);
	out[2]=2.0f*(in[1]*in[3]-in[0]*in[2]);
	out[3]=0.0f;
	out[4]=2.0f*(in[1]*in[2]-in[0]*in[3]);
	out[5]=1.0f-2.0f*(xx+zz);
	out[6]=2.0f*(in[2]*in[3]+in[0]*in[1]);
	out[7]=0.0f;
	out[8]=2.0f*(in[1]*in[3]+in[0]*in[2]);
	out[9]=2.0f*(in[2]*in[3]-in[0]*in[1]);
	out[10]=1.0f-2.0f*(xx+yy);
	out[11]=0.0f;
	out[12]=0.0f;
	out[13]=0.0f;
	out[14]=0.0f;
	out[15]=1.0f;
}
