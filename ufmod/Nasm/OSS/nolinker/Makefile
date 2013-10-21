# Target OS: Linux (i386)
# Driver:    OSS

LIB = ../../../ufmodlib/src/

minimal: minimal.asm ../mini.xm $(LIB)nasm.asm $(LIB)ufmod.asm $(LIB)core.asm $(LIB)eff.inc
	nasm -fbin -t -O5 -i$(LIB) minimal.asm
	chmod +x ./minimal
