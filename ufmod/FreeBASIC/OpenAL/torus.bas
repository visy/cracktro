' Original Torus example by rel (see examples/gfx/rel-torus.bas)
' Modified version to work as an audio visualization.
' Using uFMOD with OpenAL.

#include "al/al.bi"
#include "al/alc.bi"
#include "tinyptc.bi"
#include "OALuFMOD.bi" ' uFMOD (OpenAL)

Const SCR_TITLE = "FreeBASIC"
Const SCR_WIDTH = 320
Const SCR_HEIGHT = 240
Const PI_180 = 0.017453292519943295769236907684886 ' Pi/180
Const XMID = SCR_WIDTH\2
Const YMID = SCR_HEIGHT\2
Const LENS = 128

Dim Shared As Integer volume, ang, angx, angy, angz, dist, ttx, tty
Dim Shared SCR_BUFFER(0 To SCR_WIDTH * SCR_HEIGHT - 1) As Integer
Dim Shared As Single ssx, ssy, ssz, ccx, ccy, ccz, xxx, xxy, xxz, yyx, yyy, yyz, zzx, zzy, zzz
Dim Shared As Single rrx, rry, aax, aay, aaz, a, p, q, r, rad, x, y, z
Dim Shared As Ushort lvol, rvol, ll, lr

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

' Make sure to perform a cleanup before exiting!
Declare Sub atexit alias "atexit" (byval as any ptr)
Dim Shared As Integer flag_clean = 0
Sub onexit()
	If flag_clean = 0 Then
		uFMOD_StopSong()
		alcMakeContextCurrent(0)
		alcDestroyContext(OAL_Context)
		alcCloseDevice(OAL_Device)
	EndIf
End Sub

Private Sub smooth(buffer() As Integer)
Dim As Integer maxpixel, offset, pixel, r, g, b
	maxpixel = Ubound(buffer)
	For offset = SCR_WIDTH To maxpixel-SCR_WIDTH
		pixel = buffer(offset-1) Shr 2
		r = pixel Shr 16
		g = pixel Shr 8 And 63
		b = pixel And 63
		pixel = buffer(offset+1) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		pixel = buffer(offset+SCR_WIDTH) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		pixel = buffer(offset-SCR_WIDTH) Shr 2
		r = r + (pixel Shr 16)
		g = g + (pixel Shr 8 And 63)
		b = b + (pixel And 63)
		buffer(offset) = r Shl 16 Or g Shl 8 Or b
	Next offset
End Sub

If ptc_open(SCR_TITLE, SCR_WIDTH, SCR_HEIGHT) <> 0 Then

	angx = 90
	angy = 180
	angz = 270
	p = -11
	q = 12
	rad = 60
	ptc_update @SCR_BUFFER(0) ' Update the screen buffer to prevent lags.

	If PlayOAL() = 1 Then
		Dim d As integer = -1

		' ONEXIT() will be called when user presses ESC.
		atexit(@ONEXIT)

		While MultiKey(&h1C) = 0
			volume = uFMOD_GetStats()
			lvol = Loword(volume) Shr 9
			rvol = Hiword(volume) Shr 9
			p = p + ((lvol)*.001)
			q = q - ((rvol)*.001)
			If rad <= 30 or (lvol > ll and rvol > lr) Then
				d = 5
			ElseIf rad >= 60 or (lvol < ll and rvol < lr) Then
				d = -3
			EndIf
			rad += ((lvol+rvol)/ 20 ) * 0.15 * d
			angx = (angx + 1 + (1/lvol)) Mod 360
			angy = (angx + 1 + (1/(lvol+rvol))) Mod 360
			angz = (angx + 1 + (1/rvol)) Mod 360
			aax = angx * PI_180
			aay = angy * PI_180
			aaz = angz * PI_180
			ccx = Cos(aax)
			ssx = Sin(aax)
			ccy = Cos(aay)
			ssy = Sin(aay)
			ccz = Cos(aaz)
			ssz = Sin(aaz)
			xxx = ccy * ccz
			xxy = ssx * ssy * ccz - ccx * ssz
			xxz = ccx * ssy * ccz + ssx * ssz
			yyx = ccy * ssz
			yyy = ccx * ccz + ssx * ssy * ssz
			yyz = -ssx * ccz + ccx * ssy * ssz
			zzx = -ssy
			zzy = ssx * ccy
			zzz = ccx * ccy
			For ang = 0 To 359
				a = ang * PI_180
				r = .5 * (2 + Sin(q * a)) * rad
				x = Cos(p * a) * r
				y = Cos(q * a) * r
				z = Sin(p * a) * r
				dist = LENS - (x * zzx + y * zzy + z * zzz)
				If dist > 0 Then
					rrx = (x * xxx + y * xxy + z * xxz)
					rry = (x * yyx + y * yyy + z * yyz)
					ttx = XMID + (rrx * LENS / dist)
					tty = YMID - (rry * LENS / dist)
					If(tty > 0 And tty < SCR_HEIGHT-1 And ttx > 0 And ttx < SCR_WIDTH-1) Then SCR_Buffer(tty * SCR_WIDTH + ttx) = &h80FFFF
				EndIf
			Next ang
			smooth SCR_BUFFER()
			ptc_update @SCR_BUFFER(0)
			ll = lvol
			lr = rvol
			Sleep(5)
		Wend

		uFMOD_StopSong()
	EndIf

	PTC_CLOSE()
	alcMakeContextCurrent(0)
	alcDestroyContext(OAL_Context)
	alcCloseDevice(OAL_Device)
EndIf

' Cleanup done.
flag_clean = 1
End
