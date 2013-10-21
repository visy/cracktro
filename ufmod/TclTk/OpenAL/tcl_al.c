/* TCL.C
   -----
   uFMOD public source code release. Provided as-is.
   Make uFMOD commands available as an extension for Tcl/Tk.
*/

#define XM_MEMORY         1
#define XM_FILE           2
#define XM_NOLOOP         8
#define XM_SUSPENDED      16
#define uFMOD_MIN_VOL     0
#define uFMOD_MAX_VOL     25
#define uFMOD_DEFAULT_VOL uFMOD_MAX_VOL

/* OpenAL API */
void* alcOpenDevice(void*);
void* alcCreateContext(void*, void*);
void alcMakeContextCurrent(void*);
void alGenSources(int, void**);
int alGetError();
void alcDestroyContext(void*);
void alcCloseDevice(void*);

/* uFMOD (OpenAL) */
int uFMOD_OALPlaySong(const void*, int, int, void*);
void uFMOD_Jump2Pattern(int);
void uFMOD_Pause();
void uFMOD_Resume();
unsigned int uFMOD_GetStats();
unsigned int uFMOD_GetRowOrder();
unsigned int uFMOD_GetTime();
const unsigned char* uFMOD_GetTitle();
void uFMOD_SetVolume(int);

/* Tcl/Tk API */
void Tcl_SetResult(void*, char*, int);
void Tcl_CreateCommand(void*, char*, void*, int, int);
void Tcl_CreateExitHandler(void*, int);
void Tk_DefineBitmap(void*, const char*, void*, int, int);

/* Icon #1 */
unsigned char uf_ico1_face[] = {
	0xBF,0xFF, /*       X          */
	0x3F,0xFE, /*       XXX        */
	0x3F,0xF8, /*       XXXXX      */
	0x3F,0xE0, /*       XXXXXXX    */
	0xBF,0xE1, /*       X  XXXX    */
	0xBF,0xE7, /*       X    XX    */
	0xBF,0xEF, /*       X     X    */
	0x87,0xEF, /*    XXXX     X    */
	0x9B,0xEF, /*   X  XX     X    */
	0x8B,0xEF, /*   X XXX     X    */
	0x83,0xE1, /*   XXXXX  XXXX    */
	0xC7,0xE6, /*    XXX  X  XX    */
	0xFF,0xE2, /*         X XXX    */
	0xFF,0xE0, /*         XXXXX    */
	0xFF,0xF1, /*          XXX     */
	0xFF,0xFF  /*                  */
};
unsigned char uf_ico1_mask[] = {
	0x40,0x00, /*       X          */
	0xC0,0x01, /*       XXX        */
	0xC0,0x07, /*       XXXXX      */
	0xC0,0x1F, /*       XXXXXXX    */
	0x40,0x1E, /*       X  XXXX    */
	0x40,0x18, /*       X    XX    */
	0x40,0x10, /*       X     X    */
	0x78,0x10, /*    XXXX     X    */
	0x74,0x10, /*   X XXX     X    */
	0x7C,0x10, /*   XXXXX     X    */
	0x7C,0x1E, /*   XXXXX  XXXX    */
	0x38,0x1D, /*    XXX  X XXX    */
	0x00,0x1F, /*         XXXXX    */
	0x00,0x1F, /*         XXXXX    */
	0x00,0x0E, /*          XXX     */
	0x00,0x00  /*                  */
};

static char sret[12];
static void *OAL_device, *OAL_context = 0, *OAL_source = 0;

/* Convert integer to ascii and return to Tcl. */
static void dw2interp(void *interp, unsigned int dw){
char *cur = &sret[10];
	do{
		cur--;
		*cur = dw % 10 + '0';
		dw = dw / 10;
	}while(dw);
	Tcl_SetResult(interp, cur, 0);
}

/* Convert ascii to integer. */
static int a2dw(char *a){
int dw = 0;
	if(a){
		while(*a >= '0' && *a <= '9'){
			dw = dw * 10 + *a - '0';
			a++;
		}
	}
	return dw;
}

/* return ( (!x) ? 0 : (pi * log10(x)) ); */
static unsigned int rescale(unsigned int x){
	if(!x) return 0;
	asm(	"pushl %%eax\n\t"
		"fldlg2\n\t"
		"fildl (%%esp)\n\t"
		"fyl2x\n\t"
		"fldpi\n\t"
		"fmulp %%st(0),%%st(1)\n\t"
		"fistpl (%%esp)\n\t"
		"popl %%eax"
		: "=a" (x)
		: "a"  (x)
	);
	return x;
}

/* Start playing the given XM. */
static int ufmodplay(void *clientdata, void *interp, int argc, char *argv[]){
	if(!OAL_source || argc != 2 || !uFMOD_OALPlaySong(argv[1], 0, XM_FILE, OAL_source))
		Tcl_SetResult(interp, "error", 0);
	return 0;
}

/* Stop playback. */
static int ufmodstop(void *clientdata, void *interp, int argc, char *argv[]){
	uFMOD_OALPlaySong(0, 0, 0, 0);
	return 0;
}

/* Jump 2 pattern. */
static int ufmodjmp2pat(void *clientdata, void *interp, int argc, char *argv[]){
	if(argc == 2) uFMOD_Jump2Pattern(a2dw(argv[1]));
	return 0;
}

/* Pause playback. */
static int ufmodpause(void *clientdata, void *interp, int argc, char *argv[]){
	uFMOD_Pause();
	return 0;
}

/* Return RMS volume in L channel. */
static int ufmodgetlvol(void *clientdata, void *interp, int argc, char *argv[]){
	dw2interp(interp, rescale(uFMOD_GetStats() >> 16));
	return 0;
}

/* Return RMS volume in R channel. */
static int ufmodgetrvol(void *clientdata, void *interp, int argc, char *argv[]){
	dw2interp(interp, rescale(uFMOD_GetStats() & 0xFFFF));
	return 0;
}

/* Return currently playing row. */
static int ufmodgetrow(void *clientdata, void *interp, int argc, char *argv[]){
	dw2interp(interp, uFMOD_GetRowOrder() & 0xFFFF);
	return 0;
}

/* Return currently playing order. */
static int ufmodgetorder(void *clientdata, void *interp, int argc, char *argv[]){
	dw2interp(interp, uFMOD_GetRowOrder() >> 16);
	return 0;
}

/* Return time in s since song started. */
static int ufmodgettime(void *clientdata, void *interp, int argc, char *argv[]){
	dw2interp(interp, uFMOD_GetTime() / 1000);
	return 0;
}

/* Return current track's title, if any. */
static int ufmodgettitle(void *clientdata, void *interp, int argc, char *argv[]){
	Tcl_SetResult(interp, (char*)uFMOD_GetTitle(), 0);
	return 0;
}

/* Set global volume. */
static int ufmodsetvol(void *clientdata, void *interp, int argc, char *argv[]){
	if(argc == 2) uFMOD_SetVolume(a2dw(argv[1]));
	return 0;
}

/* This function is called before unloading the library. */
static int onexit(void *clientdata, void *interp, int argc, char *argv[]){
	uFMOD_OALPlaySong(0, 0, 0, 0);
	if(OAL_context){
		alcMakeContextCurrent(0);
		alcDestroyContext(OAL_context);
	}
	alcCloseDevice(OAL_device);
	return 0;
}

int Ufmod_Init(void *interp){
	/* Init OpenAL. */
	if(!OAL_source){
		if((OAL_device = alcOpenDevice(0))){
			OAL_context = alcCreateContext(OAL_device, 0);
			alcMakeContextCurrent(OAL_context);
			alGenSources(1, &OAL_source);
			if(alGetError()) OAL_source = 0;
		}
	}
	/* Define uFMOD commands. */
	Tcl_CreateCommand(interp, "uFMOD_OALPlaySong",  ufmodplay,     0, 0);
	Tcl_CreateCommand(interp, "uFMOD_StopSong",     ufmodstop,     0, 0);
	Tcl_CreateCommand(interp, "uFMOD_Jump2Pattern", ufmodjmp2pat,  0, 0);
	Tcl_CreateCommand(interp, "uFMOD_Pause",        ufmodpause,    0, 0);
	Tcl_CreateCommand(interp, "uFMOD_Resume",       uFMOD_Resume,  0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetLVol",      ufmodgetlvol,  0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetRVol",      ufmodgetrvol,  0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetRow",       ufmodgetrow,   0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetOrder",     ufmodgetorder, 0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetTime",      ufmodgettime,  0, 0);
	Tcl_CreateCommand(interp, "uFMOD_GetTitle",     ufmodgettitle, 0, 0);
	Tcl_CreateCommand(interp, "uFMOD_SetVolume",    ufmodsetvol,   0, 0);
	/* Define uFMOD icons. */
	Tk_DefineBitmap(interp, "uFMOD_ico1_face", uf_ico1_face, 16, 16);
	Tk_DefineBitmap(interp, "uFMOD_ico1_mask", uf_ico1_mask, 16, 16);
	/* Stop playback and release OpenAL before quitting. */
	Tcl_CreateExitHandler(onexit, 0);
	return 0;
}

int Ufmod_SafeInit(void *interp){ return Ufmod_Init(interp); }
