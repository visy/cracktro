' Copy mod\ufmod.mod\oalufmod.mod and mod\ufmod.mod\openal.mod modules to [BlitzMax]\mod\ufmod.mod
' before compiling this example for the first time.

' Make sure you have OpenAL properly installed before running this
' program. You can download the redistributable OpenAL installer from
' the official OpenAL website: http://www.openal.org/downloads.html

Framework ufmod.oalufmod
Import brl.system

' Latest BlitzMax releases have a built-in OpenAL module: pub.openal.
' When in Framework mode, you can choose between uFMOD's ufmod.openal module
' and the standart BlitzMax module according to your BlitzMax version. If not
' in Framework mode, pub.openal will be used by default in BlitzMax v1.24
' and later.

Import ufmod.openal ' uFMOD's OpenAL module
' Import pub.openal ' Default OpenAL module for BlitzMax v1.24 and later

' Embed an XM file for playback from memory. (It is also possible to play from file and resource.)
Incbin "mini.xm"

' Open the Default OpenAL device.
OAL_device = alcOpenDevice(Null)
If OAL_device = 0
	WriteStdout "Could not open the default OpenAL device"
Else

	' Create a context and make it the current context.
	OAL_context = alcCreateContext(OAL_device, Null)
	If OAL_context = 0
		WriteStdout "Could not create a context"
	Else
		alcMakeContextCurrent(OAL_context)

		' Generate a single source for playback.
		alGenSources(1, Varptr OAL_source)
		If alGetError() <> 0
			WriteStdout "Could not generate the source"
		Else

			' Start playback.
			If uFMOD_OALPlayMem(IncbinPtr "mini.xm", IncbinLen "mini.xm", 0, OAL_source) <> 0

				' Wait for user input.
				WriteStdout "Playing song... [Press Enter to quit]"
				ReadStdin()
				' Stop playback.
				uFMOD_OALStop

			EndIf

		EndIf

		' Release the current context and destroy it (the source gets destroyed as well).
		alcMakeContextCurrent(0)
		alcDestroyContext(OAL_context)

	EndIf

	' Close the actual device.
	alcCloseDevice(OAL_device)

EndIf

End
