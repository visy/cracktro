import brl.blitz
XM_NOLOOP%=8
XM_SUSPENDED%=16
uFMOD_MIN_VOL%=0
uFMOD_MAX_VOL%=25
uFMOD_DEFAULT_VOL%=25
uFMOD_OALPlayFile%(filename$z,dwReserved%,dwFlags%,source%)="BB_OALPlayFile"
uFMOD_OALPlayMem%(lpXM@*,length%,dwFlags%,source%)="BB_OALPlayMem"
uFMOD_OALStop%()="BB_OALStop"
uFMOD_OALGetStats%()="uFMOD_OALGetStats"
uFMOD_OALGetRowOrder%()="uFMOD_OALGetRowOrder"
uFMOD_OALGetTime%()="uFMOD_OALGetTime"
uFMOD_OALGetTitle$z()="uFMOD_OALGetTitle"
uFMOD_OALPause%()="uFMOD_OALPause"
uFMOD_OALResume%()="uFMOD_OALResume"
uFMOD_OALSetVolume%(vol%)="uFMOD_OALSetVolume"
uFMOD_OALJump2Pattern%(pat%)="uFMOD_OALJump2Pattern"
