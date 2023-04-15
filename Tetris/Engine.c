7#ifdef WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/keysym.h>
#endif

#include <AL/al.h>
#include <AL/alc.h>
#include "opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include <malloc.h>
#include "image.h"
#include "overlay.h"
#include "font.h"
#include "3ds.h"
#include "Game.h"
#include "Tremor/ivorbiscodec.h"
#include "Tremor/ivorbisfile.h"

#ifndef FREE
#define FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(x) ((char *)NULL+(x))
#endif

#ifdef __linux__
Display *dpy;
#endif

#ifdef __GNUC__
#ifndef __int64
#define __int64 long long
#endif
#endif

GLContext_t Context;

ALCcontext *ALContext=NULL;
ALCdevice *ALDevice=NULL;

char *szAppName="OpenGL";

int Width=600, Height=800;
int WidthP2, HeightP2;

int Done=0, Key[256], SwitchMode=0, SwitchObject=0, ToggleMusic=1;

unsigned __int64 Frequency, StartTime, EndTime;
float FPS, FPSTemp=0.0f, fTimeStep, fTime=0.0f;
int Frames=0;
const float OneOver15=1.0f/15.0f;

typedef enum
{
	TEXTURE_REFLECT,
	TEXTURE_SPECULAR,

	TEXTURE_ORIGINAL,
	TEXTURE_BLUR0,
	TEXTURE_BLUR1,

	BUFFER_DEPTH,
	BUFFER_ORIGINAL,
	BUFFER_BLUR,

	VERTEX_GENERIC,

	FRAGMENT_REFLECT,
	FRAGMENT_LIGHTING,
	FRAGMENT_DIFFUSE,
	FRAGMENT_BLUR,
	FRAGMENT_COMPOSITE,

	SOUND_LINE,
	SOUND_LINE4,
	SOUND_DROP,
	SOUND_MOVE,
	SOUND_ROTATE,
	SOUND_LEVELUP,

	NUM_OBJECTS
} ObjectNames;

unsigned int Objects[NUM_OBJECTS];

float RotateX, RotateY, PanX, PanY, Zoom=-250.0f;

Model3DS_t Model[3];

float Exposure=5.0f;

float Matrix[12][4];

Tetris_t Tetris;

typedef struct
{
	char String[256];
} String_t;

String_t *MusicList=NULL, *SceneList=NULL;
int NumMusic=0, NumScene=0;
int CurrentScene=0;
int CurrentMusic=0;

#define BUFFER_SIZE (4096*8)

FILE *oggFile;
OggVorbis_File oggStream;
vorbis_info *oggInfo=NULL;

unsigned int MusicBuffer[2];
unsigned int MusicSource;

void Render(void);
int Init(void);
void Resize(int Width, int Height);
unsigned int Create2DTextureObject(unsigned int FilterMode, unsigned int WrapMode);
void LoadScene(char *Filename);
unsigned int UploadProgram(char *Filename);
int InitSound(void);
unsigned int LoadStaticSound(char *Filename, int Loop);

int randrange(int min, int max)
{
	return (int)(((float)rand()/RAND_MAX)*(max+1)+min);
}

void Error(char *string)
{
#ifdef WIN32
	MessageBox(NULL, string, "Error", MB_OK);
#else
	fprintf(stderr, "Error: %s\n", string);
#endif
}

int Stream(unsigned int buffer)
{
	char data[BUFFER_SIZE];
	int size=0;

	while(size<BUFFER_SIZE)
	{
		int section, result;

		result=ov_read(&oggStream, data+size, BUFFER_SIZE-size, &section);

		if(result>0)
			size+=result;
		else
			break;
	}

	if(size==0)
		return 0;

	alBufferData(buffer, oggInfo->channels==1?AL_FORMAT_MONO16:AL_FORMAT_STEREO16, data, size, oggInfo->rate);

	return 1;
}

int Update(void)
{
	int processed;
	int active=1;

	if(ToggleMusic)
	{
		ALenum state;

		alGetSourcei(MusicSource, AL_SOURCE_STATE, &state);

		if(state!=AL_PLAYING)
			alSourcePlay(MusicSource);
	}

	alGetSourcei(MusicSource, AL_BUFFERS_PROCESSED, &processed);

	while(processed--)
	{
		unsigned int buffer;

		alSourceUnqueueBuffers(MusicSource, 1, &buffer);
		active=Stream(buffer);
		alSourceQueueBuffers(MusicSource, 1, &buffer);
	}

	return active;
}

int Playback(void)
{
	ALenum state;

	alGetSourcei(MusicSource, AL_SOURCE_STATE, &state);

	if(state==AL_PLAYING)
		return 1;

	if(!Stream(MusicBuffer[0]))
		return 0;

	if(!Stream(MusicBuffer[1]))
		return 0;

	alSourceQueueBuffers(MusicSource, 2, MusicBuffer);
	alSourcePlay(MusicSource);

	return 1;
}

int MusicOpen(char *path)
{
	int result;

	if(!(oggFile=fopen(path, "rb")))
		return 0;

	if((result=ov_open(oggFile, &oggStream, NULL, 0))<0)
	{
		fclose(oggFile);
		return 0;
	}

	oggInfo=ov_info(&oggStream, -1);

	alGenBuffers(2, MusicBuffer);
	alGenSources(1, &MusicSource);
	alSourcef(MusicSource, AL_PITCH, 1.0f);

	return 1;
}

void MusicClose(void)
{
	if(oggFile!=NULL)
	{
		alSourceStop(MusicSource);

		ov_clear(&oggStream);

		fclose(oggFile);
		oggFile=NULL;

		alDeleteSources(1, &MusicSource);
		alDeleteBuffers(2, MusicBuffer);
	}
}

#ifdef WIN32
String_t *BuildFileList(char *DirName, char *Filter, int *NumFiles)
{
	HANDLE hList;
	char szDir[MAX_PATH+1];
	WIN32_FIND_DATA FileData;
	String_t *Ret=NULL;

	Ret=(String_t *)malloc(sizeof(String_t));

	sprintf(szDir, "%s\\*", DirName);

	if((hList=FindFirstFile(szDir, &FileData))==INVALID_HANDLE_VALUE)
		return NULL;

	for(;;)
	{
		if(!(FileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			char *ptr=strrchr(FileData.cFileName, '.');

			if(ptr!=NULL)
			{
				if(!strcmp(ptr, Filter))
				{
					Ret=(String_t *)realloc(Ret, sizeof(String_t)*(*NumFiles+1));
					sprintf(Ret[(*NumFiles)++].String, "%s", FileData.cFileName);
				}
			}
		}

		if(!FindNextFile(hList, &FileData))
		{
			if(GetLastError()==ERROR_NO_MORE_FILES)
				break;
		}
	}

	FindClose(hList);

	return Ret;
}
#endif

#ifdef __linux__
#include <dirent.h>
#include <sys/stat.h>

String_t *BuildFileList(char *DirName, char *Filter, int *NumFiles)
{
	DIR *dp;
	struct dirent *dir_entry;
	struct stat stat_info;
	String_t *Ret=NULL;

	if((dp=opendir(DirName))==NULL)
		return NULL;

	Ret=(String_t *)malloc(sizeof(String_t));

	while((dir_entry=readdir(dp))!=NULL)
	{
		lstat(dir_entry->d_name, &stat_info);

		if(!S_ISDIR(stat_info.st_mode))
		{
			char *ptr=strrchr(dir_entry->d_name, '.');

			if(ptr!=NULL)
			{
				if(!strcmp(ptr, Filter))
				{
					Ret=(String_t *)realloc(Ret, sizeof(String_t)*(*NumFiles+1));
					sprintf(Ret[(*NumFiles)++].String, "%s", dir_entry->d_name);
				}
			}
		}
	}

	closedir(dp);

	return Ret;
}
#endif

unsigned __int64 rdtsc(void)
{
	unsigned long l, h;

#ifdef __GNUC__
	__asm__ __volatile__ ("rdtsc" : "=a" (l), "=d" (h));
#else
	__asm
	{
		rdtsc
		mov l, eax
		mov h, edx
	}
#endif

	return ((unsigned __int64)l|((unsigned __int64)h<<32));
}

#ifdef __linux__
#include <sys/time.h>
unsigned __int64 GetFrequency(void)
{
	unsigned __int64 StartTicks, StopTicks;
	struct timeval TimeStart, TimeStop;
	volatile unsigned long i;

	gettimeofday(&TimeStart, NULL);
	StartTicks=rdtsc();

	for(i=0;i<1000000;i++);

	StopTicks=rdtsc();
	gettimeofday(&TimeStop, NULL);

	return (StopTicks-StartTicks)*1000000/(TimeStop.tv_usec-TimeStart.tv_usec);
}
#endif

#ifdef WIN32
unsigned __int64 GetFrequency(void)
{
	unsigned __int64 TimeStart, TimeStop, TimeFreq;
	unsigned __int64 StartTicks, StopTicks;
	volatile unsigned __int64 i;

	QueryPerformanceFrequency((LARGE_INTEGER *)&TimeFreq);

	QueryPerformanceCounter((LARGE_INTEGER *)&TimeStart);
	StartTicks=rdtsc();

	for(i=0;i<1000000;i++);

	StopTicks=rdtsc();
	QueryPerformanceCounter((LARGE_INTEGER *)&TimeStop);

	return (StopTicks-StartTicks)*TimeFreq/(TimeStop-TimeStart);
}
#endif

#ifdef WIN32
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
			Key[wParam]=TRUE;

			switch(wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case '1':
					SwitchMode^=1;
					break;

				case '2':
					SwitchObject^=1;
					break;

				case 'M':
					if(ToggleMusic)
						alSourcePause(MusicSource);
					else
						alSourcePlay(MusicSource);

					ToggleMusic^=1;
					break;

				case VK_PAUSE:
				case 'P':
					TetrisPause(&Tetris);
					break;

				case 'N':
					TetrisNewGame(&Tetris);
					break;

				case 'B':
					TetrisGenBType(&Tetris);
					break;

				case 'A':
					Exposure+=0.125f;

					if(Exposure>=8.0f)
						Exposure=8.0f;
					break;

				case 'Z':
					Exposure-=0.125f;

					if(Exposure<=1.0f)
						Exposure=1.0f;
					break;

				case VK_PRIOR:
					CurrentScene++;

					if(CurrentScene>(NumScene-1))
						CurrentScene=0;

					LoadScene(SceneList[CurrentScene].String);
					break;

				case VK_NEXT:
					CurrentScene--;

					if(CurrentScene<0)
						CurrentScene=NumScene-1;

					LoadScene(SceneList[CurrentScene].String);
					break;

				case VK_HOME:
					if(MusicList)
					{
						MusicClose();

						CurrentMusic++;

						if(CurrentMusic>(NumMusic-1))
							CurrentMusic=0;

						if(MusicOpen(MusicList[CurrentMusic].String))
							Playback();
					}
					break;

				case VK_END:
					if(MusicList)
					{
						MusicClose();

						CurrentMusic--;

						if(CurrentMusic<0)
							CurrentMusic=NumMusic-1;

						if(MusicOpen(MusicList[CurrentMusic].String))
							Playback();
					}
					break;

				case VK_UP:
					TetrisRotate(&Tetris);
					break;

				case VK_DOWN:
					Tetris.IsDropping=1;
					break;

				case VK_LEFT:
					TetrisLeft(&Tetris);
					break;

				case VK_RIGHT:
					TetrisRight(&Tetris);
					break;

				default:
					break;
			}
			break;

		case WM_KEYUP:
			Key[wParam]=FALSE;

			switch(wParam)
			{
				case VK_DOWN:
					Tetris.IsDropping=0;
					break;
			}
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	RECT Rect;

	srand((unsigned int)rdtsc());

	memset(Objects, 0, NUM_OBJECTS);

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
	AdjustWindowRect(&Rect, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED, FALSE);

	Context.hWnd=CreateWindow(szAppName, szAppName, WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED|WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, Rect.right-Rect.left, Rect.bottom-Rect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(Context.hWnd, SW_SHOW);
	SetForegroundWindow(Context.hWnd);

	if(!InitSound())
		return -1;

	WidthP2=NextPower2(Width)*8;
	HeightP2=NextPower2(Height)*8;

	if(!CreateContext(&Context, 32, 24, 0, 0, OGL_DOUBLEBUFFER))
	{
		DestroyWindow(Context.hWnd);

		return -1;
	}

	if(!Init())
	{
		DestroyContext(&Context);
		DestroyWindow(Context.hWnd);

		return -1;
	}

	Frequency=GetFrequency();
	StartTime=rdtsc();

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
			StartTime=EndTime;
			EndTime=rdtsc();

			if(MusicList)
			{
				if(!Update())
				{
					MusicClose();

					if(MusicOpen(MusicList[randrange(0, NumMusic-1)].String))
						Playback();
				}
			}

			Render();
			SwapBuffers(Context.hDC);

			if(fTime>OneOver15)
			{
				TetrisRunStep(&Tetris);
				fTime=0.0f;
			}

			fTimeStep=(float)(EndTime-StartTime)/Frequency;
			fTime+=fTimeStep;

			FPSTemp+=1.0f/fTimeStep;

			if(Frames++>15)
			{
				FPS=FPSTemp/Frames;
				FPSTemp=0.0f;
				Frames=0;
			}
		}
	}

	Free3DS(&Model[1]);
	Free3DS(&Model[0]);

	DestroyContext(&Context);
	DestroyWindow(Context.hWnd);

	FREE(SceneList);

	if(MusicList)
		MusicClose();

	FREE(MusicList);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(ALContext);
	alcCloseDevice(ALDevice);

	DestroyWindow(Context.hWnd);

	return 0;
}
#endif

#ifdef __linux__
void EventLoop(void)
{
	int Keysym;
	XEvent Event;

	while(!Done)
	{
		while(XPending(dpy)>0)
		{
			XNextEvent(dpy, &Event);

			switch(Event.type)
			{
				case Expose:
					break;

				case ConfigureNotify:
					Width=Event.xconfigure.width;
					Height=Event.xconfigure.height;
					break;

				case ButtonPress:
					break;

				case KeyPress:
					Keysym=XLookupKeysym(&Event.xkey, 0);
					Key[Keysym]=1;

					switch(Keysym)
					{
						case XK_Escape:
							Done=1;
							break;

						case '1':
							SwitchMode^=1;
							break;

						case '2':
							SwitchObject^=1;
							break;

						case 'm':
							if(ToggleMusic)
								alSourcePause(MusicSource);
							else
								alSourcePlay(MusicSource);

							ToggleMusic^=1;
							break;

						case XK_Pause:
						case 'p':
							TetrisPause(&Tetris);
							break;

						case 'n':
							TetrisNewGame(&Tetris);
							break;

						case 'b':
							TetrisGenBType(&Tetris);
							break;

						case 'a':
							Exposure+=0.125f;

							if(Exposure>=8.0f)
								Exposure=8.0f;
							break;

						case 'z':
							Exposure-=0.125f;

							if(Exposure<=1.0f)
								Exposure=1.0f;
							break;

						case XK_Page_Up:
							CurrentScene++;

							if(CurrentScene>(NumScene-1))
								CurrentScene=0;

							LoadScene(SceneList[CurrentScene].String);
							break;

						case XK_Page_Down:
							CurrentScene--;

							if(CurrentScene<0)
								CurrentScene=NumScene-1;

							LoadScene(SceneList[CurrentScene].String);
							break;

						case XK_Home:
							if(MusicList)
							{
								MusicClose();

								CurrentMusic++;

								if(CurrentMusic>(NumMusic-1))
									CurrentMusic=0;

								if(MusicOpen(MusicList[CurrentMusic].String))
									Playback();
							}
							break;

						case XK_End:
							if(MusicList)
							{
								MusicClose();

								CurrentMusic--;

								if(CurrentMusic<0)
									CurrentMusic=NumMusic-1;

								if(MusicOpen(MusicList[CurrentMusic].String))
									Playback();
							}
							break;

						case XK_Up:
							TetrisRotate(&Tetris);
							break;

						case XK_Down:
							Tetris.IsDropping=1;
							break;

						case XK_Left:
							TetrisLeft(&Tetris);
							break;

						case XK_Right:
							TetrisRight(&Tetris);
							break;
					}
					break;

				case KeyRelease:
					Keysym=XLookupKeysym(&Event.xkey, 0);
					Key[Keysym]=0;

					switch(Keysym)
					{
						case XK_Down:
							Tetris.IsDropping=0;
							break;
					}
					break;
			}
		}

		if(MusicList)
		{
			if(!Update())
			{
				MusicClose();

				CurrentMusic++;

				if(CurrentMusic>=(NumMusic-1))
					CurrentMusic=0;

				if(MusicOpen(MusicList[CurrentMusic].String))
					Playback();
			}
		}

		StartTime=EndTime;
		EndTime=rdtsc();

		Render();
		glXSwapBuffers(dpy, Context.win);

		fTimeStep=(float)(EndTime-StartTime)/Frequency;
		fTime+=fTimeStep;

		FPSTemp+=1.0f/fTimeStep;

		if(Frames++>15)
		{
			FPS=FPSTemp/Frames;
			FPSTemp=0.0f;
			Frames=0;
		}
	}
}

int main(int argc, char **argv)
{
	XVisualInfo *visinfo=NULL;
	XSetWindowAttributes Attrib;
	XTextProperty WindowProp;
	Window root;

	srand((unsigned long)rdtsc());

	dpy=XOpenDisplay(NULL);

	if(dpy==NULL)
	{
		fprintf(stderr, "Can't open display\n");

		return -1;
	}

	Frequency=GetFrequency();

	WidthP2=NextPower2(Width);
	HeightP2=NextPower2(Height);

	if(!InitSound())
		return -1;

	if(!CreateContext(&Context, 24, 24, 0, 0, OGL_DOUBLEBUFFER))
	{
		fprintf(stderr, "Error: CreateContext failed\n");
		XCloseDisplay(dpy);

		return -1;
	}

	if(Context.visualid)
	{
		XVisualInfo visinfotemp;
		int visinfoitems;

		visinfotemp.visualid=Context.visualid;
		visinfo=XGetVisualInfo(dpy, VisualIDMask, &visinfotemp, &visinfoitems);

		if(visinfo==NULL&&visinfoitems!=1)
			return -1;
	}

	root=RootWindow(dpy, DefaultScreen(dpy));

	Attrib.background_pixel=0;
	Attrib.border_pixel=0;
	Attrib.colormap=XCreateColormap(dpy, root, visinfo->visual, AllocNone);
	Attrib.event_mask=StructureNotifyMask|ExposureMask|ButtonPressMask|KeyPressMask|KeyReleaseMask;

	Context.win=XCreateWindow(dpy, root, 0, 0, Width, Height, 0, visinfo->depth, InputOutput, visinfo->visual, CWBackPixel|CWBorderPixel|CWColormap|CWEventMask, &Attrib);

	XStringListToTextProperty(&szAppName, 1, &WindowProp);
	XSetWMName(dpy, Context.win, &WindowProp);
	XFree(WindowProp.value);

	XMapWindow(dpy, Context.win);

	XFree(visinfo);

	glXMakeCurrent(dpy, Context.win, Context.ctx);

	if(!Init())
	{
		fprintf(stderr, "Error: Init failed\n");

		DestroyContext(&Context);
		XDestroyWindow(dpy, Context.win);
		XCloseDisplay(dpy);

		return -1;
	}

	EventLoop();

	Free3DS(&Model[1]);
	Free3DS(&Model[0]);

	DestroyContext(&Context);
	XDestroyWindow(dpy, Context.win);

	FREE(SceneList);

	if(MusicList)
		MusicClose();

	FREE(MusicList);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(ALContext);
	alcCloseDevice(ALDevice);

	XCloseDisplay(dpy);

	return 0;
}
#endif

void DrawModel3DS(Model3DS_t *Model)
{
	int i;

	for(i=0;i<Model->NumMesh;i++)
	{
		if(GL_ARB_vertex_buffer_object_Flag&&Model->Mesh[i].VBOID)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, Model->Mesh[i].VBOID);

			glVertexPointer(3, GL_FLOAT, sizeof(float)*8, BUFFER_OFFSET(0));
			glEnableClientState(GL_VERTEX_ARRAY);

			glNormalPointer(GL_FLOAT, sizeof(float)*8, BUFFER_OFFSET(sizeof(float)*4));
			glEnableClientState(GL_NORMAL_ARRAY);
		}
		else
		{
			glVertexPointer(3, GL_FLOAT, 0, Model->Mesh[i].Vertex);
			glEnableClientState(GL_VERTEX_ARRAY);

			glNormalPointer(GL_FLOAT, 0, Model->Mesh[i].Normal);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		glDrawElements(GL_TRIANGLES, Model->Mesh[i].NumFace*3, GL_UNSIGNED_SHORT, Model->Mesh[i].Face);

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if(GL_ARB_vertex_buffer_object_Flag)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void BuildVBO3DS(Model3DS_t *Model)
{
	int i, j;

	if(!GL_ARB_vertex_buffer_object_Flag)
		return;

	for(i=0;i<Model->NumMesh;i++)
	{
		float *buffer=NULL, *ptr;

		if(!Model->Mesh[i].NumVertex)
			continue;

		glGenBuffersARB(1, &Model->Mesh[i].VBOID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, Model->Mesh[i].VBOID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)*Model->Mesh[i].NumVertex*8, NULL, GL_STATIC_DRAW_ARB);

		if(glGetError()==GL_NO_ERROR)
			buffer=(float *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);

		if(buffer==NULL)
		{
			char temp[1024];

			sprintf(temp, "VBO data buffer memory map failed or out of memory for object: %s", Model->Mesh[i].Name);
			Error(temp);

			glDeleteBuffersARB(1, &Model->Mesh[i].VBOID);
			Model->Mesh[i].VBOID=0;
			break;
		}

		for(j=0, ptr=buffer;j<Model->Mesh[i].NumVertex;j++)
		{
			*ptr++=Model->Mesh[i].Vertex[3*j+0];
			*ptr++=Model->Mesh[i].Vertex[3*j+1];
			*ptr++=Model->Mesh[i].Vertex[3*j+2];
			*ptr++=1.0f;

			*ptr++=Model->Mesh[i].Normal[3*j+0];
			*ptr++=Model->Mesh[i].Normal[3*j+1];
			*ptr++=Model->Mesh[i].Normal[3*j+2];
			*ptr++=1.0f;
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

void Blur(void)
{
	int QW=Width>>2, QH=Height>>2;
	float s=(float)Width/WidthP2, t=(float)Height/HeightP2;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Objects[BUFFER_BLUR]);
	BeginOverlay(QW, QH);

	// Downsample
	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_ORIGINAL]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, t);
		glVertex2i(0, QH);
		glTexCoord2f(0, 0);
		glVertex2i(0, 0);
		glTexCoord2f(s, t);
		glVertex2i(QW, QH);
		glTexCoord2f(s, 0);
		glVertex2i(QW, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// *Downsample

	// Blur
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[FRAGMENT_BLUR]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, 1.0f/(WidthP2>>2), 1.0f/(HeightP2>>2), 0.0f, 0.0f);

	// X
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, 1.0f/(WidthP2>>2), 0.0f, 0.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_BLUR0]);
	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, QH);
		glVertex2i(0, 0);
		glVertex2i(QW, QH);
		glVertex2i(QW, 0);
	glEnd();

	// Y
	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, 0.0f, 1.0f/(HeightP2>>2), 0.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_BLUR1]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, QH);
		glVertex2i(0, 0);
		glVertex2i(QW, QH);
		glVertex2i(QW, 0);
	glEnd();

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	// *Blur

	EndOverlay();

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Render(void)
{
	float QuatX[4], QuatY[4], Quat[4];

	//Render scene
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Objects[BUFFER_ORIGINAL]);

	Resize(Width, Height);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(PanX, PanY, Zoom);
	QuatAngle(RotateX, 0.0f, 1.0f, 0.0f, QuatX);
	QuatAngle(RotateY, 1.0f, 0.0f, 0.0f, QuatY);
	QuatMultiply(QuatX, QuatY, Quat);
	QuatMatrix(Quat, NULL);

	//Skybox
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Objects[TEXTURE_REFLECT]);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glPushMatrix();
	DrawSkyBoxInf();
	glPopMatrix();
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	//***

	glPushMatrix();
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, Objects[VERTEX_GENERIC]);
	glEnable(GL_VERTEX_PROGRAM_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[SwitchMode?FRAGMENT_LIGHTING:FRAGMENT_REFLECT]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);

	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  0, Matrix[ 0]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  1, Matrix[ 1]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  2, Matrix[ 2]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  3, Matrix[ 3]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  4, Matrix[ 4]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  5, Matrix[ 5]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  6, Matrix[ 6]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  7, Matrix[ 7]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  8, Matrix[ 8]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,  9, Matrix[ 9]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 10, Matrix[10]);
	glProgramEnvParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 11, Matrix[11]);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Objects[TEXTURE_REFLECT]);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, Objects[TEXTURE_SPECULAR]);

	TetrisDraw(&Tetris);

	glActiveTextureARB(GL_TEXTURE0_ARB);

	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[FRAGMENT_DIFFUSE]);
	DrawModel3DS(&Model[2]);

	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glPopMatrix();

	glPopMatrix();

	Blur();

	BeginOverlay(Width, Height);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		// Composite
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, Objects[FRAGMENT_COMPOSITE]);
		glEnable(GL_FRAGMENT_PROGRAM_ARB);

		glProgramEnvParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, 1.0f/WidthP2, 1.0f/HeightP2, 0.0f, 0.0f);
		glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, Exposure, 0.0f, 0.0f, 0.0f);

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_ORIGINAL]);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glBindTexture(GL_TEXTURE_2D, Objects[TEXTURE_BLUR0]);

		glBegin(GL_TRIANGLE_STRIP);
			glVertex3i(0, Height, -1);
			glVertex3i(0, 0, -1);
			glVertex3i(Width, Height, -1);
			glVertex3i(Width, 0, -1);
		glEnd();

		glActiveTextureARB(GL_TEXTURE0_ARB);

		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		// Stats
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		switch(Tetris.GameState)
		{
			case GameState_Running:
				Font_Print(0, 0, "Score: %d\nLevel: %d\nExposure: %0.3f\nFPS: %0.1f", Tetris.Score, 11-Tetris.GameSpeed, Exposure, FPS);
				break;

			case GameState_Paused:
				Font_Print(0, 0, "Paused\nScore: %d\nLevel: %d\nExposure: %0.3f\nFPS: %0.1f", Tetris.Score, 11-Tetris.GameSpeed, Exposure, FPS);
				break;

			case GameState_GameOver:
				Font_Print(0, 0, "Game Over\nScore: %d\nLevel: %d\nExposure: %0.3f\nFPS: %0.1f", Tetris.Score, 11-Tetris.GameSpeed, Exposure, FPS);
				break;
		}
	EndOverlay();
}

int Init(void)
{
	if(!GL_ARB_vertex_program_Flag)
	{
		Error("GL_ARB_vertex_program extension missing");
		return 0;
	}

	if(!GL_ARB_fragment_program_Flag)
	{
		Error("GL_ARB_fragment_program extension missing");
		return 0;
	}

	SceneList=BuildFileList(".", ".scn", &NumScene);

	if(SceneList==NULL)
	{
		Error("Could not find any scenes.");
		return 0;
	}

	CurrentScene=randrange(0, NumScene-1);
	LoadScene(SceneList[CurrentScene].String);

	Objects[TEXTURE_ORIGINAL]=Create2DTextureObject(GL_LINEAR, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, WidthP2, HeightP2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	Objects[TEXTURE_BLUR0]=Create2DTextureObject(GL_LINEAR, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, WidthP2>>2, HeightP2>>2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	Objects[TEXTURE_BLUR1]=Create2DTextureObject(GL_LINEAR, GL_CLAMP_TO_EDGE_EXT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, WidthP2>>2, HeightP2>>2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glGenRenderbuffersEXT(1, &Objects[BUFFER_DEPTH]);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, Objects[BUFFER_DEPTH]);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB, WidthP2, HeightP2);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	glGenFramebuffersEXT(1, &Objects[BUFFER_ORIGINAL]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Objects[BUFFER_ORIGINAL]);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Objects[TEXTURE_ORIGINAL], 0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, Objects[BUFFER_DEPTH]);

	glGenFramebuffersEXT(1, &Objects[BUFFER_BLUR]);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, Objects[BUFFER_BLUR]);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Objects[TEXTURE_BLUR0], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, Objects[TEXTURE_BLUR1], 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	Objects[VERTEX_GENERIC]=UploadProgram("Generic.tvp");

	Objects[FRAGMENT_REFLECT]=UploadProgram("Reflect.tfp");
	Objects[FRAGMENT_LIGHTING]=UploadProgram("Lighting.tfp");
	Objects[FRAGMENT_DIFFUSE]=UploadProgram("Diffuse.tfp");
	Objects[FRAGMENT_COMPOSITE]=UploadProgram("Composite.tfp");
	Objects[FRAGMENT_BLUR]=UploadProgram("Blur.tfp");

	if(Load3DS(&Model[0], "Sphere.3ds"))
		BuildVBO3DS(&Model[0]);

	if(Load3DS(&Model[1], "Cube.3ds"))
		BuildVBO3DS(&Model[1]);

	if(Load3DS(&Model[2], "Border.3ds"))
		BuildVBO3DS(&Model[2]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	TetrisNewGame(&Tetris);

	Objects[SOUND_LINE]=LoadStaticSound("Line.wav", 0);
	Objects[SOUND_LINE4]=LoadStaticSound("Line4.wav", 0);
	Objects[SOUND_DROP]=LoadStaticSound("Drop.wav", 0);
	Objects[SOUND_MOVE]=LoadStaticSound("Move.wav", 0);
	Objects[SOUND_ROTATE]=LoadStaticSound("Rotate.wav", 0);
	Objects[SOUND_LEVELUP]=LoadStaticSound("LevelUp.wav", 0);

	MusicList=BuildFileList(".", ".ogg", &NumMusic);

	if(MusicList)
	{
		CurrentMusic=randrange(0, NumMusic-1);
		if(MusicOpen(MusicList[CurrentMusic].String))
			Playback();
	}

	return 1;
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

unsigned int Create2DTextureObject(unsigned int FilterMode, unsigned int WrapMode)
{
	unsigned int TextureID;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterMode);

	return TextureID;
}

void LoadScene(char *Filename)
{
	FILE *stream;
	char Reflect[256];
	char Specular[256];

	if((stream=fopen(Filename, "rt"))==NULL)
		return;

	fscanf(stream, "%s", Reflect);
	fscanf(stream, "%s", Specular);
	fscanf(stream, "%f %f %f %f", &Matrix[0][0], &Matrix[0][1], &Matrix[0][2], &Matrix[0][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[1][0], &Matrix[1][1], &Matrix[1][2], &Matrix[1][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[2][0], &Matrix[2][1], &Matrix[2][2], &Matrix[2][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[3][0], &Matrix[3][1], &Matrix[3][2], &Matrix[3][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[4][0], &Matrix[4][1], &Matrix[4][2], &Matrix[4][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[5][0], &Matrix[5][1], &Matrix[5][2], &Matrix[5][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[6][0], &Matrix[6][1], &Matrix[6][2], &Matrix[6][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[7][0], &Matrix[7][1], &Matrix[7][2], &Matrix[7][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[8][0], &Matrix[8][1], &Matrix[8][2], &Matrix[8][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[9][0], &Matrix[9][1], &Matrix[9][2], &Matrix[9][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[10][0],&Matrix[10][1],&Matrix[10][2],&Matrix[10][3]);
	fscanf(stream, "%f %f %f %f", &Matrix[11][0],&Matrix[11][1],&Matrix[11][2],&Matrix[11][3]);

	fclose(stream);

	if(Objects[TEXTURE_REFLECT])
		glDeleteTextures(1, &Objects[TEXTURE_REFLECT]);

	Objects[TEXTURE_REFLECT]=Image_Upload(Reflect, IMAGE_RGBE|IMAGE_CUBEMAP_ANGULAR|IMAGE_CLAMP_EDGE);

	if(Objects[TEXTURE_SPECULAR])
		glDeleteTextures(1, &Objects[TEXTURE_SPECULAR]);

	Objects[TEXTURE_SPECULAR]=Image_Upload(Specular, IMAGE_RGBE|IMAGE_CUBEMAP_ANGULAR|IMAGE_CLAMP_EDGE);
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

			if(Line[i]=='\r'||Line[i]=='\n')
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

	if(!strncmp(Program, "!!ARBfp", 7))
		Target=GL_FRAGMENT_PROGRAM_ARB;
	else
	{
		if(!strncmp(Program, "!!ARBvp", 7))
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

	Error((char *)glGetString(GL_PROGRAM_ERROR_STRING_ARB));

	return 0;
}

int InitSound(void)
{
	if((ALDevice=alcOpenDevice(NULL))==NULL)
	{
		Error("alcOpenDevice failed.");
		return 0;
	}

	if((ALContext=alcCreateContext(ALDevice, NULL))==NULL)
	{
		Error("alcCreateContext failed.");
		return 0;
	}


	alcMakeContextCurrent(ALContext);

	return 1;
}

unsigned int LoadStaticSound(char *Filename, int Loop)
{
	unsigned int BufferID, SoundID, ALformat;
	FILE *stream=NULL;
	unsigned long riff, wave, fmt, data;
	unsigned short format;
	unsigned short channels;
	unsigned long frequency;
	unsigned short sample;
	unsigned long length;
	unsigned char *buffer=NULL;

	if((stream=fopen(Filename, "rb"))==NULL)
		return 0;

	fread(&riff, sizeof(unsigned long), 1, stream);
	fseek(stream, sizeof(unsigned long), SEEK_CUR);
	fread(&wave, sizeof(unsigned long), 1, stream);
	fread(&fmt, sizeof(unsigned long), 1, stream);
	fseek(stream, sizeof(unsigned long), SEEK_CUR);

	if(riff!=0x46464952||wave!=0x45564157||fmt!=0x20746d66)
	{
		fclose(stream);
		return 0;
	}

	fread(&format, sizeof(unsigned short), 1, stream);

	if(format!=1)
	{
		// Only support PCM streams
		fclose(stream);
		return 0;
	}

	fread(&channels, sizeof(unsigned short), 1, stream);
	fread(&frequency, sizeof(unsigned long), 1, stream);
	fseek(stream, sizeof(unsigned long), SEEK_CUR);
	fseek(stream, sizeof(unsigned short), SEEK_CUR);
	fread(&sample, sizeof(unsigned short), 1, stream);

	if(channels==2)
	{
		if(sample==16)
			ALformat=AL_FORMAT_STEREO16;
		else
			ALformat=AL_FORMAT_STEREO8;
	}
	else
	{
		if(sample==16)
			ALformat=AL_FORMAT_MONO16;
		else
			ALformat=AL_FORMAT_MONO8;
	}

	fread(&data, sizeof(unsigned long), 1, stream);

	if(data!=0x61746164)
	{
		fclose(stream);
		return 0;
	}

	fread(&length, sizeof(unsigned long), 1, stream);

	buffer=(unsigned char *)malloc(length);

	if(buffer==NULL)
	{
		fclose(stream);
		return 0;
	}

	fread(buffer, sizeof(unsigned char), length, stream);

	alGenBuffers(1, &BufferID);
	alBufferData(BufferID, ALformat, buffer, length, frequency);

	FREE(buffer);

	alGenSources(1, &SoundID);
	alSourcef(SoundID, AL_PITCH, 1.0f);
	alSourcei(SoundID, AL_LOOPING, Loop);
	alSourcei(SoundID, AL_BUFFER, BufferID);

	return SoundID;
}
