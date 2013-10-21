; Copy the ufmod library to [PureBasic]\purelibraries\userlibraries
; before compiling this example for the first time.

; Supported OS: Linux, Win32

; uFMOD constants
#XM_MEMORY         = 1
#XM_FILE           = 2
#XM_NOLOOP         = 8
#XM_SUSPENDED      = 16
#uFMOD_MIN_VOL     = 0
#uFMOD_MAX_VOL     = 25
#uFMOD_DEFAULT_VOL = 25

If uFMOD_PlaySong(?xm, ?endxm-?xm, #XM_MEMORY) <> 0
	; Pop-up a message box to let uFMOD play the XM till user input.
	; You can stop it anytime calling uFMOD_PlaySong(0,0,0).
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
; Executable = test
; DisableDebugger
