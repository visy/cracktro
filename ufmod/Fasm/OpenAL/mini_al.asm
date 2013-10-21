format ELF

section '.text' executable

; LIBC
extrn read
extrn write
extrn tcgetattr
extrn tcsetattr
extrn _exit

; OpenAL API
include 'openal32.inc'

; uFMOD (OpenAL)
include 'oalufmod.inc'

; Let's place the stream right inside the code section.
xm         file 'mini.xm'
xm_length  = $ - xm
splaying   db "Playing song... [Press any key to quit]",0Ah
msg_size   = $ - splaying
serror     db "Error",0Ah
err_size   = $ - serror

public _start
_start:
	; EBX = 0
	; ESI = device
	; EDI = context
	; EBP = &source
	xor ebx,ebx
	xor edi,edi
	mov ebp,esp

	; Open the preferred device.
	push ebx
	call alcOpenDevice
	test eax,eax
	xchg eax,esi
	jz error

	; Create a context and make it current.
	push esi
	call alcCreateContext
	xchg eax,edi
	push edi
	call alcMakeContextCurrent

	; Generate 1 source for playback.
	push ebp
	push 1
	call alGenSources

	; Detect a possible error.
	call alGetError
	test eax,eax
	jnz error

	; Start playback.
	push DWORD [ebp]
	push XM_MEMORY
	push xm_length
	push xm
	call uFMOD_OALPlaySong
	test eax,eax
	jz error

	push msg_size
	push splaying
	push 1
	call write

	; Get current terminal mode.
	push ebx    ; \
	push ebx    ;  > make more space for termios
	push ebx    ; /
	mov ebx,esp ; <- pointer to termios
	push esp
	push 0
	call tcgetattr

	; Disable ICANON and ECHO.
	and DWORD [ebx + 12],not 10
	push ebx
	push 0
	push 0
	call tcsetattr

	; Wait for user input.
	push 1
	push esp
	push 0
	call read

	; Restore normal terminal mode.
	or DWORD [ebx + 12],10
	push ebx
	xor ebx,ebx
	push ebx
	push ebx
	call tcsetattr

	; Stop playback.
	call uFMOD_OALPlaySong

cleanup:
	; Release the current context and destroy it (the source gets destroyed as well).
	push ebx
	call alcMakeContextCurrent
	push edi
	call alcDestroyContext

	; Close the actual device.
	push esi
	call alcCloseDevice

exit:
	mov esp,ebp ; restore stack
	push ebx
	call _exit

error:
	; Report an error.
	push err_size
	push serror
	push 1
	call write
	jmp cleanup
