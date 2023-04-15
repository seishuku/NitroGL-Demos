typedef struct winampVisModule
{
	char *description;
	HWND hwndParent;
	HINSTANCE hDllInstance;
	int sRate;
	int nCh;
	int latencyMs;
	int delayMs;
	int spectrumNch;
	int waveformNch;
	unsigned char spectrumData[2][576];
	unsigned char waveformData[2][576];

	void(*Config)(struct winampVisModule *this_mod);
	int (*Init)(struct winampVisModule *this_mod);
	int (*Render)(struct winampVisModule *this_mod);
	void (*Quit)(struct winampVisModule *this_mod);
	void *userData;
} winampVisModule;

typedef struct
{
	int version;
	char *description;
	winampVisModule* (*getModule)(int);
} winampVisHeader;

typedef winampVisHeader* (*winampVisGetHeaderType)();

#define VIS_HDRVER 0x101
