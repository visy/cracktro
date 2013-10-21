import brl.blitz
XM_NOLOOP%=8
XM_SUSPENDED%=16
uFMOD_MIN_VOL%=0
uFMOD_MAX_VOL%=25
uFMOD_DEFAULT_VOL%=25
uFMOD_ALSAPlayFile%(filename$z,dwReserved%,dwFlags%,szPCM_dev$z)="BB_ALSAPlayFile"
uFMOD_ALSAPlayMem%(pXM@*,length%,dwFlags%,szPCM_dev$z)="BB_ALSAPlayMem"
uFMOD_ALSAStop%()="BB_ALSAStop"
uFMOD_ALSAGetStats%()="uFMOD_ALSAGetStats"
uFMOD_ALSAGetRowOrder%()="uFMOD_ALSAGetRowOrder"
uFMOD_ALSAGetTime%()="uFMOD_ALSAGetTime"
uFMOD_ALSAGetTitle$z()="uFMOD_ALSAGetTitle"
uFMOD_ALSAPause%()="uFMOD_ALSAPause"
uFMOD_ALSAResume%()="uFMOD_ALSAResume"
uFMOD_ALSASetVolume%(vol%)="uFMOD_ALSASetVolume"
uFMOD_ALSAJump2Pattern%(pat%)="uFMOD_ALSAJump2Pattern"
