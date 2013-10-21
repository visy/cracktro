; MINIMAL.ASM
; -----------
; A pure FASM no-linker example without macros
; and standard includes. Only for real coders ;)

format ELF executable
entry start

; uFMOD setup:
UF_OUTPUT equ OSS    ; Set output driver to OSS     (OSS, ALSA, OPENAL)
UF_FREQ   equ 48000  ; Set sampling rate to 48KHz   (22050, 44100, 48000)
UF_RAMP   equ STRONG ; Select STRONG interpolation  (NONE, WEAK, STRONG)
UD_MODE   equ UNSAFE ; Select UNSAFE mode           (NORMAL, UNSAFE, BENCHMARK)
PBASIC	  equ 0      ; Disable PureBasic extensions
BLITZMAX  equ 0      ; Disable BlitzMax extensions
NOLINKER  equ 1      ; Select "no linker" mode

; uFMOD constants:
XM_MEMORY	  = 1
XM_FILE 	  = 2
XM_NOLOOP	  = 8
XM_SUSPENDED	  = 16
uFMOD_MIN_VOL	  = 0
uFMOD_MAX_VOL	  = 25
uFMOD_DEFAULT_VOL = 25

section readable executable

; Let's place the stream right inside the code section.
xm         file '../mini.xm'
xm_length  = $ - xm
splaying   db "Playing song... [Press Enter to quit]",0Ah
msg_size   = $ - splaying
serror     db "Error",0Ah
err_size   = $ - serror

start:
	; Start playback.
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_PlaySong
	test eax,eax
	jz error

	; syscall write(1, splaying, msg_size)
	mov edx,msg_size
	mov ecx,splaying
	push 4
	xor ebx,ebx
	pop eax
	inc ebx
	int 80h

	; syscall read(0, &tmp, 1)
	push 3
	xor edx,edx
	pop eax
	inc edx
	push eax
	xor ebx,ebx
	mov ecx,esp
	int 80h

	; Stop playback.
	push eax
	push 0
	call uFMOD_PlaySong

r:
	; syscall exit(0)
	xor eax,eax
	xor ebx,ebx
	inc eax
	int 80h

error:
	; syscall write(1, serror, err_size)
	mov edx,err_size
	mov ecx,serror
	push 4
	xor ebx,ebx
	pop eax
	inc ebx
	int 80h
	jmp r

	; Include the whole uFMOD sources here. (Right after
	; your main code to avoid naming conflicts, but stil
	; inside your code section.)
	macro PUBLIC symbol {} ; hide all publics
	include '../../../ufmodlib/src/fasm.asm'
