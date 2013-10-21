# Make the uFMOD libraries in ELF format
# Target OS: Linux (i386)

# *** CONFIG START
# *** Check the Readme docs for a complete reference
# *** on configuring the following options.

# Select compiler: NASM or FASM
UF_ASM = FASM

# Select output format: OBJ or LIB
UF_FMT = OBJ

# Select mixing rate: 22050, 44100 or 48000 (22.05 KHz, 44.1 KHz or 48 KHz)
UF_FREQ = 48000

# Set volume ramping mode (interpolation): NONE, WEAK or STRONG
UF_RAMP = STRONG

# Set build mode: NORMAL, UNSAFE or BENCHMARK
UF_MODE = NORMAL
# *** CONFIG END

ifeq ($(UF_FMT),LIB)
libufmod.a,libalsaufmod.a,liboalufmod.a: ufmod.o,alsaufmod.o,oalufmod.o
	ar rc libufmod.a     ufmod.o
	ar rc libalsaufmod.a alsaufmod.o
	ar rc liboalufmod.a  oalufmod.o
	rm *.o
endif

ifeq ($(UF_ASM),NASM)
ufmod.o,alsaufmod.o,oalufmod.o: src/nasm.asm src/core.asm src/ufmod.asm src/eff.inc
	nasm -dOSS    -O5 -t -felf -df$(UF_FREQ) -d$(UF_RAMP) -d$(UF_MODE) -isrc/ -oufmod.o     src/nasm.asm
	nasm -dALSA   -O5 -t -felf -df$(UF_FREQ) -d$(UF_RAMP) -d$(UF_MODE) -isrc/ -oalsaufmod.o src/nasm.asm
	nasm -dOPENAL -O5 -t -felf -df$(UF_FREQ) -d$(UF_RAMP) -d$(UF_MODE) -isrc/ -ooalufmod.o  src/nasm.asm
else
ifeq ($(UF_ASM),FASM)
ufmod.o,alsaufmod.o,oalufmod.o: src/fasm.asm src/core.asm src/ufmod.asm src/eff.inc
	echo UF_OUTPUT equ OSS         >tmp.asm
	echo UF_FREQ   equ $(UF_FREQ) >>tmp.asm
	echo UF_RAMP   equ $(UF_RAMP) >>tmp.asm
	echo UF_MODE   equ $(UF_MODE) >>tmp.asm
	echo PBASIC    equ 0          >>tmp.asm
	echo BLITZMAX  equ 0          >>tmp.asm
	echo NOLINKER  equ 0          >>tmp.asm
	echo include \'src/fasm.asm\' >>tmp.asm
	fasm tmp.asm ufmod.o
	echo UF_OUTPUT equ ALSA        >tmp.asm
	echo UF_FREQ   equ $(UF_FREQ) >>tmp.asm
	echo UF_RAMP   equ $(UF_RAMP) >>tmp.asm
	echo UF_MODE   equ $(UF_MODE) >>tmp.asm
	echo PBASIC    equ 0          >>tmp.asm
	echo BLITZMAX  equ 0          >>tmp.asm
	echo NOLINKER  equ 0          >>tmp.asm
	echo include \'src/fasm.asm\' >>tmp.asm
	fasm tmp.asm alsaufmod.o
	echo UF_OUTPUT equ OPENAL      >tmp.asm
	echo UF_FREQ   equ $(UF_FREQ) >>tmp.asm
	echo UF_RAMP   equ $(UF_RAMP) >>tmp.asm
	echo UF_MODE   equ $(UF_MODE) >>tmp.asm
	echo PBASIC    equ 0          >>tmp.asm
	echo BLITZMAX  equ 0          >>tmp.asm
	echo NOLINKER  equ 0          >>tmp.asm
	echo include \'src/fasm.asm\' >>tmp.asm
	fasm tmp.asm oalufmod.o
	rm tmp.asm
else
	echo $(UF_ASM) not supported
endif
endif
