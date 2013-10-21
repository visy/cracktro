# Target OS: Linux (i386)
# Driver:    OpenAL

# Link to OpenAL dinamically at runtime? [Y/N]
OPENAL_DYN = N

mini_al: mini_al.c oalufmod.o
ifeq ($(OPENAL_DYN),Y)
	gcc -Os -s -nostdlib -lc mini_al.c oalufmod.o -o mini_al -lc -lm -ldl -lpthread -D OPENAL_DYN
else
	gcc -Os -s -nostdlib -lc mini_al.c oalufmod.o -o mini_al -lc -lm -ldl -lpthread -lopenal
endif
	strip -R .comment -R .gnu.version mini_al
	# sstrip mini_al
