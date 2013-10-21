; UFMOD.ASM
; ---------
; uFMOD public source code release. Provided as-is.

FSOUND_Block      equ 10
fragments         equ 4
fragmentsmask     equ 0Fh  ; (2^fragments)-1
FSOUND_BlockSize  equ 1024 ; 1 << FSOUND_Block
totalblocks       equ (fragmentsmask+1)
FSOUND_BufferSize equ (FSOUND_BlockSize)*totalblocks

DEBUG_ON equ 0
if DEBUG_ON
; For debugging purposes only
print_eax:
; EAX - val
	pushad
	mov esi,eax
	mov edx,MixBuf
	mov ecx,7
	mov BYTE [edx+8],10
print_eax_loop:
	mov eax,esi
	and al,0Fh
	cmp al,10
	sbb al,69h
	das
	mov [edx+ecx],al
	shr esi,4
	dec ecx
	jns print_eax_loop
	mov ecx,edx
	mov ebx,1
	mov edx,9
	mov eax,4
	int 80h
	popad
	ret
endif

if RAMP_STRONG
	volumerampsteps   equ 128
	volumeramps_pow   equ 7
endif

if RAMP_WEAK
	volumerampsteps   equ 16
	volumeramps_pow   equ 4
endif

if RAMP_NONE
	volumerampsteps   equ 64
	volumeramps_pow   equ 6
endif

XM_MEMORY                 equ 1
XM_FILE                   equ 2
XM_NOLOOP                 equ 8
XM_SUSPENDED              equ 16
FMUSIC_ENVELOPE_SUSTAIN   equ 2
FMUSIC_ENVELOPE_LOOP      equ 4
FMUSIC_FREQ               equ 1
FMUSIC_VOLUME             equ 2
FMUSIC_PAN                equ 4
FMUSIC_TRIGGER            equ 8
FMUSIC_VOLUME_OR_FREQ     equ 3
FMUSIC_VOLUME_OR_PAN      equ 6
FMUSIC_VOL_OR_FREQ_OR_TR  equ 11
FMUSIC_FREQ_OR_TRIGGER    equ 9
NOT_FMUSIC_TRIGGER        equ 0F7h
NOT_FMUSIC_TRIGGER_OR_FRQ equ 0F6h

; FMUSIC_XMCOMMANDS enum:
FMUSIC_XM_PORTAUP         equ 1
FMUSIC_XM_PORTADOWN       equ 2
FMUSIC_XM_PORTATO         equ 3
FMUSIC_XM_VIBRATO         equ 4
FMUSIC_XM_PORTATOVOLSLIDE equ 5
FMUSIC_XM_VIBRATOVOLSLIDE equ 6
FMUSIC_XM_TREMOLO         equ 7
FMUSIC_XM_SETPANPOSITION  equ 8
FMUSIC_XM_SETSAMPLEOFFSET equ 9
FMUSIC_XM_VOLUMESLIDE     equ 10
FMUSIC_XM_PATTERNJUMP     equ 11
FMUSIC_XM_SETVOLUME       equ 12
FMUSIC_XM_PATTERNBREAK    equ 13
FMUSIC_XM_SPECIAL         equ 14
FMUSIC_XM_SETSPEED        equ 15
FMUSIC_XM_SETGLOBALVOLUME equ 16
FMUSIC_XM_GLOBALVOLSLIDE  equ 17
FMUSIC_XM_KEYOFF          equ 20
FMUSIC_XM_PANSLIDE        equ 25
FMUSIC_XM_MULTIRETRIG     equ 27
FMUSIC_XM_TREMOR          equ 29
FMUSIC_XM_EXTRAFINEPORTA  equ 33

; FMUSIC_XMCOMMANDSSPECIAL enum:
FMUSIC_XM_FINEPORTAUP      equ 1
FMUSIC_XM_FINEPORTADOWN    equ 2
FMUSIC_XM_SETGLISSANDO     equ 3
FMUSIC_XM_SETVIBRATOWAVE   equ 4
FMUSIC_XM_SETFINETUNE      equ 5
FMUSIC_XM_PATTERNLOOP      equ 6
FMUSIC_XM_SETTREMOLOWAVE   equ 7
FMUSIC_XM_SETPANPOSITION16 equ 8
FMUSIC_XM_RETRIG           equ 9
FMUSIC_XM_NOTECUT          equ 12
FMUSIC_XM_NOTEDELAY        equ 13
FMUSIC_XM_PATTERNDELAY     equ 14

	uFMOD_mem dd mem_open, mem_read
if XM_FILE_ON
	          dd mem_open
	uFMOD_fs  dd file_open, file_read, file_close
endif

if DRIVER_OSS
	OSS_driver db "/dev/dsp",0 ; uses 8-bit unsigned linear encoding
endif

if JUMP_TO_PAT_ON

	; Jump to the given pattern
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAJump2Pattern
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALJump2Pattern
		EXPORTcc DRIVER_OSS,    PB_uFMOD_Jump2Pattern
	else
		PUBLIC uFMOD_Jump2Pattern
		uFMOD_Jump2Pattern:
		mov eax,[esp+4]
	endif
	mov ecx,_mod+36
	movzx eax,ax
	and DWORD [ecx+FMUSIC_MODULE.nextrow-36],0
	cmp ax,[ecx+FMUSIC_MODULE.numorders-36]
	sbb edx,edx
	and eax,edx
	mov [ecx+FMUSIC_MODULE.nextorder-36],eax
	ret
endif

if VOL_CONTROL_ON

	; Set global volume [0: silence, 25: max. volume]
	vol_scale dw 1     ; -45 dB
	          dw 130   ; -24
		  dw 164   ; -23
	          dw 207   ; -22
	          dw 260   ; -21
	          dw 328   ; -20
	          dw 413   ; -19
	          dw 519   ; -18
	          dw 654   ; -17
	          dw 823   ; -16
	          dw 1036  ; -15
	          dw 1305  ; -14
	          dw 1642  ; -13
	          dw 2068  ; -12
	          dw 2603  ; -11
	          dw 3277  ; -10
	          dw 4125  ; -9
	          dw 5193  ; -8
	          dw 6538  ; -7
	          dw 8231  ; -6
	          dw 10362 ; -5
	          dw 13045 ; -4
	          dw 16423 ; -3
	          dw 20675 ; -2
	          dw 26029 ; -1
	          dw 32768 ; 0 dB
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSASetVolume
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALSetVolume
		EXPORTcc DRIVER_OSS,    PB_uFMOD_SetVolume
	else
		PUBLIC uFMOD_SetVolume
		uFMOD_SetVolume:
		mov eax,[esp+4]
	endif
		cmp eax,25
		jna get_vol_scale
		push 25
		pop eax
	get_vol_scale:
		mov ax,[vol_scale+eax*2]
		mov [ufmod_vol],eax
		ret
endif

if PAUSE_RESUME_ON

	; Pause the currently playing song.
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAPause
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALPause
		EXPORTcc DRIVER_OSS,    PB_uFMOD_Pause
	else
		PUBLIC uFMOD_Pause
		uFMOD_Pause:
	endif
	mov al,1
	jmp $+4

	; Resume the currently paused song.
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAResume
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALResume
		EXPORTcc DRIVER_OSS,    PB_uFMOD_Resume
	else
		PUBLIC uFMOD_Resume
		uFMOD_Resume:
	endif
	xor eax,eax
	mov BYTE [ufmod_pause_],al
	ret
endif

if INFO_API_ON

; Return currently playing signal stats:
;    lo word : RMS volume in R channel
;    hi word : RMS volume in L channel
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAGetStats
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALGetStats
		EXPORTcc DRIVER_OSS,    PB_uFMOD_GetStats
	else
		PUBLIC uFMOD_GetStats
		uFMOD_GetStats:
	endif
	push 8
	jmp $+4

; Return currently playing row and order:
;    lo word : row
;    hi word : order
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAGetRowOrder
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALGetRowOrder
		EXPORTcc DRIVER_OSS,    PB_uFMOD_GetRowOrder
	else
		PUBLIC uFMOD_GetRowOrder
		uFMOD_GetRowOrder:
	endif
	push 12
	pop ecx
	if DRIVER_OSS
		mov eax,[RealBlock]
		shl eax,fragments+3 ; x totalblocks x FMUSIC_STATS_size
		xor edx,edx
		cmp DWORD [oss_numblocks],edx
		je oss_numblock_z
		div DWORD [oss_numblocks]
	oss_numblock_z:
		mov eax,DWORD [eax+ecx+tInfo-8]
	else
		mov edx,RealBlock
		mov eax,[edx]
		add edx,ecx
		mov eax,[edx+eax*FMUSIC_STATS_size]
	endif
	ret

; Return the time in milliseconds since the song was started.
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAGetTime
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALGetTime
		EXPORTcc DRIVER_OSS,    PB_uFMOD_GetTime
	else
		PUBLIC uFMOD_GetTime
		uFMOD_GetTime:
	endif
	mov eax,[time_ms]
	ret

; Return the currently playing track title, if any.
	if PBASIC
		EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAGetTitle
		EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALGetTitle
		EXPORTcc DRIVER_OSS,    PB_uFMOD_GetTitle
		push esi
		push edi
		push 6
		mov esi,szTtl
		mov edi,[PB_StringBase]
		pop ecx
		mov eax,edi
		rep movsd
		pop edi
		pop esi
		ret
	endif

endif

; Dynamic memory allocation
alloc:
; EAX: how many bytes to allocate
	add eax,3
	and eax,-4
	jle alloc_error2
	mov ecx,ufmod_heap
alloc_lookup:
	cmp DWORD [ecx],0
	je do_alloc
	mov ecx,[ecx]
	cmp [ecx+4],eax
	jl alloc_lookup
	sub [ecx+4],eax
	mov eax,[ecx+4]
	lea eax,[eax+ecx+12]
	ret
do_alloc:
	add eax,12
	push edi
	push ebx
	xor edi,edi
	mov edx,eax
	push edi ; offset
	add edx,8191
	push -1  ; fd
	and edx,-8192
	push 22h ; flags <- MAP_ANONYMOUS | MAP_PRIVATE
	push 3   ; prot <- PROT_READ | PROT_WRITE
	push edx ; length
	push edi ; start
	mov ebx,esp
	push edx
	sub edx,eax
	lea eax,[edi+90]
	push edx
	int 80h ; mmap
	pop edx ; free space
	pop ebx ; total block size
	add esp,24
	; Test for error condition
	cmp eax,-4096
	ja alloc_error1
	mov [ecx],eax
	mov edi,eax
	mov ecx,ebx
	lea eax,[eax+edx+12]
	push eax
	xor eax,eax
	shr ecx,2
	mov [edi],eax
	mov [edi+4],edx
	mov [edi+8],ebx
	sub ecx,3
	add edi,12
	rep stosd
	pop eax
	pop ebx
	pop edi
	ret
alloc_error1:
	pop ebx
	pop edi
alloc_error2:
	xor eax,eax
	pop edx ; EIP
	pop ebx
	leave
	ret

; ***********************
; * XM_MEMORY CALLBACKS *
; ***********************
mem_read:
; buf in EAX
; size in EDX
	push edi
	push esi
	xchg eax,edi ; buf
	mov esi,mmf
	lodsd
	mov ecx,edx
	add edx,[esi]
	cmp edx,eax
	jl copy
	sub eax,[esi]
	xchg eax,ecx
copy:
	test ecx,ecx
	jle mem_read_R
	lodsd
	add eax,[esi]
	mov [esi-4],edx
mem_do_copy:
	mov dl,[eax]
	mov [edi],dl
	inc eax
	inc edi
	dec ecx
	jnz mem_do_copy
mem_read_R:
	pop esi
	pop edi
if INFO_API_ON
	if PBASIC
	else
		PUBLIC uFMOD_GetTitle
		uFMOD_GetTitle:
		mov eax,szTtl
	endif
endif
mem_open:
	ret

; *********************
; * XM_FILE CALLBACKS *
; *********************
if XM_FILE_ON
file_open:
; pszName in ESI
	push ebx
	xor ecx,ecx ; flags <= O_RDONLY
	mov ebx,esi
	push 5
	pop eax ; open
	int 80h
	pop ebx
	push -1
	push 1
	mov edx,mmf
	mov [SW_Exit],eax
	pop DWORD [edx+8] ; cache offset
	pop DWORD [edx]   ; maximum size
	ret

do_fread:
; file offset             in ECX
; buffer                  in EDI
; number of bytes to read in ESI
	push ebx
	xor edx,edx
	mov ebx,[SW_Exit] ; fd
	lea eax,[edx+19]  ; #19 = lseek
	int 80h
	mov edx,esi
	push 3
	mov ecx,edi
	pop eax           ; #3 = read
	int 80h
	pop ebx
	ret

file_read:
; buf in EAX
; size in EDX
	push ebx
	push esi
	push edi
	push ebp
	xchg eax,edi
file_read_begin:
	test edx,edx
	jg file_read_chk_cache
file_read_done:
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret
	; *** CHECK IN THE CACHE
file_read_chk_cache:
	mov ebp,mmf+4
	mov esi,[ebp]
	sub esi,[ebp+4] ; cache_offset
	js file_read_cache_done
	mov ecx,8192
	sub ecx,esi
	jle file_read_cache_done
	add esi,MixBuf
	sub edx,ecx
	jns file_read_do_cache
	add ecx,edx
file_read_do_cache:
	add [ebp],ecx
	rep movsb
	test edx,edx
	jle file_read_done ; data read from the cache (no need to access the FS)
file_read_cache_done:
	; *** FS BATCH READ OPERATION
	mov ecx,edx
	add ecx,[ebp]
	and ecx,0FFFFE000h
	sub ecx,[ebp]
	jle file_read_fs_done ; Too few data requested for a FS batch operation
	sub edx,ecx
	mov esi,ecx           ; number of bytes to read
	push edx
	mov eax,[ebp]
	add [ebp],ecx
	xchg eax,ecx          ; file offset
	call do_fread
	add edi,esi
	pop edx
file_read_fs_done:
	; *** UPDATE THE CACHE
	mov ecx,[ebp]
	push edi
	and ecx,0FFFFE000h ; file offset
	push edx
	mov [ebp+4],ecx    ; cache_offset
	mov esi,8192       ; number of bytes to read
	mov edi,MixBuf     ; buffer
	call do_fread
	pop edx
	pop edi
	jmp file_read_begin

file_close:
	push ebx
	push 6
	mov ebx,[SW_Exit] ; fd
	pop eax ; close
	int 80h
	pop ebx
	ret

endif

uFMOD_lseek:
; pos  in EAX
; org  in ECX
; !org in Z
	mov edx,mmf+4
	jz mem_ok
	add eax,[edx]
mem_ok:
	test eax,eax
	js mem_seek_R
	cmp eax,[edx-4]
	ja mem_seek_R
	mov [edx],eax
mem_seek_R:
	ret

; Starts playing a song.
if PBASIC
	EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAPlaySong
	EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALPlaySong
	EXPORTcc DRIVER_OSS,    PB_uFMOD_PlaySong
	push eax
else
	EXPORTcc DRIVER_ALSA,   uFMOD_ALSAPlaySong
	EXPORTcc DRIVER_OPENAL, uFMOD_OALPlaySong
	EXPORTcc DRIVER_OSS,    uFMOD_PlaySong
endif
	; *** FREE PREVIOUS TRACK, IF ANY
	call uFMOD_FreeSong
	if DRIVER_OPENAL
		; *** GENERATE SOME STREAMING BUFFERS
		push databuf     ; *buffers
		push totalblocks ; n
		call alGenBuffers
		pop eax
		mov edx,[esp+20] ; source
		pop eax
		mov [FileDSP],edx
	endif
	mov eax,[esp+8]  ; param
	mov [mmf],eax
	mov ecx,[esp+12] ; fdwSong
if PBASIC
	pop edx
else
	mov edx,[esp+4] ; lpXM
endif
	test edx,edx
	jz mem_seek_R
	; *** SET I/O CALLBACKS
	push ebx
	push esi
	push edi
	push ebp
	mov ebp,uFMOD_fopen
	xor eax,eax
	mov [ebp-16],eax ; mmf+4
	test cl,XM_MEMORY
	mov esi,uFMOD_mem
if XM_FILE_ON
	jnz set_callbacks
	test cl,XM_FILE
	lea esi,[esi+(uFMOD_fs-uFMOD_mem)]
endif
	jz uFMOD_FreeSong+4
set_callbacks:
if NOLOOP_ON
	test cl,XM_NOLOOP
	setnz [ebp-24] ; ufmod_noloop
endif
if PAUSE_RESUME_ON
	and cl,XM_SUSPENDED
	mov [ebp-23],cl ; ufmod_pause_
endif
	mov edi,ebp ; uFMOD_fopen
	movsd
	movsd
if XM_FILE_ON
	movsd
endif
	mov esi,edx ; uFMOD_fopen:lpXM <= ESI
if VOL_CONTROL_ON
	cmp [ebp-4],eax ; ufmod_vol
	jne play_vol_ok
	mov WORD [ebp-4],32768
play_vol_ok:
endif
if DRIVER_OSS
	; OPEN OSS DRIVER
	xchg eax,ecx
	push 5
	inc ecx            ; flags <= O_WRONLY
	mov ebx,OSS_driver ; pathname
	pop eax ; open
	int 80h
	mov [FileDSP],eax
	mov ebx,eax ; fd
	inc eax
	jz uFMOD_FreeSong+4
endif
if DRIVER_ALSA
	; OPEN ALSA DRIVER
	mov ebx,FileDSP
	push 0              ; mode
	push 0              ; stream <= SND_PCM_STREAM_PLAYBACK
if PBASIC
	push DWORD [esp+36] ; name   <= szPCM_dev
else
	push DWORD [esp+40] ; name   <= szPCM_dev
endif
	push ebx            ; pcmp
	call snd_pcm_open
	add esp,16
	test eax,eax
	js uFMOD_FreeSong+4
endif
	; LOAD NEW TRACK
	mov [ebp-12],esi ; mmf+8 <= pMem
if XM_FILE_ON
	call DWORD [ebp] ; uFMOD_fopen
endif
	call LoadXM
if XM_FILE_ON
	xchg eax,edi
	call DWORD [ebp+8] ; uFMOD_fclose
	test edi,edi
else
	test eax,eax
endif
	jz uFMOD_FreeSong+4
if DRIVER_OSS
	; *** INIT THE OSS DRIVER
	mov edx,ebp
	; Set the buffering parameters (OSS may ignore this call):
	; maximum number of fragments = totalblocks
	; fragment size (in bytes)    = 2^(FSOUND_Block + 2)
	push 54
	mov ecx,0C004500Ah ; request <= SNDCTL_DSP_SETFRAGMENT
	pop eax            ; ioctl
	mov DWORD [ebp],(totalblocks * 10000h) + FSOUND_Block + 2
	push eax
	int 80h
	pop eax
	push 16
	add ecx,-5         ; request <= SNDCTL_DSP_SAMPLESIZE (0C0045005h)
	mov edx,ebp
	pop DWORD [ebp]    ; argp
	push eax
	int 80h
	mov edx,ebp
	pop eax
	cmp DWORD [ebp],16
	jne goto_freesong
	dec ecx
	shr DWORD [edx],4
	dec ecx            ; request <= SNDCTL_DSP_STEREO (0C0045003h)
	push eax
	int 80h
	mov edx,ebp
	pop eax
	cmp DWORD [ebp],1
	jne goto_freesong
	mov DWORD [edx],FSOUND_MixRate
	dec ecx            ; request <= SNDCTL_DSP_SPEED (0C0045002h)
	push eax
	int 80h
	cmp DWORD [ebp],FSOUND_MixRate
	pop eax
goto_freesong:
	jne uFMOD_FreeSong+4
	mov edx,thread_stack
	mov ecx,800C500Ch ; request <= SNDCTL_DSP_GETOSPACE
	and DWORD [edx+12],0
	int 80h
	test eax,eax
	jns oss_getbuffersize
	; Some old ALSA-emulated OSS drivers don-t support GETOSPACE
	mov edx,totalblocks
	jmp oss_buffersize_ok
oss_getbuffersize:
	mov edx,[thread_stack+12] ; audio_buf_info.bytes (buffer size in bytes)
	sar edx,FSOUND_Block + 2  ; num. blocks = OSS buffer size / uFMOD block size
	cmp edx,1
	jle uFMOD_FreeSong+4      ; buffer too small
oss_buffersize_ok:
	mov [ebp+12],edx ; oss_numblocks
endif
if DRIVER_ALSA
	; *** INIT THE ALSA DRIVER
	lea esi,[ebx-(FileDSP-thread_stack)] ; *dir   <= thread_stack
	lea edi,[ebx+12]                     ; params <= MixBuf
	mov ebx,[ebx]
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_any
	pop edx
	test eax,eax
	pop edx
	js uFMOD_FreeSong+4
	push 3   ; SND_PCM_ACCESS_RW_INTERLEAVED
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_set_access
	add esp,12
	test eax,eax
	js uFMOD_FreeSong+4
	push 2   ; SND_PCM_FORMAT_S16_LE
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_set_format
	add esp,12
	test eax,eax
	js uFMOD_FreeSong+4
	and DWORD [esi],0
	mov DWORD [ebp],FSOUND_MixRate
	push esi ; *dir
	push ebp ; *val
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_set_rate_min
	add esp,16
	test eax,eax
	js uFMOD_FreeSong+4
	push 2   ; val
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_set_channels
	add esp,12
	test eax,eax
	js uFMOD_FreeSong+4
	and DWORD [esi],0
	push esi ; *dir
	push ebp ; *val
	push edi ; params
	push 2
	pop DWORD [ebp]
	push ebx ; pcm
	call snd_pcm_hw_params_set_periods_min
	mov DWORD [ebp],FSOUND_BufferSize
	push ebp ; *val
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params_set_buffer_size_min
	push edi ; params
	push ebx ; pcm
	call snd_pcm_hw_params
	add esp,36
	test eax,eax
	js uFMOD_FreeSong+4
endif
	xor edi,edi
	; *** PREFILL THE MIXER BUFFER
loop_3:
if DRIVER_OPENAL
	mov eax,[uFMOD_FillBlk]
	mov eax,[ebp+eax*4+12] ; databuf
	mov [mmt],eax
	call alGetError
	test eax,eax
	jnz uFMOD_FreeSong+4
endif
	call uFMOD_SW_Fill
	cmp [uFMOD_FillBlk],edi
	jnz loop_3
	mov [SW_Exit],edi
	; *** CREATE THE THREAD
	xor edx,edx        ; pt_regs (context)
	mov ecx,thread_top ; child_stack
if DRIVER_OPENAL
	push ecx ; != 0
else
	push ebx
endif
	mov ebx,511h       ; flags <= CLONE_VM | CLONE_FILES | SIGCHLD
	push 120
	pop eax ; clone
	int 80h
	test eax,eax
	jz uFMOD_Thread_EP ; goto child process (aka thread)
	mov [TID],eax
	pop eax
	js uFMOD_FreeSong+4
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret
uFMOD_Thread_EP:
	call uFMOD_Thread
	xor eax,eax
	inc eax ; exit
	int 80h

; Stop the currently playing song, if any, and free all resources allocated for that song.
if PBASIC
	EXPORTcc DRIVER_ALSA,   PB_uFMOD_ALSAStopSong
	EXPORTcc DRIVER_OPENAL, PB_uFMOD_OALStopSong
	EXPORTcc DRIVER_OSS,    PB_uFMOD_StopSong
endif
uFMOD_FreeSong:
	push ebx
	push esi
	push edi
	push ebp
; uFMOD_FreeSong+4
	; *** STOP THE THREAD
	mov ebp,TID
	mov ebx,[ebp]
	test ebx,ebx
	jle thread_finished
	mov DWORD [ebp+12],ebx ; SW_Exit
	; Wait for thread to finish
	xor ecx,ecx ; status
	xor edx,edx ; options
	push 7
	pop eax ; waitpid
	int 80h
thread_finished:
	xor edi,edi
if BENCHMARK_ON
	; *** RESET THE PERFORMANCE COUNTER
	mov [uFMOD_tsc],edi
endif
	; *** STOP, RESET AND CLOSE THE SOUND DRIVER
	mov [ebp],edi   ; TID
	mov ebx,[ebp+4] ; FileDSP
	mov [ebp+8],edi ; uFMOD_FillBlk
	test ebx,ebx
	jz snd_closed
	mov [ebp+4],edi
if DRIVER_OSS
	; *** CLOSE THE OSS DRIVER
	push 6
	pop eax ; close
	int 80h
endif
if DRIVER_ALSA
	; *** CLOSE THE ALSA DRIVER
	push ebx
	call snd_pcm_drop
	call snd_pcm_close
	pop eax
endif
if DRIVER_OPENAL
	; *** CLOSE THE OPENAL DRIVER
	push edi          ; value = NULL
	push 1009h        ; param = AL_BUFFER
	push ebx          ; source
	call alSourceStop
	call alSourcei
	push databuf      ; *buffers
	push totalblocks  ; n
	call alDeleteBuffers
	add esp,20
endif
snd_closed:
	; *** FREE THE HEAP
	mov edi,[ebp-4]   ; ufmod_heap
heapfree:
	test edi,edi
	jz free_R
	mov ecx,[edi+8]
	mov ebx,edi
	mov edi,[edi]
	mov eax,91
	int 80h ; munmap
	jmp heapfree
free_R:
	xor eax,eax
if INFO_API_ON
	; *** CLEAR THE RealBlock, time_ms, VU ARRAY AND szTtl
	mov ecx,FMUSIC_STATS_size*totalblocks/4+3
	mov edi,RealBlock
	rep stosd
endif
	mov DWORD [ebp-4],eax ; ufmod_heap
	pop ebp
	pop edi
	pop esi
	pop ebx
	ret

uFMOD_Thread:
	push ebp
	push esi
	mov ebp,mmt
thread_loop_1:
if DRIVER_OPENAL
	; *** RESUME ON STARTUP AND BUFFER UNDERRUNS
	push ebp            ; *value = &mmt
	push 1010h          ; pname  = AL_SOURCE_STATE
	push DWORD [ebp+20] ; source = FileDSP
	call alGetSourcei
	add esp,12
	; if(state != AL_PLAYING) alSourcePlay(source);
	cmp DWORD [ebp],1012h
	je source_playing
	push DWORD [ebp+20] ; source = FileDSP
	call alSourcePlay
	pop eax
source_playing:
	push ebp            ; *value = &mmt
	push 1016h          ; pname  = AL_BUFFERS_PROCESSED
	push DWORD [ebp+20] ; source = FileDSP
	and DWORD [ebp],0
	call alGetSourcei
	add esp,12
	mov esi,[ebp]
endif
if DRIVER_ALSA
	push DWORD [ebp+20] ; FileDSP
	call snd_pcm_avail_update
	mov esi,eax
	sar esi,FSOUND_Block
	pop edx
	jns pcm_status_ok
	push DWORD [ebp+20] ; FileDSP
	call snd_pcm_prepare
	pop edx
pcm_status_ok:
endif
thread_loop_2:
	; *** TAKE A LITTLE NAP :-)
	xor eax,eax
	push 400000h ; timespec.tv_nsec
	push eax     ; timespec.tv_sec
	mov ebx,esp
	mov ecx,esp
	mov al,162 ; nanosleep
	int 80h
	pop eax
	pop edx
	; *** CHECK FOR A REQUEST TO QUIT
	cmp DWORD [ebp+28],0    ; SW_Exit
	je thread_loop_2_continue
	pop esi
	pop ebp
	ret
thread_loop_2_continue:
if DRIVER_OPENAL
	; *** DO WE NEED TO FETCH ANY MORE DATA INTO SOUND BUFFERS?
	dec esi
	js thread_loop_1
	push ebp            ; *buffers = &mmt
	push 1              ; n        = 1
	push DWORD [ebp+20] ; source   = FileDSP
	and DWORD [ebp],0
	call alSourceUnqueueBuffers
	add esp,12
endif
if DRIVER_ALSA
	; *** DO WE NEED TO FETCH ANY MORE DATA INTO SOUND BUFFERS?
	dec esi
	js thread_loop_1
endif
if INFO_API_ON
	if PAUSE_RESUME_ON
		cmp BYTE [ufmod_pause_],0
		jne thread_realblock_ok
	endif
	mov eax,[RealBlock]
	inc eax
	if DRIVER_OSS
		cmp eax,[oss_numblocks]
	else
		cmp eax,totalblocks
	endif
	jl thread_realblock_ok
	xor eax,eax
thread_realblock_ok:
	mov [RealBlock],eax
endif
	push thread_loop_2 ; EIP

uFMOD_SW_Fill:
if BENCHMARK_ON
	dw 310Fh ; rdtsc
	mov [bench_t_lo],eax
endif
	mov ecx,FSOUND_BlockSize*2
	push ebx
	push esi
	push edi
	push ebp
	mov edi,MixBuf
	xor eax,eax
	push edi ; mixbuffer <= MixBuf
	push edi ; <- MixPtr
	; MIXBUFFER CLEAR
	mov esi,_mod+36
	rep stosd
if PAUSE_RESUME_ON
	cmp [ufmod_pause_],al
	xchg eax,ebp
	jne do_swfill
endif
	mov ebp,FSOUND_BlockSize
	; UPDATE MUSIC
	mov ebx,[esi+FMUSIC_MODULE.mixer_samplesleft-36]
fill_loop_1:
	test ebx,ebx
	jnz mixedleft_nz
	; UPDATE XM EFFECTS
	cmp [esi+FMUSIC_MODULE.tick-36],ebx ; new note
	mov ecx,[esi+FMUSIC_MODULE.pattern-36]
	jne update_effects
	dec ebx
	; process any rows commands to set the next order/row
	mov edx,[esi+FMUSIC_MODULE.nextorder-36]
	mov eax,[esi+FMUSIC_MODULE.nextrow-36]
	mov [esi+FMUSIC_MODULE.nextorder-36],ebx
	test edx,edx
	mov [esi+FMUSIC_MODULE.nextrow-36],ebx
	jl fill_nextrow
	mov [esi+FMUSIC_MODULE.order-36],edx
fill_nextrow:
	test eax,eax
	jl update_note
	mov [esi+FMUSIC_MODULE.row-36],eax
update_note:
	; mod+36 : ESI
	call DoNote
if ROWCOMMANDS_ON
	cmp DWORD [esi+FMUSIC_MODULE.nextrow-36],-1
	jne inc_tick
endif
	mov eax,[esi+FMUSIC_MODULE.row-36]
	inc eax
	; if end of pattern
	; "if(mod->nextrow >= mod->pattern[mod->orderlist[mod->order]].rows)"
	cmp ax,[ebx]
	jl set_nextrow
	mov edx,[esi+FMUSIC_MODULE.order-36]
	movzx ecx,WORD [esi+FMUSIC_MODULE.numorders-36]
	inc edx
	xor eax,eax
	cmp edx,ecx
	jl set_nextorder
	; We've reached the end of the order list. So, stop playing, unless looping is enabled.
if NOLOOP_ON
	cmp [ufmod_noloop],al
	je set_restart
	mov ebp,TID
	pop DWORD [ebp+12] ; SW_Exit : remove mixbuffer - signal thread to stop
	jmp thread_finished
set_restart:
endif
	movzx edx,WORD [esi+FMUSIC_MODULE.restart-36]
	cmp edx,ecx
	sbb ecx,ecx
	and edx,ecx
set_nextorder:
	mov [esi+FMUSIC_MODULE.nextorder-36],edx
set_nextrow:
	mov [esi+FMUSIC_MODULE.nextrow-36],eax
	jmp inc_tick
update_effects:
	; mod+36 : ESI
	call DoEffs
inc_tick:
	mov eax,[esi+FMUSIC_MODULE.speed-36]
	mov ebx,[esi+FMUSIC_MODULE.mixer_samplespertick-36]
	inc DWORD [esi+FMUSIC_MODULE.tick-36]
if PATTERNDELAY_ON
	add eax,[esi+FMUSIC_MODULE.patterndelay-36]
endif
	cmp [esi+FMUSIC_MODULE.tick-36],eax
	jl mixedleft_nz
if PATTERNDELAY_ON
	and DWORD [esi+FMUSIC_MODULE.patterndelay-36],0
endif
	and DWORD [esi+FMUSIC_MODULE.tick-36],0
mixedleft_nz:
	mov edi,ebp
	cmp ebx,edi
	jae fill_ramp
	mov edi,ebx
fill_ramp:
	pop edx  ; <- MixPtr
	sub ebp,edi
	lea eax,[edx+edi*8]
	push eax ; MixPtr += (SamplesToMix<<3)
	; tail    : [arg0]
	; len     : EDI
	; mixptr  : EDX
	; _mod+36 : ESI
	call Ramp
if INFO_API_ON
	lea eax,[edi+edi*4]
	cdq
	shl eax,2
	mov ecx,FSOUND_MixRate/50
	div ecx
	; time_ms += SamplesToMix*FSOUND_OOMixRate*1000
	add [time_ms],eax
endif
	sub ebx,edi ; MixedLeft -= SamplesToMix
	test ebp,ebp
	jnz fill_loop_1
	mov [esi+FMUSIC_MODULE.mixer_samplesleft-36],ebx ; <= MixedLeft
if INFO_API_ON
	mov edx,[uFMOD_FillBlk]
	if DRIVER_OSS
		shl edx,fragments+3 ; x totalblocks x FMUSIC_STATS_size
		xor eax,eax
		xchg eax,edx
		div DWORD [oss_numblocks]
		add eax,tInfo+4
		xchg eax,edx
	else
		lea edx,[edx*FMUSIC_STATS_size+tInfo+4]
	endif
	mov ecx,[esi + FMUSIC_MODULE.row-36]
	or ecx,[esi + FMUSIC_MODULE.order-2-36]
	mov [edx],ecx
endif
do_swfill:
	; *** CLIP AND COPY BLOCK TO OUTPUT BUFFER
	pop eax ; skip MixPtr
	pop esi ; <- mixbuffer
if INFO_API_ON
	; ebx : L channel RMS volume
	; ebp : R channel RMS volume
	xor ebx,ebx
endif
	mov edi,esi
if DRIVER_OPENAL
	push FSOUND_MixRate     ; freq
	push FSOUND_BlockSize*4 ; size
	push esi                ; *data
endif
	mov ecx,FSOUND_BlockSize*2
	align 4
fill_loop_2:
	lodsd
if INFO_API_ON
	push edi
	cdq
	mov edi,eax
	push esi
	xor eax,edx
	mov esi,255*volumerampsteps/2
	sub eax,edx
	xor edx,edx
	div esi
	cmp edx,255*volumerampsteps/4
	pop esi
	sbb eax,-1
	cmp eax,8000h
	sbb edx,edx
	not edx
	or eax,edx
	sar edi,31
	and eax,7FFFh
if VOL_CONTROL_ON
	mul DWORD [ufmod_vol]
	shr eax,15
endif
	; sum. the L and R channel RMS volume
	ror ecx,1
	sbb edx,edx
	and edx,eax
	add ebp,edx ; += |vol|
	rol ecx,1
	sbb edx,edx
	not edx
	and edx,eax
	add ebx,edx ; += |vol|
	xor eax,edi
	sub eax,edi
	pop edi
else
	mov ebx,eax
	cdq
	xor eax,edx
	sub eax,edx
	mov ebp,255*volumerampsteps/2
	xor edx,edx
	div ebp
	cmp edx,255*volumerampsteps/4
	sbb eax,-1
	cmp eax,8000h
	sbb edx,edx
	not edx
	or eax,edx
	sar ebx,31
	and eax,7FFFh
if VOL_CONTROL_ON
	mul DWORD [ufmod_vol]
	shr eax,15
endif
	xor eax,ebx
	sub eax,ebx
endif
	dec ecx
	stosw
	jnz fill_loop_2
SW_Fill_Ret:
	mov eax,[uFMOD_FillBlk]
	inc eax
if DRIVER_OSS
	cmp eax,[oss_numblocks]
else
	cmp eax,totalblocks
endif
	jl SW_Fill_R
	xor eax,eax
SW_Fill_R:
	mov [uFMOD_FillBlk],eax
if INFO_API_ON
	shr ebp,FSOUND_Block      ; R_vol / FSOUND_BlockSize
	shl ebx,16-FSOUND_Block   ; (L_vol / FSOUND_BlockSize) << 16
	mov bx,bp
	if DRIVER_OSS
		shl eax,fragments+3 ; x totalblocks x FMUSIC_STATS_size
		xor edx,edx
		div DWORD [oss_numblocks]
		mov DWORD [tInfo+eax],ebx
	else
		mov DWORD [tInfo+eax*FMUSIC_STATS_size],ebx
	endif
endif
if DRIVER_OSS
	; *** DISPATCH DATA TO THE OSS DRIVER
	mov edx,FSOUND_BlockSize*4 ; cound
	push 4
	mov ecx,MixBuf             ; buf
	mov ebx,[FileDSP]          ; fd
	pop eax                    ; write
	int 80h
endif
if DRIVER_OPENAL
	; *** DISPATCH DATA TO THE OPENAL DRIVER
	push 1103h                 ; format = AL_FORMAT_STEREO16
	push DWORD [mmt]           ; buffer
	call alBufferData
	push mmt                   ; *buffers
	push 1                     ; n
	push DWORD [FileDSP]       ; source
	call alSourceQueueBuffers
	add esp,32
endif
if DRIVER_ALSA
	; *** DISPATCH DATA TO THE ALSA DRIVER
	push FSOUND_BlockSize
	push MixBuf
	push DWORD [FileDSP]
	call snd_pcm_writei
	add esp,12
endif
	pop ebp
	pop edi
	pop esi
	pop ebx
if BENCHMARK_ON
	dw 310Fh ; rdtsc
	sub eax,[bench_t_lo]
	mov [uFMOD_tsc],eax
endif
	ret
