#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "ufmod.h"        /* uFMOD (OSS) */
#include "dump_mini_xm.c" /* The XM file */

/* Sometimes, LIBC requires the following symbols: */
const char environ, __progname;

static const char msg[]   = "Playing song... [Press any key to quit]\n";
static const char error[] = "-ERR: Unable to start playback\n";

void _start(void){
struct termios term;
int c;
	/* Start playback. */
	if(uFMOD_PlaySong(xm, sizeof(xm), XM_MEMORY)){
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
