' A simple console file playback example.
' Using uFMOD with OpenAL.

#include "al/al.bi"
#include "al/alc.bi"
#include "OALuFMOD.bi" ' uFMOD (OpenAL)

Dim Shared as Integer OAL_Source
Dim Shared as Integer Ptr OAL_Device, OAL_Context

Private Function PlayOAL() As Integer
	OAL_Context = 0
	OAL_Device = alcOpenDevice(0)
	If OAL_Device = 0 Then Return 0
	OAL_Context = alcCreateContext(OAL_Device, 0)
	alcMakeContextCurrent(OAL_Context)
	alGenSources(1, @OAL_Source)
	If alGetError <> 0 Then Return 0
	If uFMOD_OALPlaySong(@"mini.xm", 0, XM_FILE, OAL_Source) = 0 Then Return 0
	Return 1
End Function

Sub DrawVolume(vol As Integer)
Dim As Integer i, newcolor
Dim s As String * 1
	For i=0 To 60
		newcolor = 12
		If i<8 Then
			newcolor = 10
		ElseIf i<16 Then
			newcolor = 14
		EndIf
		s = "*"
		If i>=vol Then
			s = " "
		EndIf
		color newcolor : print s;

	Next i
	print " "
End Sub

cls
If PlayOAL() = 1 Then

	' Update VUmeters and playback time until user input.
	While len(inkey)=0
		volume=uFMOD_GetStats
		locate 1,1,0
		color 15 : print "L: "; : DrawVolume(loword(volume) shr 9)
		color 15 : print "R: "; : DrawVolume(hiword(volume) shr 9)
		color 15 : print "Time (s): " & uFMOD_GetTime\1000;
		sleep 5
	Wend

	' Stop playback.
	uFMOD_StopSong()
Else
	print "-ERR: Make sure mini.xm is still there ;)";
EndIf

alcMakeContextCurrent(0)
alcDestroyContext(OAL_Context)
alcCloseDevice(OAL_Device)

End
