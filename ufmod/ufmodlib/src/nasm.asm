; NASM.ASM
; --------
; uFMOD public source code release. Provided as-is.

; *** This stub allows compiling uFMOD sources to ELF object format
; *** using NASM
; Everything documented in fasm stub!

; %error directive in NASM causes multiple prompts to appear due to
; multiple passes :( So, we'd better avoid using %error.

ifdef OSS
	DRIVER_OSS    equ 1
	DRIVER_OPENAL equ 0
	DRIVER_ALSA   equ 0
else
	ifdef OPENAL
		DRIVER_OSS    equ 0
		DRIVER_OPENAL equ 1
		DRIVER_ALSA   equ 0
	else
		DRIVER_OSS    equ 0
		DRIVER_OPENAL equ 0
		DRIVER_ALSA   equ 1
	endif
endif

ifdef f44100
	FSOUND_MixRate equ 44100
	FREQ_40HZ_p    equ 1DB8Bh
	FREQ_40HZ_f    equ 3B7160h
else
	ifdef f22050
		FSOUND_MixRate equ 22050
		FREQ_40HZ_p    equ 3B716h
		FREQ_40HZ_f    equ 76E2C0h
	else
		FSOUND_MixRate equ 48000
		FREQ_40HZ_p    equ 1B4E8h
		FREQ_40HZ_f    equ 369D00h
	endif
endif

ifdef NONE
	RAMP_NONE   equ 1
	RAMP_WEAK   equ 0
	RAMP_STRONG equ 0
else
	ifdef WEAK
		RAMP_NONE   equ 0
		RAMP_WEAK   equ 1
		RAMP_STRONG equ 0
	else
		RAMP_NONE   equ 0
		RAMP_WEAK   equ 0
		RAMP_STRONG equ 1
	endif
endif

UCODE equ 0

ifdef BENCHMARK
	BENCHMARK_ON equ 1
	CHK4VALIDITY equ 1
else
	ifdef UNSAFE
		BENCHMARK_ON equ 0
		CHK4VALIDITY equ 0
	else
		BENCHMARK_ON equ 0
		CHK4VALIDITY equ 1
	endif
endif

ifdef PUREBASIC
	PBASIC equ 1
	EXTERN PB_StringBase
else
	PBASIC equ 0
endif

BLITZMAX equ 0

%include "eff.inc"

ifdef NOLINKER
	align 4
else
	[segment .text align=4]
endif

STRUC FSOUND_SAMPLE
	FSOUND_SAMPLE._length   resd 1
	FSOUND_SAMPLE.loopstart resd 1
	FSOUND_SAMPLE.looplen   resd 1
	FSOUND_SAMPLE.defvol    resb 1
	FSOUND_SAMPLE.finetune  resb 1
	FSOUND_SAMPLE.bytes     resb 1
	FSOUND_SAMPLE.defpan    resb 1
	FSOUND_SAMPLE.relative  resb 1
	FSOUND_SAMPLE.Resved    resb 1
	FSOUND_SAMPLE.loopmode  resb 1
	FSOUND_SAMPLE._align    resb 1
	FSOUND_SAMPLE.buff      resb 2
ENDSTRUC

STRUC FSOUND_CHANNEL
	FSOUND_CHANNEL.actualvolume     resd 1
	FSOUND_CHANNEL.actualpan        resd 1
	FSOUND_CHANNEL.fsampleoffset    resd 1
	FSOUND_CHANNEL.leftvolume       resd 1
	FSOUND_CHANNEL.rightvolume      resd 1
	FSOUND_CHANNEL.mixpos           resd 1
	FSOUND_CHANNEL.speedlo          resd 1
	FSOUND_CHANNEL.speedhi          resd 1
	FSOUND_CHANNEL.ramp_lefttarget  resw 1
	FSOUND_CHANNEL.ramp_righttarget resw 1
	FSOUND_CHANNEL.ramp_leftspeed   resd 1
	FSOUND_CHANNEL.ramp_rightspeed  resd 1
	FSOUND_CHANNEL.fsptr            resd 1
	FSOUND_CHANNEL.mixposlo         resd 1
	FSOUND_CHANNEL.ramp_leftvolume  resd 1
	FSOUND_CHANNEL.ramp_rightvolume resd 1
	FSOUND_CHANNEL.ramp_count       resw 1
	FSOUND_CHANNEL.speeddir         resb 2
ENDSTRUC

STRUC FMUSIC_NOTE
	FMUSIC_NOTE.unote   resb 1
	FMUSIC_NOTE.number  resb 1
	FMUSIC_NOTE.uvolume resb 1
	FMUSIC_NOTE.effect  resb 1
	FMUSIC_NOTE.eparam  resb 1
ENDSTRUC

STRUC FMUSIC_STATS
	FMUSIC_STATS.L_vol   resw 1
	FMUSIC_STATS.R_vol   resw 1
	FMUSIC_STATS.s_row   resw 1
	FMUSIC_STATS.s_order resw 1
ENDSTRUC

STRUC FMUSIC_PATTERN
	FMUSIC_PATTERN.rows        resw 1
	FMUSIC_PATTERN.patternsize resw 1
	FMUSIC_PATTERN.data        resd 1
ENDSTRUC

STRUC FMUSIC_INSTRUMENT
	FMUSIC_INSTRUMENT.sample       resd 16
	FMUSIC_INSTRUMENT.keymap       resb 96
	FMUSIC_INSTRUMENT.VOLPoints    resw 24
	FMUSIC_INSTRUMENT.PANPoints    resw 24
	FMUSIC_INSTRUMENT.VOLnumpoints resb 1
	FMUSIC_INSTRUMENT.PANnumpoints resb 1
	FMUSIC_INSTRUMENT.VOLsustain   resb 1
	FMUSIC_INSTRUMENT.VOLLoopStart resb 1
	FMUSIC_INSTRUMENT.VOLLoopEnd   resb 1
	FMUSIC_INSTRUMENT.PANsustain   resb 1
	FMUSIC_INSTRUMENT.PANLoopStart resb 1
	FMUSIC_INSTRUMENT.PANLoopEnd   resb 1
	FMUSIC_INSTRUMENT.VOLtype      resb 1
	FMUSIC_INSTRUMENT.PANtype      resb 1
	FMUSIC_INSTRUMENT.VIBtype      resb 1
	FMUSIC_INSTRUMENT.VIBsweep     resb 1
	FMUSIC_INSTRUMENT.iVIBdepth    resb 1
	FMUSIC_INSTRUMENT.VIBrate      resb 1
	FMUSIC_INSTRUMENT.VOLfade      resw 1
ENDSTRUC

STRUC FMUSIC_CHANNEL
	FMUSIC_CHANNEL.note          resb 1
	FMUSIC_CHANNEL.samp          resb 1
	FMUSIC_CHANNEL.notectrl      resb 1
	FMUSIC_CHANNEL.inst          resb 1
	FMUSIC_CHANNEL.cptr          resd 1
	FMUSIC_CHANNEL.freq          resd 1
	FMUSIC_CHANNEL.volume        resd 1
	FMUSIC_CHANNEL.voldelta      resd 1
	FMUSIC_CHANNEL.freqdelta     resd 1
	FMUSIC_CHANNEL.pan           resd 1
	FMUSIC_CHANNEL.envvoltick    resd 1
	FMUSIC_CHANNEL.envvolpos     resd 1
	FMUSIC_CHANNEL.envvoldelta   resd 1
	FMUSIC_CHANNEL.envpantick    resd 1
	FMUSIC_CHANNEL.envpanpos     resd 1
	FMUSIC_CHANNEL.envpandelta   resd 1
	FMUSIC_CHANNEL.ivibsweeppos  resd 1
	FMUSIC_CHANNEL.ivibpos       resd 1
	FMUSIC_CHANNEL.keyoff        resb 2
	FMUSIC_CHANNEL.envvolstopped resb 1
	FMUSIC_CHANNEL.envpanstopped resb 1
	FMUSIC_CHANNEL.envvolfrac    resd 1
	FMUSIC_CHANNEL.envvol        resd 1
	FMUSIC_CHANNEL.fadeoutvol    resd 1
	FMUSIC_CHANNEL.envpanfrac    resd 1
	FMUSIC_CHANNEL.envpan        resd 1
	FMUSIC_CHANNEL.period        resd 1
	FMUSIC_CHANNEL.sampleoffset  resd 1
	FMUSIC_CHANNEL.portatarget   resd 1
	FMUSIC_CHANNEL.patloopno     resb 4
	FMUSIC_CHANNEL.patlooprow    resd 1
	FMUSIC_CHANNEL.realnote      resb 1
	FMUSIC_CHANNEL.recenteffect  resb 1
	FMUSIC_CHANNEL.portaupdown   resb 2
	FMUSIC_CHANNEL.xtraportadown resb 1
	FMUSIC_CHANNEL.xtraportaup   resb 1
	FMUSIC_CHANNEL.volslide      resb 1
	FMUSIC_CHANNEL.panslide      resb 1
	FMUSIC_CHANNEL.retrigx       resb 1
	FMUSIC_CHANNEL.retrigy       resb 1
	FMUSIC_CHANNEL.portaspeed    resb 1
	FMUSIC_CHANNEL.vibpos        resb 1
	FMUSIC_CHANNEL.vibspeed      resb 1
	FMUSIC_CHANNEL.vibdepth      resb 1
	FMUSIC_CHANNEL.tremolopos    resb 1
	FMUSIC_CHANNEL.tremolospeed  resb 1
	FMUSIC_CHANNEL.tremolodepth  resb 1
	FMUSIC_CHANNEL.tremorpos     resb 1
	FMUSIC_CHANNEL.tremoron      resb 1
	FMUSIC_CHANNEL.tremoroff     resb 1
	FMUSIC_CHANNEL.wavecontrol   resb 1
	FMUSIC_CHANNEL.finevslup     resb 1
	FMUSIC_CHANNEL.fineportaup   resb 1
	FMUSIC_CHANNEL.fineportadown resb 1
ENDSTRUC

STRUC FMUSIC_MODULE
	FMUSIC_MODULE.pattern              resd 1
	FMUSIC_MODULE.instrument           resd 1
	FMUSIC_MODULE.mixer_samplesleft    resd 1
	FMUSIC_MODULE.globalvolume         resd 1
	FMUSIC_MODULE.tick                 resd 1
	FMUSIC_MODULE.speed                resd 1
	FMUSIC_MODULE.order                resd 1
	FMUSIC_MODULE.row                  resd 1
	FMUSIC_MODULE.patterndelay         resd 1
	FMUSIC_MODULE.nextorder            resd 1
	FMUSIC_MODULE.nextrow              resd 1
	FMUSIC_MODULE.unused1              resd 1
	FMUSIC_MODULE.numchannels          resd 1
	FMUSIC_MODULE.Channels             resd 1
	FMUSIC_MODULE.uFMOD_Ch             resd 1
	FMUSIC_MODULE.mixer_samplespertick resd 1
	FMUSIC_MODULE.numorders            resw 1
	FMUSIC_MODULE.restart              resw 1
	FMUSIC_MODULE.numchannels_xm       resb 1
	FMUSIC_MODULE.globalvsl            resb 1
	FMUSIC_MODULE.numpatternsmem       resw 1
	FMUSIC_MODULE.numinsts             resw 1
	FMUSIC_MODULE.flags                resw 1
	FMUSIC_MODULE.defaultspeed         resw 1
	FMUSIC_MODULE.defaultbpm           resw 1
	FMUSIC_MODULE.orderlist            resb 256
ENDSTRUC

%macro PUBLIC 1
	ifdef NOLINKER
	else
		GLOBAL %1:function
	endif
%endmacro
%define OFFSET
%define PTR

%macro EXPORTcc 2
	if %1
		ifdef NOLINKER
		else
			GLOBAL %2:function
		endif
		%2:
	endif
%endmacro

if DRIVER_OPENAL
	extern alBufferData,alDeleteBuffers,alGenBuffers,alGetError,alGetSourcei
	extern alSourcei,alSourcePlay,alSourceQueueBuffers,alSourceStop,alSourceUnqueueBuffers
endif

if DRIVER_ALSA
	extern snd_pcm_open,snd_pcm_drop,snd_pcm_close,snd_pcm_writei
	extern snd_pcm_prepare,snd_pcm_hw_params,snd_pcm_avail_update,snd_pcm_hw_params_any
	extern snd_pcm_hw_params_set_access,snd_pcm_hw_params_set_format
	extern snd_pcm_hw_params_set_channels,snd_pcm_hw_params_set_rate_min
	extern snd_pcm_hw_params_set_periods_min,snd_pcm_hw_params_set_buffer_size_min
endif

include "ufmod.asm"
include "core.asm"

ifdef NOLINKER
	textsize equ $ - textstart
	align 16
	bssoffset equ $ - $$
	[segment .bss]
	bssstart:
else
	[segment .bss align=16]
endif

if DRIVER_OSS
	thread_stack resb 1020
else
	thread_stack resb 2556
endif
thread_top    resd 1
_mod          resb FMUSIC_MODULE_size
mmt           resd 3
ufmod_heap    resd 1
TID           resd 1
FileDSP       resd 1
uFMOD_FillBlk resd 1
SW_Exit       resd 1
MixBuf        resb FSOUND_BlockSize*8
ufmod_noloop  resb 1
ufmod_pause_  resb 1
mix_endflag   resb 2
mmf           resd 4
ufmod_vol     resd 1
uFMOD_fopen   resd 1
uFMOD_fread   resd 1
uFMOD_fclose  resd 1
if DRIVER_OSS
	oss_numblocks resd 1
endif
if DRIVER_OPENAL
	databuf resd totalblocks
endif
if INFO_API_ON
	RealBlock resd 1
	time_ms   resd 1
	tInfo     resb FMUSIC_STATS_size*totalblocks
	szTtl     resb 24
endif
DummySamp resb FSOUND_SAMPLE_size
if BENCHMARK_ON
	bench_t_lo resd 1
	GLOBAL uFMOD_tsc
	uFMOD_tsc resd 1
endif
ifdef NOLINKER
	bsssize equ $ - bssstart
endif
