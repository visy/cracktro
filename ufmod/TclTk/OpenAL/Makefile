# Target OS: Linux (i386)
# Driver:    OpenAL

libufmod.so: tcl_al.c oalufmod.o
	gcc -Wall -Os -s -shared -o liboalufmod.so tcl_al.c oalufmod.o -lopenal
	strip -R .comment -R .gnu.version liboalufmod.so
	# sstrip liboalufmod.so
