@echo off
rem A batch file for crosscompiling uFMOD ELF libraries
rem for Linux (i386) in Windows.

rem *** CONFIG START
rem *** Check the Readme docs for a complete reference
rem *** on configuring the following options

rem Pathes:
SET UF_NASM=\nasm
SET UF_FASM=\fasm
SET UF_ARCH=ar

rem Select compiler: NASM or FASM
SET UF_ASM=FASM

rem Select output format: OBJ or LIB
SET UF_FMT=OBJ

rem Select mixing rate: 22050, 44100 or 48000 (22.05 KHz, 44.1 KHz or 48 KHz)
SET UF_FREQ=48000

rem Set volume ramping mode (interpolation): NONE, WEAK or STRONG
SET UF_RAMP=STRONG

rem Set build mode: NORMAL, UNSAFE or BENCHMARK
SET UF_MODE=NORMAL
rem *** CONFIG END

if %UF_ASM%==NASM goto NASM
if %UF_ASM%==FASM goto FASM
echo %UF_ASM% not supported
goto TheEnd

:NASM
if not exist "%UF_NASM%\nasmw.exe" goto Err1
SET UF_ASM=-O5 -t -felf -df%UF_FREQ% -d%UF_RAMP% -d%UF_MODE% -isrc\
"%UF_NASM%\nasmw" %UF_ASM% -dOSS    -oufmod.o     src\nasm.asm
"%UF_NASM%\nasmw" %UF_ASM% -dOPENAL -ooalufmod.o  src\nasm.asm
"%UF_NASM%\nasmw" %UF_ASM% -dALSA   -oalsaufmod.o src\nasm.asm
goto MkLb

:FASM
if not exist "%UF_FASM%\fasm.exe" goto Err2
echo UF_OUTPUT          equ OSS        >tmp.asm
echo UF_FREQ            equ %UF_FREQ% >>tmp.asm
echo UF_RAMP            equ %UF_RAMP% >>tmp.asm
echo UF_MODE            equ %UF_MODE% >>tmp.asm
echo PBASIC             equ 0         >>tmp.asm
echo BLITZMAX           equ 0         >>tmp.asm
echo NOLINKER           equ 0         >>tmp.asm
echo include 'src\fasm.asm'           >>tmp.asm
"%UF_FASM%\fasm" tmp.asm ufmod.o
echo UF_OUTPUT          equ OPENAL     >tmp.asm
echo UF_FREQ            equ %UF_FREQ% >>tmp.asm
echo UF_RAMP            equ %UF_RAMP% >>tmp.asm
echo UF_MODE            equ %UF_MODE% >>tmp.asm
echo PBASIC             equ 0         >>tmp.asm
echo BLITZMAX           equ 0         >>tmp.asm
echo NOLINKER           equ 0         >>tmp.asm
echo include 'src\fasm.asm'           >>tmp.asm
"%UF_FASM%\fasm" tmp.asm oalufmod.o
echo UF_OUTPUT          equ ALSA       >tmp.asm
echo UF_FREQ            equ %UF_FREQ% >>tmp.asm
echo UF_RAMP            equ %UF_RAMP% >>tmp.asm
echo UF_MODE            equ %UF_MODE% >>tmp.asm
echo PBASIC             equ 0         >>tmp.asm
echo BLITZMAX           equ 0         >>tmp.asm
echo NOLINKER           equ 0         >>tmp.asm
echo include 'src\fasm.asm'           >>tmp.asm
"%UF_FASM%\fasm" tmp.asm alsaufmod.o
del tmp.asm

:MkLb
if %UF_FMT%==OBJ goto TheEnd
"%UF_ARCH%" rc libufmod.a     ufmod.o
"%UF_ARCH%" rc liboalufmod.a  oalufmod.o
"%UF_ARCH%" rc libalsaufmod.a alsaufmod.o
del *.o
goto TheEnd

:Err1
echo Couldn't find nasmw.exe in %UF_NASM%\
goto TheEnd
:Err2
echo Couldn't find fasm.exe  in %UF_FASM%\

:TheEnd
pause
@echo on
cls
