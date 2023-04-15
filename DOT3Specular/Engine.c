#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <commdlg.h>
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

BOOL Done=FALSE, Key[256], ModelLoaded=FALSE, Wireframe=FALSE;

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

unsigned int FontBase, FontTexture, Base[1], Bump[1], VectorMap, FalloffMap, NHKMap;
unsigned int FragmentShader[1], VertexShader[1];
unsigned int Tangent, EyePos;

OBJ_Model_t Model[1];

float light0[4]={ 0.0f, 0.0f, 48.0f, 1.0f };
float light0color[3]={ 1.0f, 1.0f, 1.0f };

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
unsigned int GenVectorMap(void);
unsigned int Gen3DFalloffMap(void);
unsigned int GenNHKMap(int MinK, int MaxK);
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
		Base[0]=TGA_UploadImage(filename);

	ofn.lpstrTitle="Open Bump Texture";

	if(GetOpenFileName(&ofn))
		Bump[0]=TGA_UploadDOT3(filename);
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
		OBJ_LoadModel(&Model[0], filename);
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

			if(wParam==(MK_LBUTTON|MK_CONTROL))
			{
				light0[0]+=mouse.dx;
				light0[1]+=mouse.dy;
			}

			if(wParam==(MK_RBUTTON|MK_CONTROL))
				light0[2]+=mouse.dy;

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

				case 'W':
					Wireframe^=TRUE;
					break;

				case 'A':
					light0color[0]+=0.05f;

					if(light0color[0]>1.0f)
						light0color[0]=1.0f;
					break;

				case 'Z':
					light0color[0]-=0.05f;

					if(light0color[0]<0.0f)
						light0color[0]=0.0f;
					break;

				case 'S':
					light0color[1]+=0.05f;

					if(light0color[1]>1.0f)
						light0color[1]=1.0f;
					break;

				case 'X':
					light0color[1]-=0.05f;

					if(light0color[1]<0.0f)
						light0color[1]=0.0f;
					break;

				case 'D':
					light0color[2]+=0.05f;

					if(light0color[2]>1.0f)
						light0color[2]=1.0f;
					break;

				case 'C':
					light0color[2]-=0.05f;

					if(light0color[2]<0.0f)
						light0color[2]=0.0f;
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

void matrixInvert(GLfloat in[16], GLfloat out[16])
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

void vecMatMult(float vecIn[3], float m[16], float vecOut[3])
{
	vecOut[0]=(vecIn[0]*m[0])+(vecIn[1]*m[4])+(vecIn[2]*m[8])+m[12];
	vecOut[1]=(vecIn[0]*m[1])+(vecIn[1]*m[5])+(vecIn[2]*m[9])+m[13];
	vecOut[2]=(vecIn[0]*m[2])+(vecIn[1]*m[6])+(vecIn[2]*m[10])+m[14];
}

void Render(void)
{
	float eyePosEyeSpace[3]={ 0.0f, 0.0f, 0.0f }, eyePosObjectSpace[3], modelViewMatrix[16], modelViewMatrixInverse[16];

	glLightfv(GL_LIGHT0, GL_POSITION, light0);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, Zoom-48.0f);
	glRotatef(RotateX, 0.0f, 1.0f, 0.0f);
	glRotatef(RotateY, 1.0f, 0.0f, 0.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	matrixInvert(modelViewMatrix, modelViewMatrixInverse);
	vecMatMult(eyePosEyeSpace, modelViewMatrixInverse, eyePosObjectSpace);
	glSetInvariantEXT(EyePos, GL_FLOAT, eyePosObjectSpace);

	glBegin(GL_POINTS);
	glColor3fv(light0color);
	glVertex3fv(light0);
	glEnd();

	glBindVertexShaderEXT(VertexShader[0]);
	glEnable(GL_VERTEX_SHADER_EXT);

	glBindFragmentShaderATI(FragmentShader[0]);
	glEnable(GL_FRAGMENT_SHADER_ATI);
	glSetFragmentShaderConstantATI(GL_CON_0_ATI, light0color);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Base[0]);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Bump[0]);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, VectorMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glBindTexture(GL_TEXTURE_3D_EXT, FalloffMap);
	glEnable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE5_ARB);
	glBindTexture(GL_TEXTURE_2D, NHKMap);
	glEnable(GL_TEXTURE_2D);

	if(Wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
		if(ModelLoaded)
			OBJ_Draw(&Model[0]);
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTextureARB(GL_TEXTURE5_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D_EXT);

	glDisable(GL_FRAGMENT_SHADER_ATI);

	glDisable(GL_VERTEX_SHADER_EXT);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 32, FontBase, FontTexture, "Light Color:  R - %0.2f  G - %0.2f  B - %0.2f", light0color[0], light0color[1], light0color[2]);
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

	if(!IsExtensionSupported("GL_EXT_texture3D"))
	{
		MessageBox(hWnd, "GL_EXT_texture3D not found!", "OpenGL", MB_OK);
		return FALSE;
	}
	else
		glTexImage3DEXT=(PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress("glTexImage3DEXT");

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	Base[0]=TGA_UploadImage("crumplebase.tga");
	Bump[0]=TGA_UploadDOT3("crumplebump.tga");

	VectorMap=GenVectorMap();
	FalloffMap=Gen3DFalloffMap();
	NHKMap=GenNHKMap(0, 16);

	if(OBJ_LoadModel(&Model[0], "torus.obj"))
		ModelLoaded=TRUE;

	BuildVertexShader(VertexShader);
	BuildFragmentShader(FragmentShader);

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

unsigned int Gen3DFalloffMap(void)
{
	unsigned int TextureID;
	int x, y, z, size=16;
	float rad=(float)size-1, rad_sq=rad*rad;
	unsigned char *buffer;

	buffer=malloc(size*size*size);

	for(z=0;z<size;z++)
	{
		for(y=0;y<size;y++)
		{
			for(x=0;x<size;x++)
			{
				float d_sq=(float)((z*z)+(y*y)+(x*x));

				if(d_sq<rad_sq)
				{
					float falloff=((rad_sq-d_sq)/rad_sq)*((rad_sq-d_sq)/rad_sq);

					buffer[z*size*size+y*size+x]=(unsigned char)(255.0f*falloff);
				}
				else
					buffer[z*size*size+y*size+x]=0;
			}
		}
	}

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_3D_EXT, TextureID);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_MIRROR_CLAMP_TO_EDGE_ATI);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_MIRROR_CLAMP_TO_EDGE_ATI);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R_EXT, GL_MIRROR_CLAMP_TO_EDGE_ATI);
	glTexParameterf(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE8, size, size, size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

	return TextureID;
}

unsigned int GenNHKMap(int MinK, int MaxK)
{
	unsigned int TextureID;
	unsigned char *buffer;
	int x, y, size=256;

	buffer=malloc(size*size);

	for(y=0;y<size;y++)
		for(x=0;x<size;x++)
			if(x==0)
				buffer[y*size+x]=0;
			else
				buffer[y*size+x]=(unsigned char)(255.0f*min((float)pow((((float)y)/((float)size-1)), ((((float)x*(float)(MaxK-MinK))/((float)size-1))+(float)MinK)), 1.0f));

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, size, size, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
}

void BuildVertexShader(unsigned int *ShaderID)
{
	unsigned int ModelView, Projection;
	unsigned int EyeVertex, Vertex, Normal, Binormal, TexCoord, Color, Light0;
	unsigned int Mag, TempVector, r0, r1, r2, r3, c0;
	float c0f=1.0f/512.0f;

	ModelView=glBindParameterEXT(GL_MODELVIEW_MATRIX);
	Projection=glBindParameterEXT(GL_PROJECTION_MATRIX);
	Vertex=glBindParameterEXT(GL_CURRENT_VERTEX_EXT);
	Normal=glBindParameterEXT(GL_CURRENT_NORMAL);
	Color=glBindParameterEXT(GL_CURRENT_COLOR);
	Light0=glBindLightParameterEXT(GL_LIGHT0, GL_POSITION);
	TexCoord=glBindTextureUnitParameterEXT(GL_TEXTURE0_ARB, GL_CURRENT_TEXTURE_COORDS);

	ShaderID[0]=glGenVertexShadersEXT(1);
	glBindVertexShaderEXT(ShaderID[0]);
	glBeginVertexShaderEXT();

	c0=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_CONSTANT_EXT, GL_FULL_RANGE_EXT, 1);
	glSetLocalConstantEXT(c0, GL_FLOAT, &c0f);

	EyeVertex=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Binormal=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r0=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r1=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r2=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	r3=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	Mag=glGenSymbolsEXT(GL_SCALAR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);
	TempVector=glGenSymbolsEXT(GL_VECTOR_EXT, GL_LOCAL_EXT, GL_FULL_RANGE_EXT, 1);

	Tangent=glGenSymbolsEXT(GL_VECTOR_EXT, GL_VARIANT_EXT, GL_FULL_RANGE_EXT, 1);

	EyePos=glGenSymbolsEXT(GL_VECTOR_EXT, GL_INVARIANT_EXT, GL_FULL_RANGE_EXT, 1);

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

	glShaderOp2EXT(GL_OP_SUB_EXT, r1, EyePos, Vertex);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, r1, r1);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, r1, r1, Mag);

	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Tangent, r0);
	glWriteMaskEXT(r2, TempVector, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Binormal, r0);
	glWriteMaskEXT(r2, TempVector, GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE);
	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Normal, r0);
	glWriteMaskEXT(r2, TempVector, GL_FALSE, GL_FALSE, GL_TRUE, GL_FALSE);

	glShaderOp1EXT(GL_OP_MOV_EXT, GL_OUTPUT_TEXTURE_COORD1_EXT, r2);

	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Tangent, r1);
	glWriteMaskEXT(r3, TempVector, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Binormal, r1);
	glWriteMaskEXT(r3, TempVector, GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE);
	glShaderOp2EXT(GL_OP_DOT3_EXT, TempVector, Normal, r1);
	glWriteMaskEXT(r3, TempVector, GL_FALSE, GL_FALSE, GL_TRUE, GL_FALSE);

	glShaderOp2EXT(GL_OP_ADD_EXT, r3, r2, r3);
	glShaderOp2EXT(GL_OP_DOT3_EXT, Mag, r3, r3);
	glShaderOp1EXT(GL_OP_RECIP_SQRT_EXT, Mag, Mag);
	glShaderOp2EXT(GL_OP_MUL_EXT, GL_OUTPUT_TEXTURE_COORD2_EXT, r3, Mag);

	glShaderOp2EXT(GL_OP_SUB_EXT, r0, Vertex, Light0);
	glShaderOp2EXT(GL_OP_MUL_EXT, GL_OUTPUT_TEXTURE_COORD3_EXT, r0, c0);

	glEndVertexShaderEXT();
}

void BuildFragmentShader(unsigned int *ShaderID)
{
	ShaderID[0]=glGenFragmentShadersATI(1);
	glBindFragmentShaderATI(ShaderID[0]);
	glBeginFragmentShaderATI();

//	glSetFragmentShaderConstantATI(GL_CON_0_ATI, light0color);

	glSampleMapATI(GL_REG_1_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_2_ATI, GL_TEXTURE1_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_3_ATI, GL_TEXTURE2_ARB, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_2_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_2X_BIT_ATI|GL_BIAS_BIT_ATI,
		GL_REG_2_ATI, GL_NONE, GL_2X_BIT_ATI|GL_BIAS_BIT_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_CON_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_DOT3_ATI,
		GL_REG_3_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_1_ATI, GL_NONE, GL_2X_BIT_ATI|GL_BIAS_BIT_ATI,
		GL_REG_3_ATI, GL_NONE, GL_2X_BIT_ATI|GL_BIAS_BIT_ATI);

	glSampleMapATI(GL_REG_0_ATI, GL_TEXTURE0_ARB, GL_SWIZZLE_STR_ATI);
	glPassTexCoordATI(GL_REG_2_ATI, GL_REG_2_ATI, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_4_ATI, GL_TEXTURE3_ARB, GL_SWIZZLE_STR_ATI);
	glSampleMapATI(GL_REG_5_ATI, GL_REG_3_ATI, GL_SWIZZLE_STR_ATI);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_5_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_ALPHA, GL_NONE,
		GL_REG_5_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_ADD_ATI,
		GL_REG_0_ATI, GL_NONE, GL_SATURATE_BIT_ATI,
		GL_REG_5_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_4_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE);

	glColorFragmentOp2ATI(GL_MUL_ATI,
		GL_REG_0_ATI, GL_NONE, GL_NONE,
		GL_REG_2_ATI, GL_NONE, GL_NONE,
		GL_REG_0_ATI, GL_NONE, GL_NONE);

//	glColorFragmentOp1ATI(GL_MOV_ATI,
//		GL_REG_0_ATI, GL_NONE, GL_NONE,
//		GL_REG_5_ATI, GL_NONE, GL_NONE);

	glEndFragmentShaderATI();
}
