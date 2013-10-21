#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "alsaufmod.h"    /* uFMOD (ALSA) */
#include "dump_mini_xm.c" /* The XM file */

/* Sometimes, LIBC requires the following symbols: */
const char environ, __progname;

static const char msg[]   = "Playing song... [Press any key to quit]\n";
static const char error[] = "Error\n";

void _start(void){
struct termios term;
int c;
	/* Start playback. */
	if(uFMOD_ALSAPlaySong(xm, sizeof(xm), XM_MEMORY, "plughw:0,0")){
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
		write(1, error, sizeof(error) - 1);
	_exit(0);
}
