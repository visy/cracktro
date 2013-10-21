; Copy the alsaufmod library to [PureBasic]/purelibraries/userlibraries
; before compiling the following example.

; Supported OS: Linux

; uFMOD constants
#XM_MEMORY         = 1
#XM_FILE           = 2
#XM_NOLOOP         = 8
#XM_SUSPENDED      = 16
#uFMOD_MIN_VOL     = 0
#uFMOD_MAX_VOL     = 25
#uFMOD_DEFAULT_VOL = 25

; A note on selecting the PCM device name
; ---------------------------------------
; For compatibility with PureBasic's sound system you should select
; a different PCM device name. "default", "hw:0,0" and "plughw:0,0"
; generally map to the same device actually used with OSS.
; PureBasic sound system uses SDL, which normally uses OSS too.
; So, if you want to use PureBasic's sound system and uFMOD in
; the same application, let the user select an appropriate PCM device
; for uFMOD.

If uFMOD_ALSAPlaySong(?xm, ?endxm-?xm, #XM_MEMORY, "plughw:0,0") <> 0
	; Pop-up a message box to let uFMOD play the XM till user input.
	; You can stop it anytime calling uFMOD_ALSAPlaySong(0,0,0,"").
	; PureBasic will stop it automatically before exiting.
	MessageRequester("PureBasic", "uFMOD ruleZ!")
EndIf

End

DataSection

xm:
	IncludeBinary "mini.xm"
endxm:

EndDataSection
; IDE Options = PureBasic 3.94 (Linux - x86) - (c) 2005 Fantaisie Software
; Folding = -
; Executable = test_alsa
; DisableDebugger