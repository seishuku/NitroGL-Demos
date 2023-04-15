#include <windows.h>
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
#include "camera.h"
#include "quat.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

HPBUFFERARB hPbuffer=NULL;
HDC hPbufferDC=NULL;
HGLRC hPbufferRC=NULL;

char *szAppName="OpenGL";

RECT WindowRect;

int Width=800, Height=600;

int DynWidth=256, DynHeight=256;

int Done=0, Key[256], Object=0;

LARGE_INTEGER timer_freq, startTime, currentTime, time_counter, time_current;
float fps=0.0f, fTime=0.0f;
int Frames=0;

unsigned int Base, Bump, DynCubemap, FontTexture;
unsigned int FragmentProgram[2], VertexProgram[1];

TDS_Model_t Model[3];

Camera_t Camera;

float Light[4]={ 0.0f, 0.0f, 40.0f, 1.0f }, LightRadius=1.0f/384.0f;
float Kd[3]={ 1.0f, 1.0f, 1.0f };
float Ks[3]={ 1.0f, 1.0f, 1.0f }, SpecularExpoent=8.0f;
float Ka[3]={ 0.0f, 0.0f, 0.0f };

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

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB=NULL;

PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB=NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB=NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB=NULL;

PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB=NULL;
PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB=NULL;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB=NULL;
PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB=NULL;
PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB=NULL;

PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB=NULL;
PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB=NULL;
PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
int CreateContext(HWND hWnd, HDC *hDC, HGLRC *hRC, int Color, int Depth, int Stencil, int SampleBuffers);
int CreatePbufferContext(HPBUFFERARB *hPbuffer, HDC *hDC, HGLRC *hRC, int Width, int Height, int Color, int Depth, int Stencil, unsigned long RenderTexture, int IsRenderTextureMipmap, unsigned long *TextureID);
void DestroyContext(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DestroyPbufferContext(HPBUFFERARB *hPbuffer, HDC *hDC, HGLRC *hRC);
void SetCubeRenderFace(HPBUFFERARB hPbuffer, int Face);
void Resize(int Width, int Height);
int IsExtensionSupported(const char *extension);
void BuildVertexProgram(unsigned int *ShaderID);
void BuildFragmentProgram(unsigned int *ShaderID);
void RenderDynamicCubeMap(unsigned int TextureID);

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

	if(!CreateContext(hWnd, &hDC, &hRC, 32, 24, 8, (MessageBox(hWnd, "Enable FSAA (if supported)?", "OpenGL", MB_ICONQUESTION|MB_YESNO)==IDYES)?4:0))
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

	TDS_Delete(&Model[0]);
	TDS_Delete(&Model[1]);

	DestroyContext(hWnd, &hDC, &hRC);
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

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_SPACE:
					Object^=1;
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
	float fTimeStep, IOR=1.1f, WaveSplit=0.0625f;
	unsigned short i;

	time_current=time_counter;
	QueryPerformanceCounter(&time_counter);

	fTimeStep=(float)(time_counter.QuadPart-time_current.QuadPart)/timer_freq.QuadPart;
	fTime+=fTimeStep;

	Light[0]=(float)sin(fTime)*75.0f;
	Light[2]=(float)cos(fTime)*75.0f;

	Resize(DynWidth, DynHeight);

	for(i=0;i<6;i++)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		switch(i)
		{
			case 0:
				glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;

			case 1:
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;

			case 2:
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
				break;

			case 3:
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				break;

			case 4:
				glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
				break;

			case 5:
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				break;
		}

		glBegin(GL_POINTS);
		glColor3fv(Kd);
		glVertex3f(Light[0], Light[1], Light[2]);
		glEnd();

		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexProgram[0]);
		glEnable(GL_VERTEX_PROGRAM_ARB);

		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[0]); // Bind the Phong lighting shader
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, Light[0], Light[1], Light[2], LightRadius);
		glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, Kd[0], Kd[1], Kd[2], 1.0f);
	 	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, Ks[0], Ks[1], Ks[2], SpecularExpoent);
		glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, Ka[0], Ka[1], Ka[2], 1.0f);

		TDS_Render(&Model[0], 1, 0, 0);

		glDisable(GL_FRAGMENT_PROGRAM_ARB);
		glDisable(GL_VERTEX_PROGRAM_ARB);

		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, DynCubemap);
		glCopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i, 0, 0, 0, 0, 0, DynWidth, DynHeight);
	}

	Resize(Width, Height);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	CameraUpdate(&Camera, fTimeStep);

	for(i=0;i<Model[0].NumMeshes;i++)
		CameraCheckCollision(&Camera, Model[0].Mesh[i].Vertex, Model[0].Mesh[i].Face, Model[0].Mesh[i].NumFace);

	glPushMatrix();
	LookAt(Camera.Position, Camera.View, Camera.UpVector);

	glBegin(GL_POINTS);
	glColor3fv(Kd);
	glVertex3f(Light[0], Light[1], Light[2]);
	glEnd();

	// Render the main room
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexProgram[0]);
	glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, 0.5f, 0.0f, 0.0f, 0.0f); // Tangent scaling (controls how "bumpy" it is)

	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[0]); // Bind the Phong lighting shader
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, Light[0], Light[1], Light[2], LightRadius);
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1, Kd[0], Kd[1], Kd[2], 1.0f);
 	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 2, Ks[0], Ks[1], Ks[2], SpecularExpoent);
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 3, Ka[0], Ka[1], Ka[2], 1.0f);

	TDS_Render(&Model[0], 1, 0, 0);

	// Render the reflective and refractive object
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, VertexProgram[0]);
	glProgramEnvParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, 0.125f, 0.0f, 0.0f, 0.0f); // Tangent scaling (controls how "bumpy" it is)

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, FragmentProgram[1]); // Bind the reflection and refraction shader
	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, IOR, IOR+WaveSplit, IOR+(WaveSplit*2.0f), 0.0f); // Set refraction parameters (Index Of Refraction and Wavelength Split)

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, DynCubemap);

	if(Object)
		TDS_Render(&Model[1], 1, Base, Bump);
	else
		TDS_Render(&Model[2], 1, Base, Bump);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);

	glPopMatrix();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Font_Print(0, 16, FontTexture, "%s\nFPS: %0.1f", gluErrorString(glGetError()), fps);
	EndOverlay();
}

int CheckR9700ICDVer(void)
{
	const char *Version=glGetString(GL_VERSION);
	const char *Renderer=glGetString(GL_RENDERER);

	// If it isn't a Radeon 9700
	if(!strcmp("Radeon 9700", Renderer))
		return 0;

	// If it is a Radeon 9700, what version/build ICD?
	if(strcmp("1.3.3413", Version))
		return 1;

	return 0;
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

	if(!IsExtensionSupported("GL_ATI_vertex_array_object"))
	{
		MessageBox(hWnd, "GL_ATI_vertex_array_object not found!", "OpenGL", MB_OK);
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

	FontTexture=IMAGE_UploadImage("font.tga");

	Base=IMAGE_UploadImage("white.tga");
	Bump=IMAGE_UploadDOT3("bumpfloor.tga");

	TDS_LoadModel(&Model[0], "room.3ds");
	TDS_LoadModel(&Model[1], "teapot.3ds");
	TDS_LoadModel(&Model[2], "sphere.3ds");

	glGenTextures(1, &DynCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, DynCubemap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB8, DynWidth, DynHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	BuildVertexProgram(VertexProgram);
	BuildFragmentProgram(FragmentProgram);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPointSize(16.0f);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	CameraInit(&Camera);

	return TRUE;
}

int CreateContext(HWND hWnd, HDC *hDC, HGLRC *hRC, int Color, int Depth, int Stencil, int SampleBuffers)
{
	HWND hWndTemp=NULL;
	HDC hDCTemp=NULL;
	HGLRC hRCTemp=NULL;
	PIXELFORMATDESCRIPTOR pfd;
	BOOL IsMultisample=TRUE;
	unsigned int PixelFormat=-1;
	unsigned int NumFormats;
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

int CreatePbufferContext(HPBUFFERARB *hPbuffer, HDC *hDC, HGLRC *hRC, int Width, int Height, int Color, int Depth, int Stencil, unsigned long RenderTexture, int IsRenderTextureMipmap, unsigned long *TextureID)
{
	HDC hDCTemp=NULL;
	int Attrib[128], i=0, PixelFormat;
	unsigned int Formats;

	hDCTemp=wglGetCurrentDC();

	if(!IsExtensionSupported("WGL_ARB_pbuffer"))
	{
		MessageBox(hWnd, "WGL_ARB_pbuffer not found!", "OpenGL", MB_OK);
		return 0;
	}
	else
	{
		wglCreatePbufferARB=(PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB=(PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB=(PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB=(PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
		wglQueryPbufferARB=(PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
	}

	if(RenderTexture)
	{
		if(!IsExtensionSupported("WGL_ARB_render_texture"))
		{
			MessageBox(hWnd, "WGL_ARB_render_texture not found!", "OpenGL", MB_OK);
			return 0;
		}
		else
		{
			wglBindTexImageARB=(PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
			wglReleaseTexImageARB=(PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
			wglSetPbufferAttribARB=(PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");
		}
	}

	Attrib[i++]=WGL_DRAW_TO_PBUFFER_ARB;
	Attrib[i++]=1;
	Attrib[i++]=WGL_SUPPORT_OPENGL_ARB;
	Attrib[i++]=1;
	Attrib[i++]=WGL_PIXEL_TYPE_ARB;
	Attrib[i++]=WGL_TYPE_RGBA_ARB;
	Attrib[i++]=WGL_COLOR_BITS_ARB;
	Attrib[i++]=Color;
	Attrib[i++]=WGL_DEPTH_BITS_ARB;
	Attrib[i++]=Depth;
	Attrib[i++]=WGL_STENCIL_BITS_ARB;
	Attrib[i++]=Stencil;

	if(RenderTexture)
	{
		Attrib[i++]=WGL_BIND_TO_TEXTURE_RGBA_ARB;
		Attrib[i++]=1;
	}

	Attrib[i++]=0;
	Attrib[i++]=0;

	if(!wglChoosePixelFormatARB(hDCTemp, Attrib, NULL, 1, &PixelFormat, &Formats))
	{
		MessageBox(hWnd, "wglChoosePixelFormatARB Failed!", "OpenGL", MB_OK);
		return 0;
	}

	i=0;

	if(RenderTexture)
	{
		Attrib[i++]=WGL_TEXTURE_FORMAT_ARB;
		Attrib[i++]=WGL_TEXTURE_RGBA_ARB;
		Attrib[i++]=WGL_TEXTURE_TARGET_ARB;
		Attrib[i++]=RenderTexture;
		Attrib[i++]=WGL_MIPMAP_TEXTURE_ARB;
		Attrib[i++]=IsRenderTextureMipmap;
	}

	Attrib[i++]=0;
	Attrib[i++]=0;

	if(!(*hPbuffer=wglCreatePbufferARB(hDCTemp, PixelFormat, Width, Height, Attrib)))
	{
		MessageBox(NULL, "wglCreatePbufferARB Failed!", "OpenGL", MB_OK);
		return 0;
	}

	if(!(*hDC=wglGetPbufferDCARB(*hPbuffer)))
	{
		MessageBox(NULL, "wglGetPbufferDCARB Failed!", "OpenGL", MB_OK);
		return 0;
	}

	if(!(*hRC=wglCreateContext(*hDC)))
	{
		MessageBox(NULL, "wglCreateContext Failed!", "OpenGL", MB_OK);
		return 0;
	}

	if(TextureID!=NULL)
	{
		glGenTextures(1, TextureID);

		switch(RenderTexture)
		{
			case WGL_TEXTURE_1D_ARB:
				glBindTexture(GL_TEXTURE_1D, *TextureID);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IsRenderTextureMipmap)
					glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				else
					glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;

			case WGL_TEXTURE_2D_ARB:
				glBindTexture(GL_TEXTURE_2D, *TextureID);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IsRenderTextureMipmap)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;

			case WGL_TEXTURE_CUBE_MAP_ARB:
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, *TextureID);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if(IsRenderTextureMipmap)
					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				else
					glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;

			default:
				glDeleteTextures(1, TextureID);
				break;
		}
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

void DestroyPbufferContext(HPBUFFERARB *hPbuffer, HDC *hDC, HGLRC *hRC)
{
	if(hPbuffer==NULL)
		return;

	if(hRC==NULL)
		return;

	if(hDC==NULL)
		return;

	wglDeleteContext(*hRC);
	wglReleasePbufferDCARB(*hPbuffer, *hDC);
	wglDestroyPbufferARB(*hPbuffer);
}

void SetCubeRenderFace(HPBUFFERARB hPbuffer, int Face)
{
	int Attrib[]={ WGL_CUBE_MAP_FACE_ARB, WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+Face, 0, 0 };

	wglSetPbufferAttribARB(hPbuffer, Attrib);
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
				return 1;

		start=terminator;
	}

	extensions=NULL;

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

void CheckProgramError(char *title)
{
	int errorposition;

	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &errorposition);

	if(errorposition==-1)
		return;

	MessageBox(hWnd, glGetString(GL_PROGRAM_ERROR_STRING_ARB), title, MB_OK);
}

void BuildVertexProgram(unsigned int *ProgramID)
{
	char *Program;

	Program=read_text_file("generic.tvp");

	glGenProgramsARB(1, &ProgramID[0]);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, ProgramID[0]);
	glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	CheckProgramError("Vertex Program - generic.tvp");

	free(Program);
}

void BuildFragmentProgram(unsigned int *ProgramID)
{
	char *Program;

	Program=read_text_file("phong.tfp");

	glGenProgramsARB(1, &ProgramID[0]);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, ProgramID[0]);
	glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	CheckProgramError("Fragment Program - phong.tfp");

	free(Program);

	Program=read_text_file("reflrefr.tfp");

	glGenProgramsARB(1, &ProgramID[1]);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, ProgramID[1]);
	glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	CheckProgramError("Fragment Program - reflrefr.tfp");

	free(Program);
}
