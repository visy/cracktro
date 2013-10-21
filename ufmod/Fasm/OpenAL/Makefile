# Target OS: Linux (i386)
# Driver:    OpenAL

mini_al: mini_al.o oalufmod.o
	gcc -s -nostdlib -o mini_al mini_al.o oalufmod.o -lopenal -lc -lm -ldl -lpthread
	strip -R .comment -R .gnu.version mini_al
	# sstrip mini_al

mini_al.o: mini_al.asm mini.xm
	fasm mini_al.asm mini_al.o
