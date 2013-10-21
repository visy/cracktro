# Target OS: Linux (i386)
# Driver:    OSS

libufmod.so: tcl.c ufmod.o
	gcc -Wall -Os -s -shared -o libufmod.so tcl.c ufmod.o
	strip -R .comment -R .gnu.version libufmod.so
	# sstrip libufmod.so
