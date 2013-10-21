/*
  JMP2PAT.C
  ---------
  Sometimes it makes sense merging various XM tracks
  sharing the same instruments in a single XM file.
  This example program uses such an XM file actually
  containing 3 tracks and the uFMOD_Jump2Pattern()
  function to play all 3 tracks in the same file.

  BLITZXMK.XM tracked by Kim (aka norki):
    [00:07] - track #1
    [08:10] - track #2
    [11:13] - track #3
*/

#include <unistd.h>
#include "gui.h"              /* GTK wrapper */
#include "ufmod.h"            /* uFMOD (OSS) */
#include "dump_blitzxmk_xm.c" /* The XM file */

/* Sometimes, LIBC requires the following symbols: */
const char environ, __progname;

static const char error[] = "-ERR: Unable to start playback\n";
unsigned int paused = 0, track[] = {0, 8, 11}; /* Preset pattern indexes */

/* Jump to the given pattern index. */
void play_pattern(GtkWidget *widget, int *data){ uFMOD_Jump2Pattern(*data); }

/* Pause/Resume playback. */
void pause_resume(GtkWidget *widget){
	if(paused){ uFMOD_Resume(); set_button_label(widget, "Pause");  }
	else{       uFMOD_Pause();  set_button_label(widget, "Resume"); }
	paused ^= 1;
}

/* Stop playback and break the main loop. */
void quit(){ uFMOD_StopSong(); gtk_main_quit(); }

void _start(void){
GtkWidget *window, *box, *button;

	/* Preload the GUI */
	gtk_init(0, 0);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Jump2Pattern test");
	gtk_container_border_width(GTK_CONTAINER(window), 10);
	box = gtk_hbox_new(0, 5);
	gtk_container_add(GTK_CONTAINER(window), box);
	button = gtk_button_new_with_label("1");
	gtk_widget_set_usize(button, 30, 30);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
	   GTK_SIGNAL_FUNC(play_pattern), track);     /* Play Track #1 */
	gtk_box_pack_start(GTK_BOX(box), button, 0, 0, 0);
	gtk_widget_show(button);
	button = gtk_button_new_with_label("2");
	gtk_widget_set_usize(button, 30, 30);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
	   GTK_SIGNAL_FUNC(play_pattern), track + 1); /* Play Track #2 */
	gtk_box_pack_start(GTK_BOX(box), button, 0, 0, 0);
	gtk_widget_show(button);
	button = gtk_button_new_with_label("3");
	gtk_widget_set_usize(button, 30, 30);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
	   GTK_SIGNAL_FUNC(play_pattern), track + 2); /* Play Track #3 */
	gtk_box_pack_start(GTK_BOX(box), button, 0, 0, 0);
	gtk_widget_show(button);
	button = gtk_button_new_with_label("Pause");
	gtk_widget_set_usize(button, 72, 30);
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
	   GTK_SIGNAL_FUNC(pause_resume), 0);         /* Pause/Resume */
	gtk_box_pack_start(GTK_BOX(box), button, 0, 0, 0);
	gtk_widget_show(button);
	gtk_signal_connect(GTK_OBJECT(window), "destroy",
	   GTK_SIGNAL_FUNC(quit), 0);                 /* Exit on close */
	gtk_widget_show(box);
	window_prevent_resize(window);

	/* Start playback */
	if(uFMOD_PlaySong(xm, sizeof(xm), XM_MEMORY)){

		/* Show the GUI and start processing events. */
		gtk_widget_show(window);
		gtk_main();
	}else
		write(1, error, sizeof(error) - 1);
	_exit(0);
}
