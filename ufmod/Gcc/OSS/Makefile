# Target OS: Linux (i386)
# Driver:    OSS

minimal, Jmp2Pat: minimal.c Jmp2Pat.c dump_blitzxmk_xm.c dump_mini_xm.c gui.h ufmod.o
	gcc -Os -s -nostdlib -lc minimal.c ufmod.o -o minimal
	gcc -Os -s -nostdlib -lc Jmp2Pat.c ufmod.o -o Jmp2Pat `gtk-config --cflags` `gtk-config --libs`
	strip -R .comment -R .gnu.version -R .data minimal
	strip -R .comment -R .gnu.version Jmp2Pat
	# sstrip minimal
	# sstrip Jmp2Pat
