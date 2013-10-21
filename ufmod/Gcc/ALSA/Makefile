# Target OS: Linux (i386)
# Driver:    ALSA

mini_alsa: mini_alsa.c dump_mini_xm.c alsaufmod.o
	gcc -Os -s -nostdlib -lasound -lc mini_alsa.c alsaufmod.o -o mini_alsa
	strip -R .comment -R .gnu.version -R .data mini_alsa
	# sstrip mini_alsa
