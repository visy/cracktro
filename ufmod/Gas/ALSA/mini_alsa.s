XM_MEMORY         = 1
XM_FILE           = 2
XM_NOLOOP         = 8
XM_SUSPENDED      = 16
uFMOD_MIN_VOL     = 0
uFMOD_MAX_VOL     = 25
uFMOD_DEFAULT_VOL = 25

.text

# Let's place the stream right inside the code section.
xm:        .incbin "mini.xm"
xm_length  = .-xm
splaying:  .ascii "Playing song... [Press any key to quit]\n"
msg_size   = .-splaying
serror:    .ascii "Error\n"
err_size   = .-serror

# Place a suitable ALSA PCM device name here
szPCM_dev: .ascii "plughw:0,0\0"

.globl _start
_start:
	movl %esp,%ebp
	xor %ebx,%ebx

	# Start playback.
	pushl $szPCM_dev
	pushl $XM_MEMORY
	pushl $xm_length
	pushl $xm
	call uFMOD_ALSAPlaySong
	test %eax,%eax
	jz error

	pushl $msg_size
	pushl $splaying
	pushl $1
	call write

	# Get current terminal mode.
	pushl %ebx     # \
	pushl %ebx     #  > make more space for termios
	pushl %ebx     # /
	movl %esp,%esi # <- pointer to termios
	pushl %esp
	pushl %ebx
	call tcgetattr

	# Disable ICANON and ECHO.
	andl $~10,12(%esi)
	pushl %esi
	pushl %ebx
	pushl %ebx
	call tcsetattr

	# Wait for user input.
	pushl $1
	pushl %esp
	pushl %ebx
	call read

	# Restore normal terminal mode.
	orl $10,12(%esi)
	pushl %esi
	pushl %ebx
	pushl %ebx
	call tcsetattr

	# Stop playback.
	call uFMOD_ALSAPlaySong

r:
	movl %ebp,%esp # fix stack
	pushl %ebx
	call _exit

error:
	pushl $err_size
	pushl $serror
	pushl $1
	call write
	jmp r
