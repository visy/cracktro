rm ./main
gcc -m32 -Os -nostdlib -lc main.c oalufmod.o -lm -dl -lpthread -lopenal -fomit-frame-pointer -lSDL -lGL /lib/i386-linux-gnu/libc.so.6 /lib/i386-linux-gnu/ld-linux.so.2 -omain
strip -s -R .comment -R .gnu.version main
sstrip main
cp unpack.header 64k
#gzip -cn9 main >> 64k
chmod a+x 64k
