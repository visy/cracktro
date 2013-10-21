; uFMOD setup:
%define OSS
%define f48000 ; 48KHz
%define UNSAFE
%define NOLINKER

; ELF format definitions
%define ET_EXEC    2 ; Executable file
%define EM_386     3 ; Intel 386
%define EV_CURRENT 1 ; Current version
%define SHN_UNDEF  0 ; Meaningless section reference
%define PT_LOAD    1 ; Loadable segment
%define PF_R       4 ; Segment is readable
%define PF_W       2 ; Segment is writeable
%define PF_X       1 ; Segment is executable

; Section alignment
%define SEC_ALIGN  1000h

; Switch to 32 bit output
BITS 32

; Image mapped at RVA = 0x08048000
org 8048000h

; ELF header
ehd:
	db 7Fh,'E','L','F',1,1,1,0,0,0,0,0,0,0,0,0 ; e_ident
	dw ET_EXEC                                 ; e_type
	dw EM_386                                  ; e_machine
	dd EV_CURRENT                              ; e_version
	dd _start                                  ; e_entry
	dd phd - $$                                ; e_phoff
	dd 0                                       ; e_shoff
	dd 0                                       ; e_flags
	dw ehdsize                                 ; e_ehsize
	dw phdsize                                 ; e_phentsize
	dw 2                                       ; e_phnum
	dw 0                                       ; e_shentsize
	dw 0                                       ; e_shnum
	dw SHN_UNDEF                               ; e_shstrndx
ehdsize equ $ - ehd

; Program header
phd:
	; text section
	dd PT_LOAD                                 ; p_type
	dd textoffset                              ; p_offset
	dd textstart                               ; p_vaddr
	dd textstart                               ; p_paddr
	dd textsize                                ; p_filesz
	dd textsize                                ; p_memsz
	dd PF_R+PF_X                               ; p_flags
	dd SEC_ALIGN                               ; p_align
phdsize equ $ - phd
	; bss section
	dd PT_LOAD                                 ; p_type
	dd bssoffset                               ; p_offset
	dd bssstart                                ; p_vaddr
	dd bssstart                                ; p_paddr
	dd 0                                       ; p_filesz
	dd bsssize                                 ; p_memsz
	dd PF_W                                    ; p_flags
	dd SEC_ALIGN                               ; p_align

; Let's place the stream right inside the code section.
textstart:
textoffset equ $-$$
xm        incbin "../mini.xm"
xm_length equ $ - xm
splaying  db "Playing song... [Press Enter to quit]",0Ah
msg_size  equ $ - splaying
serror    db "Error",0Ah
err_size  equ $ - serror

; Etry point
_start:

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
	%include "nasm.asm"
