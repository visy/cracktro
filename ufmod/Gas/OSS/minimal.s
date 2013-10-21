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
splaying:  .ascii "Playing song... [Press Enter to quit]\n"
msg_size   = .-splaying

serror:    .ascii "Error\n"
err_size   = .-serror

.globl _start
_start:
	# Start playback.
	pushl $XM_MEMORY
	pushl $xm_length
	pushl $xm
	call uFMOD_PlaySong
	test %eax,%eax
	jz error

	# syscall write(1, splaying, msg_size)
	movl $msg_size,%edx
	movl $splaying,%ecx
	pushl $4
	xor %ebx,%ebx
	popl %eax
	incl %ebx
	int $0x80

	# syscall read(0, &tmp, 1)
	pushl $3
	xor %edx,%edx
	popl %eax
	incl %edx
	pushl %eax
	xor %ebx,%ebx
	movl %esp,%ecx
	int $0x80

	# Stop playback.
	pushl %eax
	pushl $0
	call uFMOD_PlaySong

r:
	# syscall exit(0)
	xor %eax,%eax
	xor %ebx,%ebx
	incl %eax
	int $0x80

error:
	# syscall write(1, serror, err_size)
	movl $err_size,%edx
	movl $serror,%ecx
	pushl $4
	xor %ebx,%ebx
	popl %eax
	incl %ebx
	int $0x80
	jmp r
