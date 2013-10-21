format ELF

section '.text' executable

; LIBC
extrn read
extrn write
extrn tcgetattr
extrn tcsetattr
extrn _exit

; uFMOD (OSS)
include 'ufmod.inc'

; Let's place the stream right inside the code section.
xm         file 'mini.xm'
xm_length  = $ - xm
splaying   db "Playing song... [Press any key to quit]",0Ah
msg_size   = $ - splaying
serror     db "Error",0Ah
err_size   = $ - serror

public _start
_start:
	mov ebp,esp
	xor edi,edi

	; Start playback.
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_PlaySong
	test eax,eax
	jz error

	push msg_size
	push splaying
	push 1
	call write

	; Get current terminal mode.
	push edi    ; \
	push edi    ;  > make more space for termios
	push edi    ; /
	mov esi,esp ; <- pointer to termios
	push esp
	push edi
	call tcgetattr

	; Disable ICANON and ECHO.
	and DWORD [esi + 12],not 10
	push esi
	push edi
	push edi
	call tcsetattr

	; Wait for user input.
	push 1
	push esp
	push edi
	call read

	; Restore normal terminal mode.
	or DWORD [esi + 12],10
	push esi
	push edi
	push edi
	call tcsetattr

	; Stop playback.
	push edi
	push edi
	push edi
	call uFMOD_PlaySong

r:
	mov esp,ebp ; <- fix stack
	push edi
	call _exit

error:
	push err_size
	push serror
	push 1
	call write
	jmp r
