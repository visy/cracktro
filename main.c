#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "oalufmod.h"     /* uFMOD (OpenAL) */
#include "song.h" /* The XM file */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include <math.h>

#include <SDL/SDL.h>

#include "GL/gl.h"
#include "SDL/SDL.h"

#include "stb_image.c"

#include "guage.h"
#include "greets.h"

SDL_Surface *load_image(char *filename)
{
    int x, y, comp;
    unsigned char *data;
    uint32_t rmask, gmask, bmask, amask;
    SDL_Surface *rv;

    FILE *file = fopen(filename, "rb");
    if (!file)
        return 0;

    data = stbi_load_from_file(file, &x, &y, &comp, 0);
    fclose(file);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    if (comp == 4) {
        rv = SDL_CreateRGBSurface(0, x, y, 32, rmask, gmask, bmask, amask);
    } else if (comp == 3) {
        rv = SDL_CreateRGBSurface(0, x, y, 24, rmask, gmask, bmask, 0);
    } else {
        stbi_image_free(data);
        return 0;
    }

    memcpy(rv->pixels, data, comp * x * y);
    stbi_image_free(data);

    return rv;
}

GLuint create_texture(SDL_Surface *tex) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	SDL_FreeSurface(tex);
	return texture;
}

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

void draw_glyph(float x, float y, float z, int tile, float angle) {

	glTranslatef(x,y,0);
	glRotatef(angle,0.0f,0.0f,1.0f);

	float tile_ix = (int)(tile%16);
	float tile_iy = (int)(tile/16);

	float tx = (tile_ix/16.f);
	float ty = (tile_iy/16.f);

	float ts = 1.0f/16.0f;

	glBegin(GL_QUADS);
		glTexCoord2f(tx+0.0f,ty+0.0f);
		glVertex3i(-1,1,z);
		glTexCoord2f(tx+ts,ty);
		glVertex3i(1,1,z);
		glTexCoord2f(tx+ts,ty+ts);
		glVertex3i(1,-1,z);
		glTexCoord2f(tx,ty+ts);
		glVertex3i(-1,-1,z);
	glEnd();
}

void draw_string(float x, float y, float scale, char *in, char in2, bool ass)
{
	int i;
	int len = strlen(in);
	glLoadIdentity();
	glScalef(scale, scale, 1.0f);
	for (i = 0; i < len; i++)
	{
		glPushMatrix();
		draw_glyph(x+i*2.0f, y, -10.0f, in[i], 0.0f);
		glPopMatrix();
	}

	if (ass)
		draw_glyph(x+(len+2)*2.0f, y, -10.f, in2, 0.0f);

}

int cracktro_data[64*16] = {7,3,3,3,3,4,2,3,3,3,3,3,3,3,3,3,4,2,3,3,3,3,3,4,2,3,3,3,3,4,2,3,3,3,3,3,4,2,3,4,2,3,3,4,2,3,3,3,3,3,4,2,3,3,3,3,3,4,11,11,11,11,11,11,5,196,147,196,147,7,5,165,151,151,165,151,165,151,212,151,7,5,195,195,197,195,195,7,5,196,147,196,147,7,5,165,149,151,149,149,7,5,165,4,5,149,149,7,5,151,165,151,212,151,7,5,195,195,197,195,195,7,11,11,11,11,11,11,5,181,196,9,9,10,8,9,9,9,9,9,9,9,165,165,7,5,195,147,9,180,195,7,5,181,196,9,9,10,8,9,9,9,181,181,7,5,165,10,8,180,164,7,8,9,9,9,165,165,7,5,195,147,9,180,195,7,11,11,11,11,11,11,5,196,214,3,3,3,3,3,3,4,2,3,4,11,5,151,7,5,197,7,11,5,197,7,5,196,214,3,3,3,3,3,3,4,5,146,147,148,7,11,11,5,181,7,2,3,4,11,5,151,7,5,197,7,11,5,197,7,11,11,11,11,11,11,5,181,146,147,148,149,150,151,152,7,5,165,4,11,5,212,7,5,195,7,11,5,195,7,5,181,146,147,148,149,150,151,152,7,5,162,163,164,165,3,4,5,164,7,5,165,4,11,5,212,7,5,195,7,11,5,195,7,11,11,11,11,11,11,5,197,162,163,164,165,166,167,168,7,5,212,180,3,180,165,7,5,197,7,11,5,197,7,5,197,162,163,164,165,166,167,168,7,5,178,179,180,165,163,4,5,164,7,5,212,180,3,180,165,7,5,197,7,11,5,197,7,11,11,11,11,11,11,5,197,178,179,180,181,182,183,184,7,5,165,165,212,212,165,10,5,195,7,11,5,195,7,5,197,178,179,180,181,182,183,184,7,5,181,7,5,165,163,4,5,181,7,5,165,165,212,212,165,10,8,9,10,11,5,195,7,11,11,11,11,11,11,5,197,194,195,196,197,198,199,200,7,5,195,7,5,195,7,2,3,3,3,3,212,195,7,5,197,194,195,196,197,198,199,200,7,5,149,7,5,149,163,7,5,164,7,5,195,7,5,195,7,2,3,3,3,3,212,195,7,11,11,11,11,11,11,5,147,210,211,212,213,214,215,216,7,5,195,7,5,212,7,5,212,212,197,212,212,212,7,5,147,210,211,212,213,214,215,216,7,5,181,7,5,181,163,7,5,164,7,5,195,7,5,212,7,5,212,212,197,212,212,212,7,11,11,11,11,11,11,8,9,9,9,9,9,9,9,9,10,8,9,10,8,9,10,8,9,9,9,9,197,195,7,8,9,9,9,9,9,9,9,9,10,8,9,10,8,9,9,10,8,9,10,8,9,10,8,9,10,8,9,9,9,9,9,9,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,8,9,10,11,8,9,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,18,77,73,99,56,48,105,68,83,84,17,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11};

static char logo_data[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,
	1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,
	1,1,1,1,0,0,0,1,1,1,1,1,0,1,0,0,
	0,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,
	1,1,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,
	1,1,1,1,0,0,1,1,1,1,1,1,0,1,0,0,
	0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,
	0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
	0,0,1,1,0,0,1,1,0,0,0,0,0,1,0,1,
	1,1,0,0,0,1,1,0,0,1,1,0,0,1,0,0,
	0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,
	1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,
	1,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,
	0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,
	1,1,1,1,0,0,1,1,0,0,0,0,0,1,1,1,
	1,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,
	0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,
	0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,0,
	1,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,
	0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,
	1,1,1,1,0,0,1,1,1,1,1,1,0,1,0,0,
	1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,
	1,1,1,1,0,0,0,1,1,1,1,1,0,1,0,0,
	0,1,1,1,0,0,1,1,1,1,1,0,0,1,0,0,
	0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

float vertsCoords[] = {0.5f, 0.5f, 0.5f,          //V0
                                      -0.5f, 0.5f, 0.5f,           //V1
                                      -0.5f, -0.5f, 0.5f,         //V2
                                       0.5f, -0.5f, 0.5f,         //V3
                                       0.5f, -0.5f, -0.5f,       //V4
                                       0.5f,  0.5f, -0.5f,       //V5
                                      -0.5f, 0.5f, -0.5f,       //V6
                                      -0.5f, -0.5f, -0.5f,     //V7
                        };  
GLubyte indices[] = {0, 1, 2, 3,              //Front face
                                5, 0, 3, 4,             //Right face
                                5, 6, 7, 4,             //Back face
                                5, 6, 1, 0,             //Upper face
                                1, 6, 7, 2,              //Left face
                                7, 4, 3, 2,             //Bottom face
                      };                     

void mankeli_display(int millis, float transition)
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static const char scroller[] = 
"                                                                                                                                                                                                                                                                                                                                                                        "\
"Hackers/LHB Cracking to your mind.                                                      We removed the protections,                                                     and we removed your mental blocks                                                                     we lowered your inhibitions and modified your state of being                                                                   with our trainers specifically designed to tap into the innards of your skulls, you can achieve things you have never believed to have been possible before....     "\
"                                                                                                                                                           ";
	glTranslatef(transition, 0.f, 0.f);

	glBegin(GL_QUADS);
	// aseta tekstuuri

	static bool off = false;
	int charoffs = millis / 40;
	if (scroller[charoffs] == 0)
           off = true;

	if (off)
		goto LOPPUXD;
	int sx, sy;
	for (sy = 0; sy < 25; sy++)
	{
		for (sx = 0; sx < 12; sx++)
		{
			int cha = scroller[sy * 12 + sx + charoffs];
			if (cha == 0)
				goto LOPPUXD;
			int cx = cha & 15;
			int cy = cha / 16;
			float tx = (float)cx / 16.f;
			float ty = (float)cy / 16.f;
			float d = 1.0f / 16.f;
			float xd = (1.0f / 40.f) * 2.0f;
			float yd = -(1.0f / 25.f) * 2.0f;
			float x1 = ((float)sx / 40.f) * 2.0f - 1.0f;
			float y1 = 2.0f - ((float)sy / 25.f) * 2.0f - 1.0f;
			glTexCoord3f(tx, ty, 0.f);
			glVertex2f(x1, y1);
			glTexCoord3f(tx+d, ty, 0.f);
			glVertex2f(x1+xd, y1);
			glTexCoord3f(tx+d, ty+d, 0.f);
			glVertex2f(x1+xd, y1+yd);
			glTexCoord3f(tx, ty+d, 0.f);
			glVertex2f(x1, y1+yd);
		}
	}
	LOPPUXD:;
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float base = 2.f + sin(millis * 0.0076f) * 0.4f + sin(millis * 0.0023f) * 0.2f;
	glFrustum(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f + base, 1.f + base);
	glScalef(1.0f, 640.f/480.f, 1.0f);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(transition, 0.f, 0.f);


	glTranslatef(0.f, 0.f, -2.f);
	glRotatef(millis * 0.08f * 1.f, 0.4f, 0.4f, 0.4f);
	glRotatef(sin(millis * 0.001f) * 100.f, 0.7f, 0.1f, 0.2f);



	int i;
	for (i = 0; i < 6; i++)
	{
		int pb = i * 4;
		float x1 = vertsCoords[indices[pb + 0] * 3 + 0];
		float xd = vertsCoords[indices[pb + 2] * 3 + 0] - vertsCoords[indices[pb + 0] * 3 + 0];
		float y1 = vertsCoords[indices[pb + 0] * 3 + 1];
		float yd = vertsCoords[indices[pb + 2] * 3 + 1] - vertsCoords[indices[pb + 0] * 3 + 1];
		float z1 = vertsCoords[indices[pb + 0] * 3 + 1];
		float zd = vertsCoords[indices[pb + 2] * 3 + 1] - vertsCoords[indices[pb + 0] * 3 + 2];

		glLineWidth(10.0f + sin(millis * 0.00331f + i) * 8.0f);

	glBegin(GL_LINE_LOOP);
	glColor3f(0.1f, 0.3f, 0.5f);
		glVertex3f(x1,y1,z1);
		glVertex3f(x1+xd,y1+yd,z1+zd);
	glColor3f(1.0f, 0.5f, 0.1f);
		glVertex3f(x1+xd,y1,z1);
		glVertex3f(x1,y1+yd,z1+zd);
	glColor3f(0.1f, 0.6f, 0.9f);
		glVertex3f(x1,y1,z1+zd);
		glVertex3f(x1+xd,y1+yd,z1);
	glColor3f(1.0f, 0.4f, 0.2f);
	glEnd();
	}

	glLoadIdentity();
		glTranslatef(transition, 0.f, 0.f);

glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	glColor3f(0.5f, 0.2f, 0.0f);
	glBegin(GL_LINES);
	glColor3f(0.1f, 0.6f, 0.9f);
	glVertex2f(-0.4f, -1.0f);
	glColor3f(1.0f, 0.4f, 0.2f);
	glVertex2f(-0.4f, 1.0f);
	glEnd();

}
void _start(void){

	void *OAL_device, *OAL_context;
	unsigned int OAL_source;
	struct termios term;
	int c;
			
	SDL_SetVideoMode(640,480,0,SDL_OPENGL|SDL_FULLSCREEN);
	SDL_ShowCursor(SDL_DISABLE);

	// ADD TO RELLU
	SDL_Delay(5000);

	/* Load OpenAL. */
	#ifdef OPENAL_DYN
		if(!LoadOpenAL()){
			_exit(0);
		}
	#endif

	int quit = 0;

	/* Init OpenAL. */
	if((OAL_device = alcOpenDevice(0))){

		OAL_context = alcCreateContext(OAL_device, 0);
		alcMakeContextCurrent(OAL_context);
		alGenSources(1, &OAL_source);

		if(!alGetError() &&
			/* Start playback. */
			uFMOD_OALPlaySong(song, sizeof(song), XM_MEMORY, OAL_source)){

			/* Get current terminal mode. */
			tcgetattr(0, &term);
			/* Disable ICANON and ECHO. */
			term.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(0, TCSANOW, &term);

////////////////////////////////// coodi

			SDL_Event event;
			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
			SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
			
			SDL_Surface *font;
			font = load_image("font.png");
			GLuint font_texture = create_texture(font);
			
			SDL_Surface *aegis;
			aegis = load_image("aegis.jpg");
			GLuint aegis_texture = create_texture(aegis);
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glFrustum(-1.33,1.33,-1,1,1.0,100);
			glMatrixMode(GL_MODELVIEW);
			//glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
	
			float ti = 0.0f;

			float scrollstartmillis = 0;
			float scrollstartmillis2 = 0;

			float mankelistart = 0;

			int prevrow = -1;
			int rowacc = 0;

			char text[1000] = "        .........................................___________________________----------------------------------                                BOOYA BITCHES !!! \021HACKERS\020 AND \021LHB\020 PRESENTS A CRACKTRO DESIGNED TO FUCK UP YOUR GENDER PERSPECTIVE - THIS TIME WE ARE OUT TO GET YOU TOTALLY \021GAY\020 BECAUSE THAT IS HOW THE HOT EUROPE WEIRDOS PARTY! ------ MUSIC BY LASERBOY === CODE BY ZERO COOL - GFX HI-STACK & ASH CHECKSUM - MESS WITH THE BEST - DIE LIKE THE REST!            THERE ARE THINGS IN THIS WORLD THAT MAKE YOU SCREAM FOR MORE AND SHOUT FOR SAFETY ---- WE CRACK YOUR MIND WITH THE COOL SOFTWAREZ AND PROVIDE IDENTITIES FOR ALL THE CUTEST PROSPECT MEMBERS - INQUIRE WITHIN! - WE WOULD ALSO LIKE TO SEND FUCKINGS TO \021AEGIS\020 FOR HAVING ESCAPED TEOSTO POLICE TO AUSTRIA INSTEAD OF BEING AT THE HOSPITAL ALTPARTY!                                                                                                    ";

			do {
				int i = 0;				
				int j = 0;				

				unsigned int roworder = uFMOD_GetRowOrder();
				int order = (roworder >> 16) & 0xFFFF;
				int row = roworder & 0xFFFF;

				if (row != prevrow) { rowacc++; prevrow = row; }

				float millis = (float)uFMOD_GetTime();

				////////////////////////////////////////////////////////

				glClearColor(0.0f,0.0f,0.0f,1.0f);
				glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

				// RASTA BARS //////////////////////////////////////////
	
				if (rowacc < 1334) { 
					if (order >= 7) {
						glDisable(GL_BLEND);
						glDisable(GL_TEXTURE_2D);
						glLoadIdentity();
				
						if ((rand() & 65535) > 65000)
						{
							glRotatef(20.f, 0.f, 0.f, 1.0f);	
							glScalef(29.f, 29.f, 29.f);
						}
						float fader = 0.0f;

						if (order == 7) {
							fader = row/32.0f;
						} else { fader = 1.0f; }

						for (i = 0; i <= 255; i++) {
							for (j = 0; j < 32; j++) {
								glBegin(GL_QUADS);
								float xp = -2+(j/8.0f);
								float xs = (j/8.0f);
								float r,g,b;
								if (order >= 0 && order < 15) {
									r = 0.5+cos((float)i/32.0f+millis*0.004)*0.7-0.8;
									g = 0.5+sin((float)i/64.0f+millis*0.003)*0.8-0.9;
									b = 0.5+cos((float)i/48.0f+millis*0.0025)*0.7-0.8;
								} else if (order == 15) {
									float xpp = xp * (row / 32.0f);
									r = 0.5+cos((float)i/32.0f+millis*0.004+xpp)*0.7-0.8;
									g = 0.5+sin((float)i/64.0f+millis*0.003+xpp)*0.8-0.9;
									b = 0.5+cos((float)i/48.0f+millis*0.0025+xpp)*0.7-0.8;
								} else {
									r = 0.5+cos((float)i/32.0f+millis*0.004+xp)*0.7-0.8;
									g = 0.5+sin((float)i/64.0f+millis*0.003+xp)*0.8-0.9;
									b = 0.5+cos((float)i/48.0f+millis*0.0025+xp)*0.7-0.8;
								}

								if (order > 18) {
									r = powf(r*(cos(millis*0.004)+b*sin(millis*0.003))*0.3,1.0f);
									g = powf(g*(cos(millis*0.003)+r*sin(millis*0.0025))*0.4,1.0f);
									b = powf(b*(cos(millis*0.0025)+g*sin(millis*0.003))*0.3,1.0f);
								} else {
									r = powf(r*(cos(millis*0.004)+g*sin(millis*0.003))*0.3,1.0f);
									g = powf(g*(cos(millis*0.003)+b*sin(millis*0.0025))*0.4,1.0f);
									b = powf(b*(cos(millis*0.0025)+r*sin(millis*0.003))*0.3,1.0f);
								}

								r*=fader;
								g*=fader;
								b*=fader;

								glColor4f(r,g,b,1.0f);
								glVertex3f(xp,-1+((float)i/128.0f),-1.f);
								glVertex3f(xp+xs,-1+((float)i/128.0f),-1.f);
								glVertex3f(xp+xs,-1.1+((float)i/128.0f),-1.f);						
								glVertex3f(xp,-1.1+((float)i/128.0f),-1.f);
								glEnd();
							}
						}

					} 			

				}	
	
				if (rowacc > 730 && rowacc < 1334) {
					glMatrixMode(GL_PROJECTION);
					glPushMatrix();
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					if (mankelistart == 0) mankelistart = millis;
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);
					glDisable(GL_BLEND);
					glColor4f(1.0f,1.0f,1.0f,1.0f);
					mankeli_display((int)(millis-mankelistart), fminf((float)(rowacc-770)/40.f, 0.f));
					glMatrixMode(GL_MODELVIEW);
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
					glPopMatrix();
						glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);
					glDisable(GL_BLEND);
					glColor4f(1.0f,1.0f,1.0f,1.0f);
				glMatrixMode(GL_MODELVIEW);
				} 
			// cracktro logo ///////////////////////////////////////
				if (order >= 1 && order < 7) {
					if (scrollstartmillis == 0.0f) scrollstartmillis = millis;
					float xscroll = 20.0f-((millis-scrollstartmillis)*0.012f);
					glDisable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);
					//glDisable(GL_TEXTURE_2D);

					for (i = 0; i < 58; i++) {
						for (j = 0; j < 10; j++) {
							float c = 1.0f;
							glLoadIdentity();
							glColor4f(c,c,c,1.0f);
							float yoffset = -8.0f+cos(millis*0.002f+j*0.1f)*4.0f;
							draw_glyph(xscroll+(i/0.5f),yoffset+(j/0.6f),-14,cracktro_data[(10-j)*64+i]-1,0.0f);
						}
					}

					glEnable(GL_BLEND);

					glBlendFunc(GL_DST_COLOR, GL_ONE);
					glBlendEquation(GL_FUNC_ADD);

					for (i = 0; i < 64; i++) {
						for (j = 1; j < 31; j++) {
							float c = 1.0f+cos(j*0.1f*millis*0.01f)*0.3;
							glLoadIdentity();
							glColor4f(c,c,c,1.0f);
							float yoffset = -10.0f+sin(millis*0.002f+j*0.1f)*1.0f;
							draw_glyph(-18.0f+(i/1.7f),yoffset+(j/1.7f),-14,logo_data[(31-j)*64+i]*32,0.0f);
						}
					}
				}

				// NYMME RENDAILLAAN SKROLLAAVAA TEKSTII/////////////////

				if (rowacc < 1300) {
					if (order >= 6) {
						glEnable(GL_BLEND);
						glBlendEquation(GL_SUBTRACT);
						glBlendFunc(GL_SRC_COLOR, GL_ONE);

						glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, font_texture);

						ti = rowacc/1.5f; 

						for (i = 16; i>=0;i--) {
							float r = 0.8+cos((float)i/4.0f+millis*0.004)*0.2-0.05;
							float g = 0.8+sin((float)i/7.0f+millis*0.003)*0.2-0.04;
							float b = 0.8+cos((float)i/6.0f+millis*0.0025)*0.2-0.02;
							glColor4f(r,g,b,1.0f);
							glLoadIdentity();
							draw_glyph(-7 + i*2,(cos(millis*0.0035+(i*2)*0.1)*sin(millis*0.004+(i*1)*0.1))*4.0f,-8+abs((sin(i*0.2+millis*0.003)*cos(i*0.3+millis*0.0035))*5.0),text[(int)ti+i],cos(millis*0.004)*4.0f);
						}
					}		
				}		

				if (order == 0) {
					// GUAGE LOGO PIIRRUSTIN ETTAS TIIAT HOMOOOOOOOOOOOOOOOOO
					glDisable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);
					//glDisable(GL_TEXTURE_2D);

					for (i = 0; i < 41; i++) {
						for (j = 0; j < 41; j++) {
							float c = 1.0f;
							glLoadIdentity();
							float setter = 1.0f;
							if (row > 16) setter = tan((row%16)*1.0f)*40.0f; 
							glTranslatef(0.0f,0.0f,cos(millis*0.01f + setter)*10.0f);
							glColor4f(c,c,c,1.0f);
							draw_glyph(-40.0f+(i/0.5f),-40.0f+(j/0.6f),-60+cos(row+i+j)*1.0f,guage_data[(41-j)*100+i]-1,0.0f);
						}
					}
				}
				// AEGISKUVA
				if (rowacc > 1089 && rowacc < 1111)
				{
					glLoadIdentity();
					glDisable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, aegis_texture);

					float tx = 0.0f;
					float ts = 1.0f;
					float ty = 0.0f;
					float z = -3.5f-cos(millis*0.006)*2.5f;

					glBegin(GL_QUADS);
						glTexCoord2f(tx+0.0f,ty+0.0f);
						glVertex3i(-1,1,z);
						glTexCoord2f(tx+ts,ty);
						glVertex3i(1,1,z);
						glTexCoord2f(tx+ts,ty+ts);
						glVertex3i(1,-1,z);
						glTexCoord2f(tx,ty+ts);
						glVertex3i(-1,-1,z);
					glEnd();
				}
	
				if (rowacc >= 1270 && rowacc < 1926) 
				{
					float yglitch = 0.f;
	
					if (rowacc > 1505 && rowacc < 1530)
					{
						yglitch = ((float)(rand()&255)/255.f) * 30.f - 15.f;
					}

					if (scrollstartmillis2 == 0.0f) scrollstartmillis2 = millis;
					float xscroll = 30.0f-((millis-scrollstartmillis2)*0.014f);
					int xsx = 0;
					if (xscroll <= -48.0f) {
						xsx = (int)((millis-scrollstartmillis2-5500)*0.01f);
						xscroll = -48.0f;
					}
					if(xsx > (500-58))
					{
						uFMOD_SetVolume(uFMOD_MIN_VOL);	
						xsx = 500-58;
					}
					glDisable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);

					for (i = 0; i < 58; i++) {
						for (j = 0; j < 39; j++) {
							float c = 1.0f;
							glLoadIdentity();
							glColor4f(c,c,c,1.0f);
							draw_glyph(xscroll+(i/0.5f),-33.0f+(j/0.57f)+yglitch,-34,greets_data[(39-j)*500+(i+xsx)]-1,0.0f);
						}
					}
				}

				if (rowacc > 1950)
				{
					static int begrow = -1;
					if (begrow == -1)
						begrow = rowacc;
					int thisrow = rowacc - begrow;
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					glDisable(GL_BLEND);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, font_texture);
					//glDisable(GL_TEXTURE_2D);
					//draw_glyph(0.f, 0.f, -10.f, rowacc, 0.f);
					//draw_glyph(1.f, 0.f, -10.f, rowacc, 0.f);
					//draw_glyph(2.f, 0.f, -10.f, rowacc, 0.f);
					//draw_glyph(3.f, 0.f, -10.f, rowacc, 0.f);
					draw_string(-35.0f, 23.0f, 0.3f, "HUMAN MIND +5 BY LHB/HACKERS", 0, 0);
					draw_string(-35.0f, 18.0f, 0.3f, "Sixth sense?", 'Y', thisrow > 19);				
					if (thisrow > 20)
						draw_string(-35.0f, 16.0f, 0.3f, "Enhanced awareness?", 'Y', thisrow > 32);				
					if (thisrow > 33)
						draw_string(-35.0f, 14.0f, 0.3f, "Third eye open?", 'y', thisrow > 39);	
					if (thisrow > 40)
						draw_string(-35.0f, 12.0f, 0.3f, "Transcendency?", 'y', thisrow > 52);			
					if (thisrow > 53)
						draw_string(-35.0f, 10.0f, 0.3f, "Start at level _", '9', thisrow > 60);		
					if (thisrow > 70)
						quit = 1;
					glPopMatrix();
				}



				// END REND ER //////////////////////////////////////////

				SDL_GL_SwapBuffers();

				// POLLI TAHTOO KEKSIN
				SDL_PollEvent(&event);
			} while (event.type!=SDL_KEYDOWN && quit == 0);

////////////////////////////////// coodi

			  SDL_Quit();

			/* Restore normal terminal mode. */
			term.c_lflag |= ICANON | ECHO;
			tcsetattr(0, TCSANOW, &term);

			/* Stop playback. */
			uFMOD_StopSong();
		}else

		/* OpenAL 0.0.7 is eager to hang for a while here.
		   Make sure you're using the latest OpenAL distro
		   available to avoid this kind of bugs. */
		alcMakeContextCurrent(0);
		alcDestroyContext(OAL_context);
		alcCloseDevice(OAL_device);

	}else
	_exit(0);
}
