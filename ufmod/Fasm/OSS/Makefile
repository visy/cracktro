# Target OS: Linux (i386)
# Driver:    OSS

minimal: minimal.o ufmod.o
	gcc -s -nostdlib -o minimal minimal.o ufmod.o -lc
	strip -R .comment -R .gnu.version -R .data minimal
	# sstrip minimal

minimal.o: minimal.asm mini.xm
	fasm minimal.asm minimal.o
