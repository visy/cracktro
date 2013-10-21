# Target OS: Linux (i386)
# Driver:    ALSA

libalsaufmod.so: tcl_alsa.c alsaufmod.o
	gcc -Wall -Os -s -shared -o libalsaufmod.so tcl_alsa.c alsaufmod.o -lasound
	strip -R .comment -R .gnu.version libalsaufmod.so
	# sstrip libalsaufmod.so
