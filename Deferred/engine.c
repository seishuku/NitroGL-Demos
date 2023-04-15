/*
 * This code is a real freakin mess, but it works.
 */

#include <windows.h>
#include <stdio.h>
#include "opengl.h"
#include "math.h"
#include "overlay.h"
#include "font.h"
#include "3ds.h"
#include "image.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(x) ((char *)NULL+(x))
#endif

GLContext_t Context, PBuffer;

char *szAppName="OpenGL";

int Width=800, Height=600;
int WidthP2, HeightP2;
float s, t;

int Done=0, Key[256];

unsigned __int64 Frequency, StartTime, EndTime;
float fps=0.0f, fTimeStep, fTime=0.0f;
int Frames=0;

typedef enum
{
	TEXTURE_RENDERTARGET0,
	TEXTURE_RENDERTARGET1,
	TEXTURE_RENDERTARGET2,

	TEXTURE_ENVMAP,

	TEXTURE_BASE0,
	TEXTURE_NORMAL0,
	TEXTURE_BASE1,
	TEXTURE_NORMAL1,

	VERTEX_GENERIC,

	FRAGMENT_MRT,
	FRAGMENT_COMPOSITE,

	NUM_OBJECTS
} ObjectNames;

unsigned int Objects[NUM_OBJECTS];

float RotateX=0.0f, RotateY=0.0f, PanX=0.0f, PanY=0.0f, Zoom=-300.0f;

Model3DS_t Model0, Model1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
void Resize(int Width, int Height);
unsigned int UploadProgram(char *Filename);

unsigned __int64 rdtsc(void)
{
	__asm rdtsc;
}

unsigned __int64 GetFrequency(void)
{
	unsigned __int64 TimeStart, TimeStop, Frequency, StartTicks, StopTicks;

	QueryPerformanceFrequency((LARGE_INTEGER *)&Frequency);

	QueryPerformanceCounter((LARGE_INTEGER *)&TimeStart);

	StartTicks=rdtsc();

	for(;;)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&TimeStop);

		if(Frequency<(TimeStop-TimeStart))
		{
			StopTicks=rdtsc();
			break;
		}
	}

	return StopTicks-StartTicks;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	RECT Rect;

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

	SetRect(&Rect, 0, 0, Width, Height);
	AdjustWindowRect(&Rect, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_POPUPWINDOW, 0);

	Context.hWnd=CreateWindow(szAppName, szAppName, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_POPUPWINDOW|WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, Rect.right-Rect.left, Rect.bottom-Rect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(Context.hWnd, SW_SHOW);
	SetForegroundWindow(Context.hWnd);

	// Power of 2 width and height, for non power of 2 texture hack
	WidthP2=_NextPower2(Width);
	HeightP2=_NextPower2(Height);
	s=(float)Width/WidthP2;
	t=(float)Height/HeightP2;

	// Create the contexts needed
	if(!CreateContext(&Context, 1, 32, 24, 0, 0))
	{
		DestroyWindow(Context.hWnd);
		return -1;
	}

	// Create the PBuffer context, double buffered, and one aux buffer.
	if(!CreatePBuffer(&PBuffer, WidthP2, HeightP2, 1, 1, 128, 24, 0, 0, 1, 1))
	{
		DestroyContext(&Context);
		DestroyWindow(Context.hWnd);
		return -1;
	}

	if(!Init())
	{
		DestroyContext(&PBuffer);
		DestroyContext(&Context);
		DestroyWindow(Context.hWnd);
		return -1;
	}

	Resize(Width, Height);

	Frequency=GetFrequency();

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
			StartTime=rdtsc();
			Render();
			SwapBuffers(Context.hDC);
			EndTime=rdtsc();

			fTimeStep=(float)(EndTime-StartTime)/Frequency;
			fTime+=fTimeStep;

			if(Frames++>15)
			{
				fps=1.0f/fTimeStep;
				Frames=0;
			}
		}
	}

	Free3DS(&Model0);
	Free3DS(&Model1);

	DestroyContext(&PBuffer);
	DestroyContext(&Context);
	DestroyWindow(Context.hWnd);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static POINT old;
	POINT pos, delta;

	switch(uMsg)
	{
		case WM_CREATE:
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_DESTROY:
			break;

		case WM_SIZE:
			Width=LOWORD(lParam);
			Height=HIWORD(lParam);
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);
			ShowCursor(FALSE);

			GetCursorPos(&pos);
			old.x=pos.x;
			old.y=pos.y;
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			ShowCursor(TRUE);
			ReleaseCapture();
			break;

		case WM_MOUSEMOVE:
			GetCursorPos(&pos);

			if(!wParam)
			{
				old.x=pos.x;
				old.y=pos.y;
				break;
			}

			delta.x=pos.x-old.x;
			delta.y=old.y-pos.y;

			if(!delta.x&&!delta.y)
				break;

			SetCursorPos(old.x, old.y);

			switch(wParam)
			{
				case MK_LBUTTON:
					RotateX+=delta.x;
					RotateY-=delta.y;
					break;

				case MK_MBUTTON:
					PanX+=delta.x;
					PanY+=delta.y;
					break;

				case MK_RBUTTON:
					Zoom+=delta.y;
					break;
			}
			break;

		case WM_KEYDOWN:
			Key[wParam]=1;

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
			Key[wParam]=0;
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawModel3DS(Model3DS_t *Model)
{
	int i;

	for(i=0;i<Model->NumMesh;i++)
	{
		if(GL_ARB_vertex_buffer_object_Flag&&Model->Mesh[i].VBOID)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, Model->Mesh[i].VBOID);

			glVertexPointer(3, GL_FLOAT, sizeof(float)*14, BUFFER_OFFSET(0));
			glEnableClientState(GL_VERTEX_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			glTexCoordPointer(2, GL_FLOAT, sizeof(float)*14, BUFFER_OFFSET(sizeof(float)*3));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE1_ARB);
			glTexCoordPointer(3, GL_FLOAT, sizeof(float)*14, BUFFER_OFFSET(sizeof(float)*5));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glClientActiveTextureARB(GL_TEXTURE2_ARB);
			glTexCoordPointer(3, GL_FLOAT, sizeof(float)*14, BUFFER_OFFSET(sizeof(float)*8));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glNormalPointer(GL_FLOAT, sizeof(float)*14, BUFFER_OFFSET(sizeof(float)*11));
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

		glDisableClientState(GL_NORMAL_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE2_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void BuildVBO3DS(Model3DS_t *Model)
{
	int i, j;

	for(i=0;i<Model->NumMesh;i++)
	{
		int index=0;
		float *data=NULL;

		if(!Model->Mesh[i].NumVertex)
			continue;

		glGenBuffersARB(1, &Model->Mesh[i].VBOID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, Model->Mesh[i].VBOID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*Model->Mesh[i].NumVertex*14, NULL, GL_STATIC_DRAW_ARB);

		if(glGetError()==GL_NO_ERROR)
			data=(float *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

		if(data==NULL)
		{
			char temp[1024];

			sprintf(temp, "VBO data buffer memory map failed or out of memory for object: %s", Model->Mesh[i].Name);
			MessageBox(NULL, temp, NULL, MB_OK);

			glDeleteBuffersARB(1, &Model->Mesh[i].VBOID);
			Model->Mesh[i].VBOID=0;
			break;
		}

		for(j=0;j<Model->Mesh[i].NumVertex;j++)
		{
			data[index++]=Model->Mesh[i].Vertex[3*j+0];
			data[index++]=Model->Mesh[i].Vertex[3*j+1];
			data[index++]=Model->Mesh[i].Vertex[3*j+2];

			data[index++]=Model->Mesh[i].UV[2*j+0];
			data[index++]=Model->Mesh[i].UV[2*j+1];

			data[index++]=Model->Mesh[i].Tangent[3*j+0];
			data[index++]=Model->Mesh[i].Tangent[3*j+1];
			data[index++]=Model->Mesh[i].Tangent[3*j+2];

			data[index++]=Model->Mesh[i].Binormal[3*j+0];
			data[index++]=Model->Mesh[i].Binormal[3*j+1];
			data[index++]=Model->Mesh[i].Binormal[3*j+2];

			data[index++]=Model->Mesh[i].Normal[3*j+0];
			data[index++]=Model->Mesh[i].Normal[3*j+1];
			data[index++]=Model->Mesh[i].Normal[3*j+2];
		}

		glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void DrawSkyBoxInf(void)
{
	float w=0.0f;
	float scale=1.0f;

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 0.0f);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex4f(scale, -scale, -scale, w);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex4f(scale, -scale, scale, w);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex4f(scale, scale, scale, w);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex4f(scale, scale, -scale, w);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex4f(-scale, -scale, scale, w);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex4f(-scale, -scale, -scale, w);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex4f(-scale, scale, -scale, w);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex4f(-scale, scale, scale, w);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex4f(-scale, scale, -scale, w);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex4f(scale, scale, -scale, w);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex4f(scale, scale, scale, w);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex4f(-scale, scale, scale, w);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex4f(-scale, -scale, scale, w);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex4f(scale, -scale, scale, w);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex4f(scale, -scale, -scale, w);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex4f(-scale, -scale, -scale, w);
	glTexCoord3f(1.0f, -1.0f, 1.0f);
	glVertex4f(scale, -scale, scale, w);
	glTexCoord3f(-1.0f, -1.0f, 1.0f);
	glVertex4f(-scale, -scale, scale, w);
	glTexCoord3f(-1.0f, 1.0f, 1.0f);
	glVertex4f(-scale, scale, scale, w);
	glTexCoord3f(1.0f, 1.0f, 1.0f);
	glVertex4f(scale, scale, scale, w);
	glTexCoord3f(-1.0f, -1.0f, -1.0f);
	glVertex4f(-scale, -scale, -scale, w);
	glTexCoord3f(1.0f, -1.0f, -1.0f);
	glVertex4f(scale, -scale, -scale, w);
	glTexCoord3f(1.0f, 1.0f, -1.0f);
	glVertex4f(scale, scale, -scale, w);
	glTexCoord3f(-1.0f, 1.0f, -1.0f);
	glVertex4f(-scale, scale, -scale, w);
	glEnd();
}

void Render(void)
{
	// Defines for the buffers (Front, Back, and Aux0)
	GLenum DrawBuffers[4]={ GL_FRONT_LEFT, GL_BACK_LEFT, GL_AUX0 };
	float QuatX[4], QuatY[4], Quat[4];

	wglMakeCurrent(PBuffer.hDC, PBuffer.hRC);
	Resize(Width, Height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(PanX, PanY, Zoom);
	QuatAngle(RotateX, 0.0f, 1.0f, 0.0f, QuatX);
	QuatAngle(RotateY, 1.0f, 0.0f, 0.0f, QuatY);
	QuatMultiply(QuatX, QuatY, Quat);
	QuatMatrix(Quat, NULL);

	// Output the needed data, uses the ATI_draw_buffer extension to output to multiple buffers
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, Objects[VERTEX_GENERIC]);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[FRAGMENT_MRT]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glDrawBuffersATI(3, DrawBuffers);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_BASE0]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_NORMAL0]);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DrawModel3DS(&Model0);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_BASE1]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_NORMAL1]);
	DrawModel3DS(&Model1);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Objects[TEXTURE_ENVMAP]);

	glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
	DrawSkyBoxInf();

	glActiveTextureARB(GL_TEXTURE0_ARB);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);

	glPopMatrix();

	wglMakeCurrent(Context.hDC, Context.hRC);
	Resize(Width, Height);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	BeginOverlay(Width, Height);
		// The actual output display, the render targets contain the View vector,
		// normal vector, and color texture data.
		// Also bound is the enviroment map, which is sampled with a dependent texture
		// read in the composite fragment program (using the view and normal vectors).
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_RENDERTARGET0]);
		wglBindTexImageARB(PBuffer.hPbuffer, WGL_FRONT_LEFT_ARB);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_RENDERTARGET1]);
		wglBindTexImageARB(PBuffer.hPbuffer, WGL_BACK_LEFT_ARB);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_RENDERTARGET2]);
		wglBindTexImageARB(PBuffer.hPbuffer, WGL_AUX0_ARB);

		glActiveTextureARB(GL_TEXTURE3_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Objects[TEXTURE_ENVMAP]);

		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[FRAGMENT_COMPOSITE]);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, t);
			glVertex3i(0, Height, -1);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3i(0, 0, -1);
			glTexCoord2f(s, t);
			glVertex3i(Width, Height, -1);
			glTexCoord2f(s, 0.0f);
			glVertex3i(Width, 0, -1);
		glEnd();

		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		glActiveTextureARB(GL_TEXTURE2_ARB);
		wglBindTexImageARB(PBuffer.hPbuffer, WGL_AUX0_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		wglBindTexImageARB(PBuffer.hPbuffer, WGL_BACK_LEFT_ARB);
		glDisable(GL_TEXTURE_2D);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		wglReleaseTexImageARB(PBuffer.hPbuffer, WGL_FRONT_LEFT_ARB);
		glDisable(GL_TEXTURE_2D);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		Font_Print(0, 0, "FPS: %0.1f", fps);
	EndOverlay();
}

unsigned int Create2DTextureObject(unsigned int Wrap, unsigned int Filter)
{
	unsigned int TextureID;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter);

	return TextureID;
}

int Init(void)
{
	if(!GL_ATI_draw_buffers_Flag)
	{
		MessageBox(Context.hWnd, "Requires ATI_draw_buffers extension.", "Error", MB_OK);
		return 0;
	}

	if(!GL_ARB_vertex_program_Flag)
	{
		MessageBox(Context.hWnd, "Requires GL_ARB_vertex_program extension.", "Error", MB_OK);
		return 0;
	}

	if(!GL_ARB_fragment_program_Flag)
	{
		MessageBox(Context.hWnd, "Requires GL_ARB_fragment_program extension.", "Error", MB_OK);
		return 0;
	}

	wglMakeCurrent(PBuffer.hDC, PBuffer.hRC);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glFlush();
	glFinish();

	wglMakeCurrent(Context.hDC, Context.hRC);

	Objects[TEXTURE_RENDERTARGET0]=Create2DTextureObject(GL_CLAMP, GL_NEAREST);
	Objects[TEXTURE_RENDERTARGET1]=Create2DTextureObject(GL_CLAMP, GL_NEAREST);
	Objects[TEXTURE_RENDERTARGET2]=Create2DTextureObject(GL_CLAMP, GL_NEAREST);

	Objects[TEXTURE_ENVMAP]=Image_Upload("grace.tga", IMAGE_CUBEMAP_CROSS|IMAGE_CLAMP_EDGE);

	Objects[TEXTURE_BASE0]=Image_Upload("fieldstone.tga", IMAGE_MIPMAP|IMAGE_TRILINEAR);
	Objects[TEXTURE_NORMAL0]=Image_Upload("fieldstone_b.tga", IMAGE_MIPMAP|IMAGE_TRILINEAR|IMAGE_NORMALMAP);
	Objects[TEXTURE_BASE1]=Image_Upload("wood.tga", IMAGE_MIPMAP|IMAGE_TRILINEAR);
	Objects[TEXTURE_NORMAL1]=Image_Upload("wood_b.tga", IMAGE_MIPMAP|IMAGE_TRILINEAR|IMAGE_NORMALMAP);

	Objects[VERTEX_GENERIC]=UploadProgram("generic.tvp");
	Objects[FRAGMENT_MRT]=UploadProgram("mrt.tfp");
	Objects[FRAGMENT_COMPOSITE]=UploadProgram("composite.tfp");

	if(Load3DS(&Model0, "level.3ds"))
		BuildVBO3DS(&Model0);

	if(Load3DS(&Model1, "elephant.3ds"))
		BuildVBO3DS(&Model1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glFlush();
	glFinish();

	return 1;
}

void InfPerspective(float fovy, float aspect, float zNear)
{
	float y=(float)tan((fovy/2.0f)*3.14159f/180.0f)*zNear, x=aspect*y;
	float m[16], nudge=1.0f-1.0f/(1<<20);

	m[0]=(2.0f*zNear)/(x+x);
	m[1]=0.0f;
	m[2]=0.0f;
	m[3]=0.0f;
	m[4]=0.0f;
	m[5]=(2.0f*zNear)/(y+y);
	m[6]=0.0f;
	m[7]=0.0f;
	m[8]=(x-x)/(x+x);
	m[9]=(y-y)/(y+y);
	m[10]=-1.0f*nudge;
	m[11]=-1.0f;
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=-2.0f*zNear*nudge;
	m[15]=0.0f;

	glMultMatrixf(m);
}

void Resize(int Width, int Height)
{
	if(Height==0)
		Height=1;

	glViewport(0,0, Width, Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	InfPerspective(90.0f, (float)Width/Height, 0.01f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ParseString(char *String)
{
	int i;
	int Done=0, Comment=0;
	char Line[256], *SrcStringPtr, *DstStringPtr;

	SrcStringPtr=DstStringPtr=String;

	while(!Done)
	{
		for(i=0;;i++)
		{
			Line[i]=*SrcStringPtr++;

			if(Line[i]=='\0')
			{
				Done=1;
				break;
			}

			if(Line[i]=='\n'||Line[i]=='\r')
			{
				Line[i+1]='\0';
				break;
			}
		}

		if(!Done)
		{
			for(i=0;;i++)
			{
				if(Line[i]=='\0')
					break;

				if(Line[i]=='/'&&Line[i+1]=='*')
					Comment=1;

				if(Line[i]=='*'&&Line[i+1]=='/')
				{
					i+=2;
					Comment=0;
				}

				if(!Comment)
				{
					if((Line[i]=='/'&&Line[i+1]=='/')||Line[i]=='#')
						break;

					*DstStringPtr++=Line[i];
				}
			}
		}
		else
			*DstStringPtr++='\0';
	}
}

unsigned int UploadProgram(char *Filename)
{
	FILE *Stream;
	int ErrorPosition;
	char *Program;
	long Bytes;
	unsigned int ProgramID=0, Target=0;

	if(!(Stream=fopen(Filename, "rb")))
		return 0;

	fseek(Stream, 0, SEEK_END);
	Bytes=ftell(Stream);
	fseek(Stream, 0, SEEK_SET);

	Program=(char *)malloc(sizeof(char)*Bytes+1);
	Bytes=fread(Program, sizeof(char), Bytes, Stream);
	Program[Bytes]=0;

	fclose(Stream);

	ParseString(Program);

	if(!strncmp(Program, "!!ARBfp1.0", 10))
		Target=GL_FRAGMENT_PROGRAM_ARB;
	else
	{
		if(!strncmp(Program, "!!ARBvp1.0", 10))
			Target=GL_VERTEX_PROGRAM_ARB;
		else
			return 0;
	}

	glGenProgramsARB(1, &ProgramID);
	glBindProgramARB(Target, ProgramID);
	glProgramStringARB(Target, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(Program), Program);

	FREE(Program);

	glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &ErrorPosition);

	if(ErrorPosition==-1)
		return ProgramID;

	MessageBox(NULL, (const char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB), Filename, MB_OK);

	return 0;
}
