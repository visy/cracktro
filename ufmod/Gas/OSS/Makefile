# Target OS: Linux (i386)
# Driver:    OSS

minimal: minimal.o ufmod.o
	gcc -s -nostdlib -lc -o minimal minimal.o ufmod.o
	strip -R .comment -R .gnu.version minimal
	# sstrip minimal

minimal.o: minimal.s mini.xm
	as -o minimal.o minimal.s
