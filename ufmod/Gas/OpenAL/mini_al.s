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

.globl _start
_start:
	# EBX = 0
	# ESI = device
	# EDI = context
	# EBP = &source
	xor %ebx,%ebx
	xor %edi,%edi
	movl %esp,%ebp

	# Open the preferred device.
	pushl %ebx
	call alcOpenDevice
	test %eax,%eax
	xchg %esi,%eax
	jz error

	# Create a context and make it current.
	pushl %esi
	call alcCreateContext
	xchg %edi,%eax
	pushl %edi
	call alcMakeContextCurrent

	# Generate 1 source for playback.
	pushl %ebp
	pushl $1
	call alGenSources

	# Detect a possible error.
	call alGetError
	test %eax,%eax
	jnz error

	# Start playback.
	pushl (%ebp)
	pushl $XM_MEMORY
	pushl $xm_length
	pushl $xm
	call uFMOD_OALPlaySong
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
	movl %esp,%ebx # <- pointer to termios
	pushl %esp
	pushl $0
	call tcgetattr

	# Disable ICANON and ECHO.
	andl $~10,12(%ebx)
	pushl %ebx
	pushl $0
	pushl $0
	call tcsetattr

	# Wait for user input.
	pushl $1
	pushl %esp
	pushl $0
	call read

	# Restore normal terminal mode.
	orl $10,12(%ebx)
	pushl %ebx
	xor %ebx,%ebx
	pushl %ebx
	pushl %ebx
	call tcsetattr

	# Stop playback.
	call uFMOD_OALPlaySong

cleanup:
	# Release the current context and destroy it (the source gets destroyed as well).
	pushl %ebx
	call alcMakeContextCurrent
	pushl %edi
	call alcDestroyContext

	# Close the actual device.
	pushl %esi
	call alcCloseDevice

exit:
	movl %ebp,%esp # fix stack
	pushl %ebx
	call _exit

error:
	pushl $err_size
	pushl $serror
	pushl $1
	call write
	jmp cleanup
