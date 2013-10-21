# Target OS: Linux (i386)
# Driver:    ALSA

mini_alsa: mini_alsa.o alsaufmod.o
	gcc -s -nostdlib -lasound -o mini_alsa mini_alsa.o alsaufmod.o
	strip -R .comment -R .gnu.version mini_alsa
	# sstrip mini_alsa

mini_alsa.o: mini_alsa.s mini.xm
	as -o mini_alsa.o mini_alsa.s
