#include <windows.h>
#include <time.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "wglati.h"
#include "image.h"
#include "overlay.h"
#include "font.h"
#include "3ds.h"
#include "quat.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

int Done=0, Key[256], HasVAO=0, HasMOB=0, IsSpinner=1, IsSpinning=1, IsManualControl=0;

LARGE_INTEGER timer_freq, startTime, currentTime, time_counter, time_current;
float fps=0.0f, fTime=0.0f, fWaterUpdate=0.0f;
int Frames=0;
const float OneOver60=1.0f/60.0f;

unsigned int FontTexture, Spinner, Swimmer, CubeMap, Orignal, Blur;

typedef enum
{
	FRAGMENT_FORCECALC,
	FRAGMENT_APPLYFORCE,
	FRAGMENT_APPLYVELOCITY,
	FRAGMENT_WEIGHTCOMBINE,
	FRAGMENT_NORMALMAP,
	FRAGMENT_REFLECT,
	FRAGMENT_BLURX,
	FRAGMENT_BLURY,
	FRAGMENT_COMBINE,
	NUM_FRAGMENT_PROGRAMS
} FragmentProgramNames;

typedef enum
{
	VERTEX_GENERIC,
	NUM_VERTEX_PROGRAMS
} VertexProgramNames;

unsigned int FragmentProgram[NUM_FRAGMENT_PROGRAMS], VertexProgram[NUM_VERTEX_PROGRAMS];

typedef enum
{
	DYNAMIC_TEXTURE_FORCE_TARGET,
	DYNAMIC_TEXTURE_VELOCITY_SOURCE,
	DYNAMIC_TEXTURE_VELOCITY_TARGET,
	DYNAMIC_TEXTURE_HEIGHT_SOURCE,
	DYNAMIC_TEXTURE_HEIGHT_TARGET,
	DYNAMIC_TEXTURE_NORMALMAP,
	NUM_DYNAMIC_TEXTURES
} DynamicTextureNames;

unsigned int DynamicTexture[NUM_DYNAMIC_TEXTURES];
unsigned int TexHeightInput, TexHeightOutput;
unsigned int TexVelocityInput, TexVelocityOutput;
unsigned int TexForceOutput;
int FlipState=0, WaterTexWidth=256, WaterTexHeight=256;

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

float Position[2]={ 0.0f, 0.0f };

TDS_Model_t Model;

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

PFNGLMAPOBJECTBUFFERATIPROC glMapObjectBufferATI=NULL;
PFNGLUNMAPOBJECTBUFFERATIPROC glUnmapObjectBufferATI=NULL;

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB=NULL;

PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB=NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB=NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
int CreateContext(HWND hWnd, HDC *hDC, HGLRC *hRC, int Color, int Depth, int Stencil, int SampleBuffers);
void DestroyContext(HWND hWnd, HDC *hDC, HGLRC *hRC);
void Resize(int Width, int Height);
int IsExtensionSupported(const char *extension);
void BuildVertexProgram(unsigned int *ShaderID);
void BuildFragmentProgram(unsigned int *ShaderID);
void InitWater(void);
void UpdateWater(void);
unsigned int FLOAT_UploadCubeVirtCross(char *filename);

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

	if(!CreateContext(hWnd, &hDC, &hRC, 32, 24, 0, (MessageBox(hWnd, "Enable FSAA (if supported)?", "OpenGL", MB_ICONQUESTION|MB_YESNO)==IDYES)?4:0))
		Done=1;

	if(!Done)
		Resize(Width, Height);

	if(!Done)
	{
		if(!Init())
			Done=1;
	}

	while(!Done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				Done=1;
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

			Frames++;
			if(Frames>20)
			{
				QueryPerformanceCounter(&currentTime);
				fps=(float)Frames/((float)(currentTime.QuadPart-startTime.QuadPart)/(float)timer_freq.QuadPart);
				Frames=0;
				QueryPerformanceCounter(&startTime);
			}
		}
	}

	DestroyContext(hWnd, &hDC, &hRC);
	DestroyWindow(hWnd);

	return 0;
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

			if(wParam==MK_MBUTTON)
			{
				PanX+=mouse.dx;
				PanY+=mouse.dy;
			}

			if(wParam==MK_RBUTTON)
				Zoom+=mouse.dy;

			if(wParam==(MK_LBUTTON|MK_CONTROL))
			{
				Position[0]+=mouse.dx;
				Position[1]+=mouse.dy;
			}

			break;

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case '1':
					IsSpinner^=1;
					break;

				case '2':
					IsSpinning^=1;
					break;

				case '3':
					IsManualControl^=1;
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

void MatrixInvert(float *m)
{
	float res[16];

	res[0]=m[0];
	res[1]=m[4];
	res[2]=m[8];
	res[3]=0.0f;
	res[4]=m[1];
	res[5]=m[5];
	res[6]=m[9];
	res[7]=0.0f;
	res[8]=m[2];
	res[9]=m[6];
	res[10]=m[10];
	res[11]=0.0f;
	res[12]=-(m[12]*m[0])-(m[13]*m[1])-(m[14]*m[2]);
	res[13]=-(m[12]*m[4])-(m[13]*m[5])-(m[14]*m[6]);
	res[14]=-(m[12]*m[8])-(m[13]*m[9])-(m[14]*m[10]);
	res[15]=1.0f;

	memcpy(m, res, sizeof(float)*16);
}

// 3D Studio model render functions
void TDS_DrawModel(TDS_Model_t *Model)
{
	int i;

	for(i=0;i<Model->NumMesh;i++)
	{
		if(HasVAO&&Model->Mesh[i].VAOID)
		{
			glArrayObjectATI(GL_VERTEX_ARRAY, 3, GL_FLOAT, sizeof(float)*14, Model->Mesh[i].VAOID, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			glArrayObjectATI(GL_TEXTURE_COORD_ARRAY, 2, GL_FLOAT, sizeof(float)*14, Model->Mesh[i].VAOID, sizeof(float)*3);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glArrayObjectATI(GL_TEXTURE_COORD_ARRAY, 3, GL_FLOAT, sizeof(float)*14, Model->Mesh[i].VAOID, sizeof(float)*5);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE2_ARB);
			glArrayObjectATI(GL_TEXTURE_COORD_ARRAY, 3, GL_FLOAT, sizeof(float)*14, Model->Mesh[i].VAOID, sizeof(float)*8);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glArrayObjectATI(GL_NORMAL_ARRAY, 3, GL_FLOAT, sizeof(float)*14, Model->Mesh[i].VAOID, sizeof(float)*11);
			glEnableClientState(GL_NORMAL_ARRAY);
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, 0, Model->Mesh[i].Vertex);
			glEnableClientState(GL_VERTEX_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			glTexCoordPointer(2, GL_FLOAT, 0, Model->Mesh[i].UV);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glTexCoordPointer(3, GL_FLOAT, 0, Model->Mesh[i].Tangent);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE2_ARB);
			glTexCoordPointer(3, GL_FLOAT, 0, Model->Mesh[i].Binormal);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glNormalPointer(GL_FLOAT, 0, Model->Mesh[i].Normal);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		glDrawElements(GL_TRIANGLES, Model->Mesh[i].NumFace*3, GL_UNSIGNED_SHORT, Model->Mesh[i].Face);

		glClientActiveTextureARB(GL_TEXTURE2_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);
}

void TDS_BuildVAO(TDS_Model_t *Model)
{
	int i, j;

	for(i=0;i<Model->NumMesh;i++)
	{
		int index=0;
		float *data=NULL;

		Model->Mesh[i].VAOID=glNewObjectBufferATI(sizeof(float)*Model->Mesh[i].NumVertex*14, NULL, GL_DYNAMIC_ATI);

		if(HasMOB)
			data=(float *)glMapObjectBufferATI(Model->Mesh[i].VAOID);
		else
			data=(float *)malloc(sizeof(float)*Model->Mesh[i].NumVertex*14);

		if(data==NULL)
		{
			char temp[1024];

			sprintf(temp, "VAO data buffer memory allocate/map failed for object: %s", Model->Mesh[i].Name);
			MessageBox(NULL, temp, "OpenGL", MB_OK);

			glFreeObjectBufferATI(Model->Mesh[i].VAOID);
			Model->Mesh[i].VAOID=0;

			break;
		}

		for(j=0;j<Model->Mesh[i].NumVertex;j++)
		{
			data[index++]=Model->Mesh[i].Vertex[3*j];
			data[index++]=Model->Mesh[i].Vertex[3*j+1];
			data[index++]=Model->Mesh[i].Vertex[3*j+2];

			data[index++]=Model->Mesh[i].UV[2*j];
			data[index++]=Model->Mesh[i].UV[2*j+1];

			data[index++]=Model->Mesh[i].Tangent[3*j];
			data[index++]=Model->Mesh[i].Tangent[3*j+1];
			data[index++]=Model->Mesh[i].Tangent[3*j+2];

			data[index++]=Model->Mesh[i].Binormal[3*j];
			data[index++]=Model->Mesh[i].Binormal[3*j+1];
			data[index++]=Model->Mesh[i].Binormal[3*j+2];

			data[index++]=Model->Mesh[i].Normal[3*j];
			data[index++]=Model->Mesh[i].Normal[3*j+1];
			data[index++]=Model->Mesh[i].Normal[3*j+2];
		}

		if(HasMOB)
			glUnmapObjectBufferATI(Model->Mesh[i].VAOID);
		else
		{
			glUpdateObjectBufferATI(Model->Mesh[i].VAOID, 0, sizeof(float)*index, data, GL_DISCARD_ATI);
			FREE(data);
		}
	}
}

void TDS_LoadTextures(TDS_Model_t *Model)
{
	int i;

	for(i=0;i<Model->NumMaterial;i++)
	{
		char temp[1024];

		Model->Material[i].BaseTexID=0;
		Model->Material[i].BumpTexID=0;

		if(Model->Material[i].Texture[0])
		{
			sprintf(temp, "%s", Model->Material[i].Texture);
			Model->Material[i].BaseTexID=IMAGE_UploadImage(temp);

			sprintf(temp, "bump%s", Model->Material[i].Texture);
			Model->Material[i].BumpTexID=IMAGE_UploadDOT3_64Bit(temp);
		}
	}
}
// End of 3D Studio render functions

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

void DownsampleTexture(void)
{
	float w=(float)Width*0.25f, h=(float)Height*0.25f;
	float TexelX=(float)Width/1024.0f, TexelY=(float)Height/1024.0f;

	glBindTexture(GL_TEXTURE_2D, Blur);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, TexelY);
		glVertex3f(0.0f, h, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(TexelX, TexelY);
		glVertex3f(w, h, -1.0f);
		glTexCoord2f(TexelX, 0.0f);
		glVertex3f(w, 0.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Updating 1/4 of the image causes bilinear artifacts, so only update 1/2
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width/2, Height/2);
}

void BlurTexture(void)
{
	float w=(float)Width*0.25f, h=(float)Height*0.25f;
	float TexelX=w/1024.0f, TexelY=h/1024.0f;
	float TexelW=1.0f/(float)1024, TexelH=1.0f/(float)1024;

	glBindTexture(GL_TEXTURE_2D, Blur);

	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, TexelW, TexelH, 0.0f, 0.0f);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_BLURX]);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, TexelY);
		glVertex3f(0.0f, h, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(TexelX, TexelY);
		glVertex3f(w, h, -1.0f);
		glTexCoord2f(TexelX, 0.0f);
		glVertex3f(w, 0.0f, -1.0f);
	glEnd();

	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width/4, Height/4);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_BLURY]);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, TexelY);
		glVertex3f(0.0f, h, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(TexelX, TexelY);
		glVertex3f(w, h, -1.0f);
		glTexCoord2f(TexelX, 0.0f);
		glVertex3f(w, 0.0f, -1.0f);
	glEnd();

	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width/4, Height/4);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void CombineTexture(void)
{
	float w=(float)Width*0.25f, h=(float)Height*0.25f;
	float TexelX=(float)Width/1024.0f, TexelY=(float)Height/1024.0f;
	float TexelX1=w/1024.0f, TexelY1=h/1024.0f;

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_COMBINE]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Orignal);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Blur);

	glBegin(GL_TRIANGLE_STRIP);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, TexelY);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, TexelY1);
		glVertex3f(0.0f, (float)Height, -1.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -1.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, TexelX, TexelY);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, TexelX1, TexelY1);
		glVertex3f((float)Width, (float)Height, -1.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, TexelX, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, TexelX1, 0.0f);
		glVertex3f((float)Width, 0.0f, -1.0f);
	glEnd();

	glActiveTextureARB(GL_TEXTURE0_ARB);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
}

void Render(void)
{
	float QuatX[4], QuatY[4];
	float fTimeStep;

	time_current=time_counter;
	QueryPerformanceCounter(&time_counter);

	fTimeStep=(float)(time_counter.QuadPart-time_current.QuadPart)/timer_freq.QuadPart;
	fTime+=fTimeStep;
	fWaterUpdate+=fTimeStep;

	// Only update every 60 frames
	if(fWaterUpdate>=OneOver60)
	{
		UpdateWater();
		fWaterUpdate=0.0f;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(PanX, PanY, Zoom-200.0f);
	QuatAngle(QuatX, RotateX, 0.0f, 1.0f, 0.0f);
	QuatAngle(QuatY, RotateY, 1.0f, 0.0f, 0.0f);
	QuatMultiply(QuatX, QuatY, quat);
	QuatMatrix(quat, matrix);
	glMultMatrixf(matrix);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glPushMatrix();
	DrawSkyBox();
	glPopMatrix();
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);

	glPushMatrix();
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexProgram[VERTEX_GENERIC]);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_REFLECT]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, DynamicTexture[DYNAMIC_TEXTURE_NORMALMAP]);
	glEnable(GL_TEXTURE_2D);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, CubeMap);

	TDS_DrawModel(&Model);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	glDisable(GL_VERTEX_PROGRAM_ARB);
	glPopMatrix();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, Orignal);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);

	glBindTexture(GL_TEXTURE_2D, Blur);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		DownsampleTexture();
		BlurTexture();
		CombineTexture();

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

	if(!IsExtensionSupported("GL_ARB_fragment_program"))
	{
		MessageBox(hWnd, "GL_ARB_fragment_program not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	//ATI VAO for fast rendering
	if(!IsExtensionSupported("GL_ATI_vertex_array_object"))
	{
		MessageBox(hWnd, "GL_ATI_vertex_array_object not found!\nFalling back to standard vertex arrays...", "OpenGL", MB_OK);
		HasVAO=0;
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
		HasVAO=1;

		//ATI MOB for fast loading
		if(!IsExtensionSupported("GL_ATI_map_object_buffer"))
			HasMOB=0;
		else
		{
			glMapObjectBufferATI=(PFNGLMAPOBJECTBUFFERATIPROC)wglGetProcAddress("glMapObjectBufferATI");
			glUnmapObjectBufferATI=(PFNGLUNMAPOBJECTBUFFERATIPROC)wglGetProcAddress("glUnmapObjectBufferATI");
			HasMOB=1;
		}
	}

	FontTexture=IMAGE_UploadImage("font.tga");

	Spinner=IMAGE_UploadImage("spinner.tga");
	Swimmer=IMAGE_UploadImage("swimmer.tga");

	CubeMap=FLOAT_UploadCubeVirtCross("gracecubemap.float");

	BuildVertexProgram(VertexProgram);
	BuildFragmentProgram(FragmentProgram);

	TDS_LoadModel(&Model, "sphere.3ds");

	if(HasVAO)
		TDS_BuildVAO(&Model);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPointSize(16.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	InitWater();

	glGenTextures(1, &Blur);
	glBindTexture(GL_TEXTURE_2D, Blur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenTextures(1, &Orignal);
	glBindTexture(GL_TEXTURE_2D, Orignal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	return TRUE;
}

int CreateContext(HWND hWnd, HDC *hDC, HGLRC *hRC, int Color, int Depth, int Stencil, int SampleBuffers)
{
	HWND hWndTemp=NULL;
	HDC hDCTemp=NULL;
	HGLRC hRCTemp=NULL;
	PIXELFORMATDESCRIPTOR pfd;
	BOOL IsMultisample=TRUE;
	unsigned int PixelFormat, NumFormats;
	int Attrib[128], i=0;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=16;
	pfd.cDepthBits=16;

	hWndTemp=CreateWindow("STATIC", NULL, WS_POPUP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, 0, 0, 0, 0, NULL, NULL, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
	hDCTemp=GetDC(hWndTemp);

	if(!(PixelFormat=ChoosePixelFormat(hDCTemp, &pfd)))
	{
		MessageBox(hWnd, "ChoosePixelFormat Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!SetPixelFormat(hDCTemp, PixelFormat, &pfd))
	{
		MessageBox(hWnd, "SetPixelFormat Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!(hRCTemp=wglCreateContext(hDCTemp)))
	{
		MessageBox(hWnd, "wglCreateContext Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!wglMakeCurrent(hDCTemp, hRCTemp))
	{
		MessageBox(hWnd, "wglMakeCurrent Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!IsExtensionSupported("WGL_ARB_pixel_format"))
	{
		MessageBox(hWnd, "WGL_ARB_pixel_format not found!", "ERROR", MB_OK);
		return 0;
	}
	else
	{
		wglGetPixelFormatAttribivARB=(PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
		wglGetPixelFormatAttribfvARB=(PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
		wglChoosePixelFormatARB=(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	}

	if(!IsExtensionSupported("WGL_ARB_multisample"))
		IsMultisample=FALSE;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCTemp);
	ReleaseDC(hWndTemp, hDCTemp);
	DestroyWindow(hWndTemp);

	*hDC=GetDC(hWnd);

	Attrib[i++]=WGL_DRAW_TO_WINDOW_ARB;
	Attrib[i++]=TRUE;
	Attrib[i++]=WGL_SUPPORT_OPENGL_ARB;
	Attrib[i++]=TRUE;
	Attrib[i++]=WGL_DOUBLE_BUFFER_ARB;
	Attrib[i++]=TRUE;
	Attrib[i++]=WGL_PIXEL_TYPE_ARB;
	Attrib[i++]=WGL_TYPE_RGBA_ARB;
	Attrib[i++]=WGL_COLOR_BITS_ARB;
	Attrib[i++]=Color;
	Attrib[i++]=WGL_DEPTH_BITS_ARB;
	Attrib[i++]=Depth;
	Attrib[i++]=WGL_STENCIL_BITS_ARB;
	Attrib[i++]=Stencil;

	if(IsMultisample&&SampleBuffers)
	{
		Attrib[i++]=WGL_SAMPLE_BUFFERS_ARB;
		Attrib[i++]=TRUE;
		Attrib[i++]=WGL_SAMPLES_ARB;
		Attrib[i++]=SampleBuffers;
	}

	Attrib[i++]=0;

	if(!wglChoosePixelFormatARB(*hDC, Attrib, NULL, 1, &PixelFormat, &NumFormats))
	{
		MessageBox(hWnd, "wglChoosePixelFormatARB failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!SetPixelFormat(*hDC, PixelFormat, NULL))
	{
		MessageBox(hWnd, "SetPixelFormat Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!(*hRC=wglCreateContext(*hDC)))
	{
		MessageBox(hWnd, "wglCreateContext Failed!", "ERROR", MB_OK);
		return 0;
	}

	if(!wglMakeCurrent(*hDC, *hRC))
	{
		MessageBox(hWnd, "wglMakeCurrent Failed!", "ERROR", MB_OK);
		return 0;
	}

	return 1;
}

void DestroyContext(HWND hWnd, HDC *hDC, HGLRC *hRC)
{
	if(hDC==NULL)
		return;

	if(hRC==NULL)
		return;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(*hRC);
	ReleaseDC(hWnd, *hDC);
}

float *infinite_perspective(float fovy, float aspect, float zNear)
{
	float y=(float)tan((fovy/2.0f)*3.14159f/180.0f)*zNear;
	float x=aspect*y;
	float *m=(float *)malloc(sizeof(float)*16);
	float nudge=1.0f-1.0f/(1<<23);

	memset(m, 0, sizeof(float)*16);

	m[0]=(2.0f*zNear)/(x+x);
	m[5]=(2.0f*zNear)/(y+y);
	m[8]=(x-x)/(x+x);
	m[9]=(y-y)/(y+y);
	m[10]=-1.0f*nudge;
	m[11]=-1.0f;
	m[14]=-2.0f*zNear*nudge;

	return m;
}

void Resize(int Width, int Height)
{
	if(Height==0)
		Height=1;

	glViewport(0,0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(infinite_perspective(90.0f, (float)Width/Height, 0.01f));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int IsExtensionSupported(const char *extension)
{
	const char *extensions=NULL, *start, *where, *terminator;

	where=strchr(extension, ' ');

	if(where||*extension=='\0')
		return 0;

	extensions=(const char *)glGetString(GL_EXTENSIONS);
	start=extensions;

	for(;;)
	{
		where=strstr(start, extension);

		if(!where)
			break;

		terminator=where+strlen(extension);

		if(where==start||*(where-1)==' ')
			if(*terminator==' '||*terminator=='\0')
				return 1;

		start=terminator;
	}

	extensions=NULL;

	if(wglGetExtensionsStringARB==NULL)
		wglGetExtensionsStringARB=(PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

	if(wglGetExtensionsStringARB==NULL)
		return 0;

	where=strchr(extension, ' ');

	if(where||*extension=='\0')
		return 0;

	extensions=wglGetExtensionsStringARB(wglGetCurrentDC());
	start=extensions;

	for(;;)
	{
		where=strstr(start, extension);

		if(!where)
			break;

		terminator=where+strlen(extension);

		if(where==start||*(where-1)==' ')
			if(*terminator==' '||*terminator=='\0')
				return 1;

		start=terminator;
	}

	return 0;
}

void UploadProgram(unsigned int Target, unsigned int ID, char *filename)
{
	FILE *stream;
	int errorposition;
	char *Program;
	long bytes;

	if(!(stream=fopen(filename, "rb")))
	{
		ID=0;
		return;
	}

	fseek(stream, 0, SEEK_END);
	bytes=ftell(stream);
	fseek(stream, 0, SEEK_SET);

	Program=(char *)malloc(sizeof(char)*bytes+1);

	bytes=fread(Program, 1, bytes, stream);

	Program[bytes]=0;

	fclose(stream);

	glBindProgramARB(Target, ID);
	glProgramStringARB(Target, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	FREE(Program);

	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorposition);

	if(errorposition==-1)
		return;

	MessageBox(hWnd, (const char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB), filename, MB_OK);
}

void BuildVertexProgram(unsigned int *ProgramID)
{
	glGenProgramsARB(NUM_VERTEX_PROGRAMS, ProgramID);

	UploadProgram(GL_VERTEX_PROGRAM_ARB, ProgramID[VERTEX_GENERIC], "generic.tvp");
}

void BuildFragmentProgram(unsigned int *ProgramID)
{
	glGenProgramsARB(NUM_FRAGMENT_PROGRAMS, ProgramID);

	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_FORCECALC], "forcecalc.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_APPLYFORCE], "applyforce.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_APPLYVELOCITY], "applyvelocity.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_WEIGHTCOMBINE], "weightcombine.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_NORMALMAP], "normalmap.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_REFLECT], "reflect.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_BLURX], "gaussianblurx.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_BLURY], "gaussianblury.tfp");
	UploadProgram(GL_FRAGMENT_PROGRAM_ARB, ProgramID[FRAGMENT_COMBINE], "combine.tfp");
}

void InitWater(void)
{
	int i;
	unsigned char *buffer=(unsigned char *)malloc(WaterTexWidth*WaterTexHeight*4);

	memset(buffer, 127, WaterTexWidth*WaterTexHeight*4);

	glGenTextures(NUM_DYNAMIC_TEXTURES, DynamicTexture);

	for(i=0;i<NUM_DYNAMIC_TEXTURES;i++)
	{
		glBindTexture(GL_TEXTURE_2D, DynamicTexture[i]);

		if(i==DYNAMIC_TEXTURE_NORMALMAP)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, WaterTexWidth, WaterTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WaterTexWidth, WaterTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
		}
	}

	TexHeightInput=DynamicTexture[DYNAMIC_TEXTURE_HEIGHT_SOURCE];
	TexHeightOutput=DynamicTexture[DYNAMIC_TEXTURE_HEIGHT_TARGET];
	TexVelocityInput=DynamicTexture[DYNAMIC_TEXTURE_VELOCITY_SOURCE];
	TexVelocityOutput=DynamicTexture[DYNAMIC_TEXTURE_VELOCITY_TARGET];

	FREE(buffer);
}

void DrawScreenQuad(float size)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-size, size, -1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-size, -size, -1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(size, size, -1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(size, -size, -1.0f);
	glEnd();
}

void UpdateWater(void)
{
	unsigned int Temp;
	float TexelWidth=1.0f/(float)WaterTexWidth, TexelHeight=1.0f/(float)WaterTexHeight;

	Resize(WaterTexWidth, WaterTexHeight);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	switch(FlipState)
	{
		case 0:
			TexHeightInput=DynamicTexture[DYNAMIC_TEXTURE_HEIGHT_SOURCE];
			TexHeightOutput=DynamicTexture[DYNAMIC_TEXTURE_HEIGHT_TARGET];
			TexVelocityInput=DynamicTexture[DYNAMIC_TEXTURE_VELOCITY_SOURCE];
			TexVelocityOutput=DynamicTexture[DYNAMIC_TEXTURE_VELOCITY_TARGET];

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			glActiveTextureARB(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_2D, TexVelocityInput);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);
			FlipState=1;
			break;

		case 1:
			Temp=TexHeightInput;
			TexHeightInput=TexHeightOutput;
			TexHeightOutput=Temp;

			Temp=TexVelocityInput;
			TexVelocityInput=TexVelocityOutput;
			TexVelocityOutput=Temp;
			FlipState=2;
			break;

		case 2:
			Temp=TexHeightInput;
			TexHeightInput=TexHeightOutput;
			TexHeightOutput=Temp;

			Temp=TexVelocityInput;
			TexVelocityInput=TexVelocityOutput;
			TexVelocityOutput=Temp;
			FlipState=1;
			break;
	}

	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, TexelWidth, TexelHeight, 0.0f, 0.0f);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_FORCECALC]);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, TexHeightInput);

	DrawScreenQuad(1.0f);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_APPLYFORCE]);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, TexVelocityInput);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, DynamicTexture[DYNAMIC_TEXTURE_FORCE_TARGET]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);

	DrawScreenQuad(1.0f);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_APPLYVELOCITY]);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, TexHeightInput);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, TexVelocityOutput);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);

	DrawScreenQuad(1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, TexHeightInput);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_WEIGHTCOMBINE]);

	DrawScreenQuad(1.0f);

	//--- Draw whatever you want to interact with the water here
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, Swimmer);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	if(IsManualControl)
		glTranslatef(Position[0]*0.0078125f, Position[1]*0.0078125f, 0.0f);
	else
		glTranslatef((float)(sin(fTime)*cos(fTime*2.0f))*0.75f, (float)(cos(fTime)*cos(fTime*2.0f))*0.75f, 0.0f);

	DrawScreenQuad(0.125f);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	if(IsSpinner)
	{
		glBindTexture(GL_TEXTURE_2D, Spinner);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();

		if(IsSpinning)
			glRotatef(fTime*64.0f, 0.0f, 0.0f, 1.0f);

		DrawScreenQuad(1.0f);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}

	glDisable(GL_BLEND);
	//---

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, TexHeightOutput);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[FRAGMENT_NORMALMAP]);

	DrawScreenQuad(1.0f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, DynamicTexture[DYNAMIC_TEXTURE_NORMALMAP]);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, WaterTexWidth, WaterTexHeight);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	Resize(Width, Height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

//---Float image
void FlipImage(int Components, float *Data, int Width, int Height)
{
	int x, y, i, flipx, flipy;
	float *buffer=(float *)malloc(sizeof(float)*Width*Height*Components);

	for(y=0;y<Height;y++)
	{
		flipy=Height-1-y;

		for(x=0;x<Width;x++)
		{
			flipx=Width-1-x;

			for(i=0;i<Components;i++)
				buffer[Components*(flipy*Width+flipx)+i]=Data[Components*(y*Width+x)+i];
		}
	}

	memcpy(Data, buffer, sizeof(float)*Width*Height*Components);
	FREE(buffer);
}

void GetBlock(int Components, float *DataIn, float *DataOut, int StartX, int StartY, int InWidth, int OutWidth, int OutHeight)
{
	int x, y, i;

	for(y=0;y<OutHeight;y++)
	{
		for(x=0;x<OutWidth;x++)
		{
			for(i=0;i<Components;i++)
				DataOut[Components*(y*OutWidth+x)+i]=DataIn[Components*((StartY+y)*InWidth+(StartX+x))+i];
		}
	}
}

unsigned int FLOAT_UploadCubeVirtCross(char *filename)
{
	unsigned int TextureID;
	FILE *stream;
	float *Data, *buffer;
	//Fixed width and height *plugh*
	int Width=768, Height=1024;
	int FaceWidth, FaceHeight;

	if((stream=fopen(filename, "rb"))==NULL)
		return 0;

	Data=(float *)malloc(sizeof(float)*Width*Height*3);
	fread(Data, sizeof(float), Width*Height*3, stream);

	fclose(stream);

	FaceWidth=Width/3;
	FaceHeight=Height/4;

	buffer=(float *)malloc(sizeof(float)*FaceWidth*FaceHeight*3);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, TextureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GetBlock(3, Data, buffer, FaceWidth*0, FaceHeight*2, Width, FaceWidth, FaceHeight);
	FlipImage(3, buffer, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	GetBlock(3, Data, buffer, FaceWidth*2, FaceHeight*2, Width, FaceWidth, FaceHeight);
	FlipImage(3, buffer, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	GetBlock(3, Data, buffer, FaceWidth*1, FaceHeight*3, Width, FaceWidth, FaceHeight);
	FlipImage(3, buffer, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	GetBlock(3, Data, buffer, FaceWidth*1, FaceHeight*1, Width, FaceWidth, FaceHeight);
	FlipImage(3, buffer, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	GetBlock(3, Data, buffer, FaceWidth*1, FaceHeight*2, Width, FaceWidth, FaceHeight);
	FlipImage(3, buffer, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	GetBlock(3, Data, buffer, FaceWidth*1, FaceHeight*0, Width, FaceWidth, FaceHeight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB_FLOAT32_ATI, FaceWidth, FaceHeight, 0, GL_RGB, GL_FLOAT, buffer);

	FREE(buffer);
	FREE(Data);

	return TextureID;
}
//---