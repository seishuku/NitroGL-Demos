#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include "glati.h"
#include "tga.h"
#include "overlay.h"
#include "font.h"

HWND hWnd=NULL;
HDC hDC=NULL;
HGLRC hRC=NULL;

char *szAppName="3D Texturing";

RECT WindowRect;

int Width=800, Height=600;
BOOL Done=FALSE, Key[255];

LARGE_INTEGER timerFrequency;
LARGE_INTEGER startTime;
LARGE_INTEGER currentTime;
float fps;
int nFrames=0;
int nLastFrames=0;
char OutBuffer[255];

LARGE_INTEGER time_freq;
LARGE_INTEGER time_counter;
LARGE_INTEGER time;

int bbox=0;

float roll, pitch, yaw, zoom, i=1.0f;

float points[64][3];

unsigned int Shell, FontBase, FontTexture, Base;

typedef struct
{
	POINT pt;
	int x, y;
	int oldx, oldy;
	int dx, dy;
} mouse_t;

mouse_t mouse;

PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT=NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Render(void);
BOOL Init(void);
BOOL Create(void);
void Destroy(void);
void Resize(int Width, int Height);
BOOL IsExtensionSupported(const char *extension);
void DrawVolume(void);
unsigned int LoadVolume(char *filename);
unsigned int GenTex(void);

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
	wc.hbrBackground=GetStockObject(WHITE_BRUSH);
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
			if(Key[VK_UP])
			{
				i+=0.00625f;

				if(i>1.0f)
					i=1.0f;
			}

			if(Key[VK_DOWN])
			{
				i-=0.00625f;

				if(i<0.0f)
					i=0.0f;
			}

			Render();
			SwapBuffers(hDC);

			nFrames++;
			if(nFrames>20)
			{
				QueryPerformanceCounter(&currentTime);
				fps=(float)nFrames/((float)(currentTime.QuadPart - startTime.QuadPart)/(float)timerFrequency.QuadPart);
				sprintf(OutBuffer, "FPS: %0.1f", fps);
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

			if(wParam==(MK_LBUTTON|MK_CONTROL))
			{
				if(abs(mouse.dx)>abs(mouse.dy))
					roll-=mouse.dx;
				else
					roll-=mouse.dy;
			}

			if(wParam==MK_LBUTTON)
			{
				yaw+=mouse.dx;
				pitch+=mouse.dy;
			}

			if(wParam==MK_RBUTTON)
				zoom+=mouse.dy;
			break;

		case WM_KEYDOWN:
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_SPACE:
					bbox^=1;
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
	float xPlane[]={ 1.0f, 0.0f, 0.0f, 0.0f };
	float yPlane[]={ 0.0f, 1.0f, 0.0f, 0.0f };
	float zPlane[]={ 0.0f, 0.0f, 1.0f, 0.0f };

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, (zoom/16)-2.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(-roll, 0.0f, 0.0f, 1.0f);

	glBindTexture(GL_TEXTURE_3D_EXT, Base);
	glEnable(GL_TEXTURE_3D_EXT);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, xPlane);
	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, yPlane);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, zPlane);
	glEnable(GL_TEXTURE_GEN_R);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5f, 0.5f, 0.5f);
	glScalef(0.5f, 0.5f, 0.5f);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, i);
		DrawVolume();
	glPopMatrix();

	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	if(bbox)
	{
		glPushMatrix();
		glBegin(GL_LINES);
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);
		glEnd();
		glPopMatrix();
	}
 
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	BeginOverlay(Width, Height);
		glColor3f(1.0f, 1.0f, 1.0f);
		Font_Print(0, 32, FontBase, FontTexture, "Density: %0.4f", i);
		Font_Print(0, 16, FontBase, FontTexture, "%s", gluErrorString(glGetError()));
		Font_Print(0, 0, FontBase, FontTexture, OutBuffer);
	EndOverlay();
}

BOOL Init(void)
{
	int slices=64;

	glTexImage3DEXT=(PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress("glTexImage3DEXT");

	if(glTexImage3DEXT==NULL)
	{
		MessageBox(hWnd, "GL_EXT_texture3D not found!", "OpenGL", MB_OK);
		return FALSE;
	}

	FontBase=Font_Build();
	FontTexture=TGA_UploadImage("font.tga");

	Base=LoadVolume("vol.raw");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Shell=glGenLists(1);
	glNewList(Shell, GL_COMPILE);
	{
		int ii, gg, hh;
		float temp;

		for(ii=0;ii<8;ii++)
		{
			for(gg=0;gg<8;gg++)
			{
				points[ii*8+gg][0]=(float)gg/7.0f-0.5f;
				points[ii*8+gg][1]=(float)ii/7.0f-0.5f;
				points[ii*8+gg][2]=-0.5f;
				temp=points[ii*8+gg][0]*points[ii*8+gg][0]+points[ii*8+gg][1]*points[ii*8+gg][1]+points[ii*8+gg][2]*points[ii*8+gg][2];
				temp=1.0f/(float)sqrt(temp);
				points[ii*8+gg][0]*=temp;
				points[ii*8+gg][1]*=temp;
				points[ii*8+gg][2]*=temp;
				points[ii*8+gg][0]*=2.0f;
				points[ii*8+gg][1]*=2.0f;
				points[ii*8+gg][2]+=1.0f;
			}
		}

		for(ii=0;ii<slices;ii++)
		{
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, -1.0f+(float)ii*2.0f/(float)(slices-1));
			glBegin(GL_QUADS);
			for(hh=0;hh<7;hh++)
			{
				for(gg=0;gg<7;gg++)
				{
					glVertex3fv(points[hh*8+gg]);
					glVertex3fv(points[hh*8+gg+1]);
					glVertex3fv(points[(hh+1)*8+gg+1]);
					glVertex3fv(points[(hh+1)*8+gg]);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
	glEndList();

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
	gluPerspective(90.0f, (float)Width/Height, 0.001f, 400.0f);
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

void DrawVolume(void)
{
	double posx[]={ 1.0f, 0.0f, 0.0f, 1.0f };
	double negx[]={ -1.0f, 0.0f, 0.0f, 1.0f };
	double posy[]={ 0.0f, 1.0f, 0.0f, 1.0f };
	double negy[]={ 0.0f, -1.0f, 0.0f, 1.0f };
	double posz[]={ 0.0f, 0.0f, 1.0f, 1.0f };
	double negz[]={ 0.0f, 0.0f, -1.0f, 1.0f };
	float out[16]={ 1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f }, in[16];

	glClipPlane(GL_CLIP_PLANE0, posx);
	glEnable(GL_CLIP_PLANE0);

	glClipPlane(GL_CLIP_PLANE1, negx);
	glEnable(GL_CLIP_PLANE1);

	glClipPlane(GL_CLIP_PLANE2, posy);
	glEnable(GL_CLIP_PLANE2);

	glClipPlane(GL_CLIP_PLANE3, negy);
	glEnable(GL_CLIP_PLANE3);

	glClipPlane(GL_CLIP_PLANE4, posz);
	glEnable(GL_CLIP_PLANE4);

	glClipPlane(GL_CLIP_PLANE5, negz);
	glEnable(GL_CLIP_PLANE5);

	glGetFloatv(GL_MODELVIEW_MATRIX, in);
	out[0]=in[0];	out[1]=in[4];	out[2]=in[8];
	out[4]=in[1];	out[5]=in[5];	out[6]=in[9];
	out[8]=in[2];	out[9]=in[6];	out[10]=in[10];
	glMultMatrixf(out);

	glCallList(Shell);

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);
}

void getRamp(unsigned char *color, float t)
{
	t*=2.0f;

	color[0]=(unsigned char)(255.0f*(float)sin(t*3.1415927f*0.5f));
	color[1]=(unsigned char)(255.0f*(float)sin(t*3.1415927f));
	color[2]=(unsigned char)(255.0f*(float)fabs(sin(t*3.1415927f*2.0f)));
	color[3]=(unsigned char)(255.0f*(float)sin(t*3.1415927f*0.5f));
}

unsigned int LoadVolume(char *filename)
{
	unsigned int TextureID;
	unsigned short avg;
	unsigned int max=0;
	int width=64, height=64, depth=64, x, y, z;
	unsigned char *buffer, *buffer2;
	FILE *stream;

	buffer=malloc(width*height*depth);
	buffer2=malloc(width*height*depth*4);

	memset(buffer2, 0, width*height*depth*4);

	if((stream=fopen(filename, "rb"))==NULL)
		return 0;

	fread(buffer, sizeof(unsigned char), width*height*depth, stream);

	fclose(stream);

	for(z=0;z<depth;z++)
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				if(max<buffer[(z*width*width+y*width+x)])
					max=buffer[(z*width*width+y*width+x)];
			}
		}
	}

	for(z=0;z<depth;z++)
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<width;x++)
			{
				if(x==0||x==width-1||y==0||y==height-1||z==0||z==depth-1)
				{
					buffer2[4*(z*width*width+y*width+x)]=0;
					buffer2[4*(z*width*width+y*width+x)+1]=0;
					buffer2[4*(z*width*width+y*width+x)+2]=0;
					buffer2[4*(z*width*width+y*width+x)+3]=0;
				}
				else
				{
					unsigned char color[4];

					avg=(buffer[(z*width*width+y*width+x)]+buffer[(z*width*width+y*width+(x+1))]+buffer[(z*width*width+(y+1)*width+(x+1))]+buffer[(z*width*width+(y+1)*width+x)])>>2;

					getRamp(color, (float)avg/max);

					buffer2[4*(z*width*width+y*width+x)]=color[0];
					buffer2[4*(z*width*width+y*width+x)+1]=color[1];
					buffer2[4*(z*width*width+y*width+x)+2]=color[2];
					buffer2[4*(z*width*width+y*width+x)+3]=color[3];
				}
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_3D_EXT, TextureID);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R_EXT, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_RGBA8, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer2);
//	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_ALPHA8, width, height, depth, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);

	free(buffer);
	free(buffer2);

	return TextureID;
}

unsigned int GenTex(void)
{
	float kCloseClip=0.1f;
	float kFarClip=520.0f;
	float kMaxFocusDistance=20000.0;
	int kConfusionLookupWidth=256, kConfusionLookupHeight=128, kConfusionLookupDepth=32;
	float kMinFocusDistance=20000.0f/128;
	float kMinFocalLength=3.0f;
	float kMaxFocalLength=60.0f;
	float kFilmDimension=3.6f;
	float kBlurFactor=1.5f;
	float mFStop=1.0f;
	int kNumOfFilterSteps=5;
	unsigned int TextureID;
	unsigned char *buffer=(unsigned char *)malloc(kConfusionLookupWidth*kConfusionLookupHeight*kConfusionLookupDepth);
	float inputx, inputy, inputz;
	float distance, focusDistance, focalLength, minDistance, maxDistance;
	float interpolator, circle;
	float hyperFocal, denominator;
	float c0=kFilmDimension/((float)max(kConfusionLookupWidth, kConfusionLookupHeight));
	float ratio1=(float)pow(kBlurFactor, kNumOfFilterSteps);
	float ratio2=(float)pow(kBlurFactor, 2.0f*kNumOfFilterSteps);
	int x, y, z;

	for(z=0;z<kConfusionLookupDepth;z++)
	{
		inputz=(float)z/(kConfusionLookupDepth-1);
		focalLength=(kMaxFocalLength-kMinFocalLength)*inputz+kMinFocalLength;

		for(y=0;y<kConfusionLookupHeight;y++)
		{
			inputy=(float)y/(kConfusionLookupHeight-1);
			focusDistance=(kMaxFocusDistance-kMinFocusDistance)*inputy+kMinFocusDistance;
			hyperFocal=focalLength*focalLength/(mFStop*ratio2*c0);
			denominator=hyperFocal-(focusDistance-focalLength);
			minDistance=0.01f*hyperFocal*focusDistance/(hyperFocal+focusDistance-focalLength);
			minDistance=max(minDistance, kCloseClip);
			maxDistance=kFarClip;

			if(denominator>0.0f)
				maxDistance=min(maxDistance, 0.01f*focusDistance*(hyperFocal/denominator));

			if(minDistance>=maxDistance)
				maxDistance=minDistance+minDistance-maxDistance;

			for(x=0;x<kConfusionLookupWidth;x++)
			{
				inputx=(float)x/(kConfusionLookupWidth-1);
				distance=(maxDistance-minDistance)*inputx+minDistance;
				distance=100.0f*distance;
				circle=(focusDistance/distance-1.0f)*focalLength*focalLength;
				circle=circle/(mFStop*(focusDistance-focalLength));
				circle=(float)fabs(circle);

				if(circle<=c0)
					interpolator=0.0f;
				else
					if(circle<=ratio1*c0)
					{
						interpolator=(circle-c0)/((ratio1-1.0f)*c0);
						interpolator=max(0.0f, interpolator);
						interpolator=min(1.0f, interpolator);
						interpolator=0.5f*interpolator;
					}
					else
					{
						interpolator=(circle-ratio1*c0)/((ratio2-ratio1)*c0);
						interpolator=max(0.0f, interpolator);
						interpolator=min(1.0f, interpolator);
						interpolator=0.5f*(1.0f+interpolator); 
					}

				buffer[x+y*kConfusionLookupWidth+z*kConfusionLookupWidth*kConfusionLookupDepth]=(unsigned char)(interpolator*255.0f)&0xff;
			}
		}
	}

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_3D_EXT, TextureID);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_WRAP_R_EXT, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3DEXT(GL_TEXTURE_3D_EXT, 0, GL_LUMINANCE8, kConfusionLookupWidth, kConfusionLookupHeight, kConfusionLookupDepth, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);

	free(buffer);

	return TextureID;
}
