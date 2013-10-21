/* GUI.H
   -----
   A common include for compatibility with GTK 1.x and 2+.
*/

#include <gtk/gtk.h>

#if GTK_MAJOR_VERSION < 2
	/* GTK 1.x */
	#define set_button_label(button, label) \
		gtk_label_set_text(GTK_LABEL(GTK_BIN(button)->child), label)
	#define window_prevent_resize(window) \
		gtk_window_set_policy(GTK_WINDOW(window), 0, 0, 1)
#else
	/* GTK 2+ */
	#define set_button_label(button, label) \
		gtk_button_set_label(GTK_BUTTON(button), label)
	#define window_prevent_resize(window) \
		gtk_window_set_resizable(GTK_WINDOW(window), 0)
#endif
