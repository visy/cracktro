# Target OS: Linux (i386)
# Driver:    ALSA

mini_alsa: mini_alsa.o alsaufmod.o
	gcc -s -nostdlib -o mini_alsa mini_alsa.o alsaufmod.o -lasound -lc
	strip -R .comment -R .gnu.version mini_alsa
	# sstrip mini_alsa

mini_alsa.o: mini_alsa.asm mini.xm
	fasm mini_alsa.asm mini_alsa.o
