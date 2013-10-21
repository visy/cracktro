#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "oalufmod.h"     /* uFMOD (OpenAL) */
#include "song.h" /* The XM file */

/* Sometimes, LIBC requires the following symbols: */
const char environ, __progname;

/* OpenAL */
void* alcOpenDevice(char*);
void* alcCreateContext(void*, void*);
void  alcMakeContextCurrent(void*);
void  alGenSources(int, void*);
int   alGetError();
void  alcDestroyContext(void*);
void  alcCloseDevice(void*);

#ifdef OPENAL_DYN

	/* OpenAL: use dynamic linking at run time. */
	#include <dlfcn.h>

	void *_imp__alBufferData,
	     *_imp__alDeleteBuffers,
	     *_imp__alGenBuffers,
	     *_imp__alGetError,
	     *_imp__alGetSourcei,
	     *_imp__alSourcei,
	     *_imp__alSourcePlay,
	     *_imp__alSourceQueueBuffers,
	     *_imp__alSourceStop,
	     *_imp__alSourceUnqueueBuffers,
	     *_imp__alcOpenDevice,
	     *_imp__alcCreateContext,
	     *_imp__alcMakeContextCurrent,
	     *_imp__alGenSources,
	     *_imp__alcDestroyContext,
	     *_imp__alcCloseDevice;

	asm( ".global alBufferData\n          alBufferData:           jmpl *(_imp__alBufferData)\n"
	     ".global alDeleteBuffers\n       alDeleteBuffers:        jmpl *(_imp__alDeleteBuffers)\n"
	     ".global alGenBuffers\n          alGenBuffers:           jmpl *(_imp__alGenBuffers)\n"
	     ".global alGetError\n            alGetError:             jmpl *(_imp__alGetError)\n"
	     ".global alGetSourcei\n          alGetSourcei:           jmpl *(_imp__alGetSourcei)\n"
	     ".global alSourcei\n             alSourcei:              jmpl *(_imp__alSourcei)\n"
	     ".global alSourcePlay\n          alSourcePlay:           jmpl *(_imp__alSourcePlay)\n"
	     ".global alSourceQueueBuffers\n  alSourceQueueBuffers:   jmpl *(_imp__alSourceQueueBuffers)\n"
	     ".global alSourceStop\n          alSourceStop:           jmpl *(_imp__alSourceStop)\n"
	     ".global alSourceUnqueueBuffers\nalSourceUnqueueBuffers: jmpl *(_imp__alSourceUnqueueBuffers)\n"
	     ".global alcOpenDevice\n         alcOpenDevice:          jmpl *(_imp__alcOpenDevice)\n"
	     ".global alcCreateContext\n      alcCreateContext:       jmpl *(_imp__alcCreateContext)\n"
	     ".global alcMakeContextCurrent\n alcMakeContextCurrent:  jmpl *(_imp__alcMakeContextCurrent)\n"
	     ".global alGenSources\n          alGenSources:           jmpl *(_imp__alGenSources)\n"
	     ".global alcDestroyContext\n     alcDestroyContext:      jmpl *(_imp__alcDestroyContext)\n"
	     ".global alcCloseDevice\n        alcCloseDevice:         jmpl *(_imp__alcCloseDevice)\n" );

	typedef struct{
		const char *pubname;
		void **f;
	}IAT_NODE;
	IAT_NODE OPENAL32[] = {
		{ "alBufferData",           &_imp__alBufferData },
		{ "alDeleteBuffers",        &_imp__alDeleteBuffers },
		{ "alGenBuffers",           &_imp__alGenBuffers },
		{ "alGetError",             &_imp__alGetError },
		{ "alGetSourcei",           &_imp__alGetSourcei },
		{ "alSourcei",              &_imp__alSourcei },
		{ "alSourcePlay",           &_imp__alSourcePlay },
		{ "alSourceQueueBuffers",   &_imp__alSourceQueueBuffers },
		{ "alSourceStop",           &_imp__alSourceStop },
		{ "alSourceUnqueueBuffers", &_imp__alSourceUnqueueBuffers },
		{ "alcOpenDevice",          &_imp__alcOpenDevice },
		{ "alcCreateContext",       &_imp__alcCreateContext },
		{ "alcMakeContextCurrent",  &_imp__alcMakeContextCurrent },
		{ "alGenSources",           &_imp__alGenSources },
		{ "alcDestroyContext",      &_imp__alcDestroyContext },
		{ "alcCloseDevice",         &_imp__alcCloseDevice }
	};

	static const char error3[] = "-ERR: Unable to load libopenal.so\n";

	/* Load OpenAL shared library and initialize all function thunks. */
	int LoadOpenAL(){
	int i;
	void *LibOpenAL, *addr;
		LibOpenAL = dlopen("libopenal.so", RTLD_NOW);
		if(!LibOpenAL) LibOpenAL = dlopen("libopenal.so.0", RTLD_NOW);
		if(!LibOpenAL) LibOpenAL = dlopen("libopenal.so.0.0.8", RTLD_NOW);
		if(!LibOpenAL) return 0;
		for(i = 0; i < sizeof(OPENAL32)/sizeof(IAT_NODE); i++){
			addr = dlsym(LibOpenAL, OPENAL32[i].pubname);
			*(OPENAL32[i].f) = addr;
			if(!addr) return 0;
		}
		return 1; /* OK */
	}

#endif

static const char msg[]    = "Playing song... [Press any key to quit]\n";
static const char error1[] = "-ERR: Unable to start playback\n";
static const char error2[] = "-ERR: Error opening the default OpenAL output device\n";

void _start(void){
void *OAL_device, *OAL_context;
unsigned int OAL_source;
struct termios term;
int c;
	/* Load OpenAL. */
	#ifdef OPENAL_DYN
		if(!LoadOpenAL()){
			write(1, error3, sizeof(error3) - 1);
			_exit(0);
		}
	#endif

	/* Init OpenAL. */
	if((OAL_device = alcOpenDevice(0))){

		OAL_context = alcCreateContext(OAL_device, 0);
		alcMakeContextCurrent(OAL_context);
		alGenSources(1, &OAL_source);

		if(!alGetError() &&
			/* Start playback. */
			uFMOD_OALPlaySong(song, sizeof(song), XM_MEMORY, OAL_source)){
			write(1, msg, sizeof(msg) - 1);

			/* Get current terminal mode. */
			tcgetattr(0, &term);
			/* Disable ICANON and ECHO. */
			term.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(0, TCSANOW, &term);

			/* Wait for user input. */
			read(0, &c, 1);

			/* Restore normal terminal mode. */
			term.c_lflag |= ICANON | ECHO;
			tcsetattr(0, TCSANOW, &term);

			/* Stop playback. */
			uFMOD_StopSong();
		}else
			write(1, error1, sizeof(error1) - 1);

		/* OpenAL 0.0.7 is eager to hang for a while here.
		   Make sure you're using the latest OpenAL distro
		   available to avoid this kind of bugs. */
		alcMakeContextCurrent(0);
		alcDestroyContext(OAL_context);
		alcCloseDevice(OAL_device);

	}else
		write(1, error2, sizeof(error2) - 1);
	_exit(0);
}
