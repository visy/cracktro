; Copy the oalufmod and pbopenal libraries to
; [PureBasic]\purelibraries\userlibraries before compiling this
; example for the first time.

; Make sure you have OpenAL properly installed before running this
; program. You can download the redistributable OpenAL installer from
; the official OpenAL website: http://www.openal.org/downloads.html

; Supported OS: Linux, Win32

; uFMOD constants
#XM_MEMORY         = 1
#XM_FILE           = 2
#XM_NOLOOP         = 8
#XM_SUSPENDED      = 16
#uFMOD_MIN_VOL     = 0
#uFMOD_MAX_VOL     = 25
#uFMOD_DEFAULT_VOL = 25

; Open the default OpenAL device.
OAL_device = alcOpenDevice(0)
If OAL_device = 0
	MessageRequester("Error", "Could not open the default OpenAL device")
Else

	; Create a context and make it the current context.
	OAL_context = alcCreateContext(OAL_device, 0)
	If OAL_context = 0
		MessageRequester("Error", "Could not create a context")
	Else
		alcMakeContextCurrent(OAL_context)

		; Generate a single source for playback.
		alGenSources(1, @OAL_source)
		If alGetError() <> 0
			MessageRequester("Error", "Could not generate the source")
		Else

			; Start playback.
			If uFMOD_OALPlaySong(?xm, ?endxm-?xm, #XM_MEMORY, OAL_source) <> 0

				; Pop-up a message box to let uFMOD play the XM till user input.
				MessageRequester("PureBasic", "uFMOD ruleZ!")
				; Stop playback.
				uFMOD_OALPlaySong(0, 0, 0, 0)

			EndIf

		EndIf

		; Release the current context and destroy it (the source gets destroyed as well).
		alcMakeContextCurrent(0)
		alcDestroyContext(OAL_context)

	EndIf

	; Close the actual device.
	alcCloseDevice(OAL_device)

EndIf

End

DataSection

xm: 
	IncludeBinary "mini.xm"
endxm:
  
EndDataSection
; IDE Options = PureBasic 3.94 (Linux - x86) - (c) 2005 Fantaisie Software
; Folding = -
; Executable = TestAL
; DisableDebugger
