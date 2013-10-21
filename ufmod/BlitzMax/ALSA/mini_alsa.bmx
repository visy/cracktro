' Copy mod\ufmod.mod\alsaufmod.mod to [BlitzMax]\mod\ufmod.mod
' before compiling this example for the first time.

Framework ufmod.alsaufmod

Import brl.system
Import "-lasound"

Incbin "mini.xm"

If uFMOD_ALSAPlayMem(IncbinPtr "mini.xm",IncbinLen "mini.xm",0,"plughw:0,0")
	WriteStdout "Playing song... [Press Enter to quit]"
	ReadStdin()
	uFMOD_ALSAStop
Else
	WriteStdout "Error!"
EndIf

End
