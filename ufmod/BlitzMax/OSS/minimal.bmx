' Copy the mod\ufmod.mod\ufmod.mod module to [BlitzMax]\mod\ufmod.mod
' before compiling this example for the first time.

Framework ufmod.ufmod

Import brl.system

Incbin "mini.xm"

If uFMOD_PlayMem(IncbinPtr "mini.xm",IncbinLen "mini.xm",0)
	WriteStdout "Playing song... [Press Enter to quit]"
	ReadStdin()
	uFMOD_Stop
Else
	WriteStdout "Error!"
EndIf

End
