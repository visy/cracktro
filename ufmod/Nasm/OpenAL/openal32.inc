; OpenAL API versions 1.0 and 1.1
; Compiler: NASM

; *** OpenAL constants:
; (floating point values have been commented out)
AL_INVALID equ -1
AL_NONE equ 0
AL_FALSE equ 0
AL_TRUE equ 1
AL_SOURCE_RELATIVE equ 202h
AL_CONE_INNER_ANGLE equ 1001h
AL_CONE_OUTER_ANGLE equ 1002h
AL_PITCH equ 1003h
AL_POSITION equ 1004h
AL_DIRECTION equ 1005h
AL_VELOCITY equ 1006h
AL_LOOPING equ 1007h
AL_BUFFER equ 1009h
AL_GAIN equ 100Ah
AL_MIN_GAIN equ 100Dh
AL_MAX_GAIN equ 100Eh
AL_ORIENTATION equ 100Fh
AL_CHANNEL_MASK equ 3000h
AL_SOURCE_STATE equ 1010h
AL_INITIAL equ 1011h
AL_PLAYING equ 1012h
AL_PAUSED equ 1013h
AL_STOPPED equ 1014h
AL_BUFFERS_QUEUED equ 1015h
AL_BUFFERS_PROCESSED equ 1016h
AL_SEC_OFFSET equ 1024h
AL_SAMPLE_OFFSET equ 1025h
AL_BYTE_OFFSET equ 1026h
AL_SOURCE_TYPE equ 1027h
AL_STATIC equ 1028h
AL_STREAMING equ 1029h
AL_UNDETERMINED equ 1030h
AL_FORMAT_MONO8 equ 1100h
AL_FORMAT_MONO16 equ 1101h
AL_FORMAT_STEREO8 equ 1102h
AL_FORMAT_STEREO16 equ 1103h
AL_REFERENCE_DISTANCE equ 1020h
AL_ROLLOFF_FACTOR equ 1021h
AL_CONE_OUTER_GAIN equ 1022h
AL_MAX_DISTANCE equ 1023h
AL_FREQUENCY equ 2001h
AL_BITS equ 2002h
AL_CHANNELS equ 2003h
AL_SIZE equ 2004h
AL_UNUSED equ 2010h
AL_PENDING equ 2011h
AL_PROCESSED equ 2012h
AL_NO_ERROR equ 0
AL_INVALID_NAME equ 0A001h
AL_ILLEGAL_ENUM equ 0A002h
AL_INVALID_ENUM equ 0A002h
AL_INVALID_VALUE equ 0A003h
AL_ILLEGAL_COMMAND equ 0A004h
AL_INVALID_OPERATION equ 0A004h
AL_OUT_OF_MEMORY equ 0A005h
AL_VENDOR equ 0B001h
AL_VERSION equ 0B002h
AL_RENDERER equ 0B003h
AL_EXTENSIONS equ 0B004h
AL_DOPPLER_FACTOR equ 0C000h
AL_DOPPLER_VELOCITY equ 0C001h
AL_SPEED_OF_SOUND equ 0C003h
AL_DISTANCE_MODEL equ 0D000h
AL_INVERSE_DISTANCE equ 0D001h
AL_INVERSE_DISTANCE_CLAMPED equ 0D002h
AL_LINEAR_DISTANCE equ 0D003h
AL_LINEAR_DISTANCE_CLAMPED equ 0D004h
AL_EXPONENT_DISTANCE equ 0D005h
AL_EXPONENT_DISTANCE_CLAMPED equ 0D006h
ALC_INVALID equ 0
ALC_FALSE equ 0
ALC_TRUE equ 1
ALC_FREQUENCY equ 1007h
ALC_REFRESH equ 1008h
ALC_SYNC equ 1009h
ALC_MONO_SOURCES equ 1010h
ALC_STEREO_SOURCES equ 1011h
ALC_NO_ERROR equ 0
ALC_INVALID_DEVICE equ 0A001h
ALC_INVALID_CONTEXT equ 0A002h
ALC_INVALID_ENUM equ 0A003h
ALC_INVALID_VALUE equ 0A004h
ALC_OUT_OF_MEMORY equ 0A005h
ALC_DEFAULT_DEVICE_SPECIFIER equ 1004h
ALC_DEVICE_SPECIFIER equ 1005h
ALC_EXTENSIONS equ 1006h
ALC_MAJOR_VERSION equ 1000h
ALC_MINOR_VERSION equ 1001h
ALC_ATTRIBUTES_SIZE equ 1002h
ALC_ALL_ATTRIBUTES equ 1003h
ALC_CAPTURE_DEVICE_SPECIFIER equ 310h
ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER equ 311h
ALC_CAPTURE_SAMPLES equ 312h
ALC_EFX_MAJOR_VERSION equ 20001h
ALC_EFX_MINOR_VERSION equ 20002h
ALC_MAX_AUXILIARY_SENDS equ 20003h
AL_METERS_PER_UNIT equ 20004h
AL_DIRECT_FILTER equ 20005h
AL_AUXILIARY_SEND_FILTER equ 20006h
AL_AIR_ABSORPTION_FACTOR equ 20007h
AL_ROOM_ROLLOFF_FACTOR equ 20008h
AL_CONE_OUTER_GAINHF equ 20009h
AL_DIRECT_FILTER_GAINHF_AUTO equ 2000Ah
AL_AUXILIARY_SEND_FILTER_GAIN_AUTO equ 2000Bh
AL_AUXILIARY_SEND_FILTER_GAINHF_AUTO equ 2000Ch
AL_REVERB_DENSITY equ 1
AL_REVERB_DIFFUSION equ 2
AL_REVERB_GAIN equ 3
AL_REVERB_GAINHF equ 4
AL_REVERB_DECAY_TIME equ 5
AL_REVERB_DECAY_HFRATIO equ 6
AL_REVERB_REFLECTIONS_GAIN equ 7
AL_REVERB_REFLECTIONS_DELAY equ 8
AL_REVERB_LATE_REVERB_GAIN equ 9
AL_REVERB_LATE_REVERB_DELAY equ 10
AL_REVERB_AIR_ABSORPTION_GAINHF equ 11
AL_REVERB_ROOM_ROLLOFF_FACTOR equ 12
AL_REVERB_DECAY_HFLIMIT equ 13
AL_CHORUS_WAVEFORM equ 1
AL_CHORUS_PHASE equ 2
AL_CHORUS_RATE equ 3
AL_CHORUS_DEPTH equ 4
AL_CHORUS_FEEDBACK equ 5
AL_CHORUS_DELAY equ 6
AL_DISTORTION_EDGE equ 1
AL_DISTORTION_GAIN equ 2
AL_DISTORTION_LOWPASS_CUTOFF equ 3
AL_DISTORTION_EQCENTER equ 4
AL_DISTORTION_EQBANDWIDTH equ 5
AL_ECHO_DELAY equ 1
AL_ECHO_LRDELAY equ 2
AL_ECHO_DAMPING equ 3
AL_ECHO_FEEDBACK equ 4
AL_ECHO_SPREAD equ 5
AL_FLANGER_WAVEFORM equ 1
AL_FLANGER_PHASE equ 2
AL_FLANGER_RATE equ 3
AL_FLANGER_DEPTH equ 4
AL_FLANGER_FEEDBACK equ 5
AL_FLANGER_DELAY equ 6
AL_FREQUENCY_SHIFTER_FREQUENCY equ 1
AL_FREQUENCY_SHIFTER_LEFT_DIRECTION equ 2
AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION equ 3
AL_VOCAL_MORPHER_PHONEMEA equ 1
AL_VOCAL_MORPHER_PHONEMEA_COARSE_TUNING equ 2
AL_VOCAL_MORPHER_PHONEMEB equ 3
AL_VOCAL_MORPHER_PHONEMEB_COARSE_TUNING equ 4
AL_VOCAL_MORPHER_WAVEFORM equ 5
AL_VOCAL_MORPHER_RATE equ 6
AL_PITCH_SHIFTER_COARSE_TUNE equ 1
AL_PITCH_SHIFTER_FINE_TUNE equ 2
AL_RING_MODULATOR_FREQUENCY equ 1
AL_RING_MODULATOR_HIGHPASS_CUTOFF equ 2
AL_RING_MODULATOR_WAVEFORM equ 3
AL_AUTOWAH_ATTACK_TIME equ 1
AL_AUTOWAH_RELEASE_TIME equ 2
AL_AUTOWAH_RESONANCE equ 3
AL_AUTOWAH_PEAK_GAIN equ 4
AL_COMPRESSOR_ONOFF equ 1
AL_EQUALIZER_LOW_GAIN equ 1
AL_EQUALIZER_LOW_CUTOFF equ 2
AL_EQUALIZER_MID1_GAIN equ 3
AL_EQUALIZER_MID1_CENTER equ 4
AL_EQUALIZER_MID1_WIDTH equ 5
AL_EQUALIZER_MID2_GAIN equ 6
AL_EQUALIZER_MID2_CENTER equ 7
AL_EQUALIZER_MID2_WIDTH equ 8
AL_EQUALIZER_HIGH_GAIN equ 9
AL_EQUALIZER_HIGH_CUTOFF equ 10
AL_EFFECT_FIRST_PARAMETER equ 0
AL_EFFECT_LAST_PARAMETER equ 8000h
AL_EFFECT_TYPE equ 8001h
AL_EFFECT_NULL equ 0
AL_EFFECT_REVERB equ 1
AL_EFFECT_CHORUS equ 2
AL_EFFECT_DISTORTION equ 3
AL_EFFECT_ECHO equ 4
AL_EFFECT_FLANGER equ 5
AL_EFFECT_FREQUENCY_SHIFTER equ 6
AL_EFFECT_VOCAL_MORPHER equ 7
AL_EFFECT_PITCH_SHIFTER equ 8
AL_EFFECT_RING_MODULATOR equ 9
AL_EFFECT_AUTOWAH equ 10
AL_EFFECT_COMPRESSOR equ 11
AL_EFFECT_EQUALIZER equ 12
AL_EFFECTSLOT_EFFECT equ 1
AL_EFFECTSLOT_GAIN equ 2
AL_EFFECTSLOT_AUXILIARY_SEND_AUTO equ 3
AL_EFFECTSLOT_NULL equ 0
AL_LOWPASS_GAIN equ 1
AL_LOWPASS_GAINHF equ 2
AL_HIGHPASS_GAIN equ 1
AL_HIGHPASS_GAINLF equ 2
AL_BANDPASS_GAIN equ 1
AL_BANDPASS_GAINLF equ 2
AL_BANDPASS_GAINHF equ 3
AL_FILTER_FIRST_PARAMETER equ 0
AL_FILTER_LAST_PARAMETER equ 8000h
AL_FILTER_TYPE equ 8001h
AL_FILTER_NULL equ 0
AL_FILTER_LOWPASS equ 1
AL_FILTER_HIGHPASS equ 2
AL_FILTER_BANDPASS equ 3
; LOWPASS_MIN_GAIN equ 0
; LOWPASS_MAX_GAIN equ 1.0
; LOWPASS_DEFAULT_GAIN equ 1.0
; LOWPASS_MIN_GAINHF equ 0
; LOWPASS_MAX_GAINHF equ 1.0
; LOWPASS_DEFAULT_GAINHF equ 1.0
; HIGHPASS_MIN_GAIN equ 0
; HIGHPASS_MAX_GAIN equ 1.0
; HIGHPASS_DEFAULT_GAIN equ 1.0
; HIGHPASS_MIN_GAINLF equ 0
; HIGHPASS_MAX_GAINLF equ 1.0
; HIGHPASS_DEFAULT_GAINLF equ 1.0
; BANDPASS_MIN_GAIN equ 0
; BANDPASS_MAX_GAIN equ 1.0
; BANDPASS_DEFAULT_GAIN equ 1.0
; BANDPASS_MIN_GAINHF equ 0
; BANDPASS_MAX_GAINHF equ 1.0
; BANDPASS_DEFAULT_GAINHF equ 1.0
; BANDPASS_MIN_GAINLF equ 0
; BANDPASS_MAX_GAINLF equ 1.0
; BANDPASS_DEFAULT_GAINLF equ 1.0
; AL_REVERB_MIN_DENSITY equ 0
; AL_REVERB_MAX_DENSITY equ 1.0
; AL_REVERB_DEFAULT_DENSITY equ 1.0
; AL_REVERB_MIN_DIFFUSION equ 0
; AL_REVERB_MAX_DIFFUSION equ 1.0
; AL_REVERB_DEFAULT_DIFFUSION equ 1.0
; AL_REVERB_MIN_GAIN equ 0
; AL_REVERB_MAX_GAIN equ 1.0
; AL_REVERB_DEFAULT_GAIN equ 0.32
; AL_REVERB_MIN_GAINHF equ 0
; AL_REVERB_MAX_GAINHF equ 1.0
; AL_REVERB_DEFAULT_GAINHF equ 0.89
; AL_REVERB_MIN_DECAY_TIME equ 0.1
; AL_REVERB_MAX_DECAY_TIME equ 20.0
; AL_REVERB_DEFAULT_DECAY_TIME equ 1.49
; AL_REVERB_MIN_DECAY_HFRATIO equ 0.1
; AL_REVERB_MAX_DECAY_HFRATIO equ 2.0
; AL_REVERB_DEFAULT_DECAY_HFRATIO equ 0.83
; AL_REVERB_MIN_REFLECTIONS_GAIN equ 0
; AL_REVERB_MAX_REFLECTIONS_GAIN equ 3.16
; AL_REVERB_DEFAULT_REFLECTIONS_GAIN equ 0.05
; AL_REVERB_MIN_REFLECTIONS_DELAY equ 0
; AL_REVERB_MAX_REFLECTIONS_DELAY equ 0.3
; AL_REVERB_DEFAULT_REFLECTIONS_DELAY equ 0.007
; AL_REVERB_MIN_LATE_REVERB_GAIN equ 0
; AL_REVERB_MAX_LATE_REVERB_GAIN equ 10.0
; AL_REVERB_DEFAULT_LATE_REVERB_GAIN equ 1.26
; AL_REVERB_MIN_LATE_REVERB_DELAY equ 0
; AL_REVERB_MAX_LATE_REVERB_DELAY equ 0.1
; AL_REVERB_DEFAULT_LATE_REVERB_DELAY equ 0.011
; AL_REVERB_MIN_AIR_ABSORPTION_GAINHF equ 0.892
; AL_REVERB_MAX_AIR_ABSORPTION_GAINHF equ 1.0
; AL_REVERB_DEFAULT_AIR_ABSORPTION_GAINHF equ 0.994
; AL_REVERB_MIN_ROOM_ROLLOFF_FACTOR equ 0
; AL_REVERB_MAX_ROOM_ROLLOFF_FACTOR equ 10.0
; AL_REVERB_DEFAULT_ROOM_ROLLOFF_FACTOR equ 0
AL_REVERB_MIN_DECAY_HFLIMIT equ 0
AL_REVERB_MAX_DECAY_HFLIMIT equ 1
AL_REVERB_DEFAULT_DECAY_HFLIMIT equ 1
AL_CHORUS_MIN_WAVEFORM equ 0
AL_CHORUS_MAX_WAVEFORM equ 1
AL_CHORUS_DEFAULT_WAVEFORM equ 1
AL_CHORUS_WAVEFORM_SINUSOID equ 0
AL_CHORUS_WAVEFORM_TRIANGLE equ 1
AL_CHORUS_MIN_PHASE equ -180
AL_CHORUS_MAX_PHASE equ 180
AL_CHORUS_DEFAULT_PHASE equ 90
; AL_CHORUS_MIN_RATE equ 0
; AL_CHORUS_MAX_RATE equ 10.0
; AL_CHORUS_DEFAULT_RATE equ 1.1
; AL_CHORUS_MIN_DEPTH equ 0
; AL_CHORUS_MAX_DEPTH equ 1.0
; AL_CHORUS_DEFAULT_DEPTH equ 0.1
; AL_CHORUS_MIN_FEEDBACK equ -1.0
; AL_CHORUS_MAX_FEEDBACK equ 1.0
; AL_CHORUS_DEFAULT_FEEDBACK equ 0.25
; AL_CHORUS_MIN_DELAY equ 0
; AL_CHORUS_MAX_DELAY equ 0.016
; AL_CHORUS_DEFAULT_DELAY equ 0.016
; AL_DISTORTION_MIN_EDGE equ 0
; AL_DISTORTION_MAX_EDGE equ 1.0
; AL_DISTORTION_DEFAULT_EDGE equ 0.2
; AL_DISTORTION_MIN_GAIN equ 0.01
; AL_DISTORTION_MAX_GAIN equ 1.0
; AL_DISTORTION_DEFAULT_GAIN equ 0.05
; AL_DISTORTION_MIN_LOWPASS_CUTOFF equ 80.0
; AL_DISTORTION_MAX_LOWPASS_CUTOFF equ 24000.0
; AL_DISTORTION_DEFAULT_LOWPASS_CUTOFF equ 8000.0
; AL_DISTORTION_MIN_EQCENTER equ 80.0
; AL_DISTORTION_MAX_EQCENTER equ 24000.0
; AL_DISTORTION_DEFAULT_EQCENTER equ 3600.0
; AL_DISTORTION_MIN_EQBANDWIDTH equ 80.0
; AL_DISTORTION_MAX_EQBANDWIDTH equ 24000.0
; AL_DISTORTION_DEFAULT_EQBANDWIDTH equ 3600.0
; AL_ECHO_MIN_DELAY equ 0
; AL_ECHO_MAX_DELAY equ 0.207
; AL_ECHO_DEFAULT_DELAY equ 0.1
; AL_ECHO_MIN_LRDELAY equ 0
; AL_ECHO_MAX_LRDELAY equ 0.404
; AL_ECHO_DEFAULT_LRDELAY equ 0.1
; AL_ECHO_MIN_DAMPING equ 0
; AL_ECHO_MAX_DAMPING equ 0.99
; AL_ECHO_DEFAULT_DAMPING equ 0.5
; AL_ECHO_MIN_FEEDBACK equ 0
; AL_ECHO_MAX_FEEDBACK equ 1.0
; AL_ECHO_DEFAULT_FEEDBACK equ 0.5
; AL_ECHO_MIN_SPREAD equ -1.0
; AL_ECHO_MAX_SPREAD equ 1.0
; AL_ECHO_DEFAULT_SPREAD equ -1.0
AL_FLANGER_MIN_WAVEFORM equ 0
AL_FLANGER_MAX_WAVEFORM equ 1
AL_FLANGER_DEFAULT_WAVEFORM equ 1
AL_FLANGER_WAVEFORM_SINUSOID equ 0
AL_FLANGER_WAVEFORM_TRIANGLE equ 1
AL_FLANGER_MIN_PHASE equ -180
AL_FLANGER_MAX_PHASE equ 180
AL_FLANGER_DEFAULT_PHASE equ 0
; AL_FLANGER_MIN_RATE equ 0
; AL_FLANGER_MAX_RATE equ 10.0
; AL_FLANGER_DEFAULT_RATE equ 0.27
; AL_FLANGER_MIN_DEPTH equ 0
; AL_FLANGER_MAX_DEPTH equ 1.0
; AL_FLANGER_DEFAULT_DEPTH equ 1.0
; AL_FLANGER_MIN_FEEDBACK equ -1.0
; AL_FLANGER_MAX_FEEDBACK equ 1.0
; AL_FLANGER_DEFAULT_FEEDBACK equ -0.5
; AL_FLANGER_MIN_DELAY equ 0
; AL_FLANGER_MAX_DELAY equ 0.004
; AL_FLANGER_DEFAULT_DELAY equ 0.002
; AL_FREQUENCY_SHIFTER_MIN_FREQUENCY equ 0
; AL_FREQUENCY_SHIFTER_MAX_FREQUENCY equ 24000.0
; AL_FREQUENCY_SHIFTER_DEFAULT_FREQUENCY equ 0
AL_FREQUENCY_SHIFTER_MIN_LEFT_DIRECTION equ 0
AL_FREQUENCY_SHIFTER_MAX_LEFT_DIRECTION equ 2
AL_FREQUENCY_SHIFTER_DEFAULT_LEFT_DIRECTION equ 0
AL_FREQUENCY_SHIFTER_MIN_RIGHT_DIRECTION equ 0
AL_FREQUENCY_SHIFTER_MAX_RIGHT_DIRECTION equ 2
AL_FREQUENCY_SHIFTER_DEFAULT_RIGHT_DIRECTION equ 0
AL_FREQUENCY_SHIFTER_DIRECTION_DOWN equ 0
AL_FREQUENCY_SHIFTER_DIRECTION_UP equ 1
AL_FREQUENCY_SHIFTER_DIRECTION_OFF equ 2
AL_VOCAL_MORPHER_MIN_PHONEMEA equ 0
AL_VOCAL_MORPHER_MAX_PHONEMEA equ 29
AL_VOCAL_MORPHER_DEFAULT_PHONEMEA equ 0
AL_VOCAL_MORPHER_MIN_PHONEMEA_COARSE_TUNING equ -24
AL_VOCAL_MORPHER_MAX_PHONEMEA_COARSE_TUNING equ 24
AL_VOCAL_MORPHER_DEFAULT_PHONEMEA_COARSE_TUNING equ 0
AL_VOCAL_MORPHER_MIN_PHONEMEB equ 0
AL_VOCAL_MORPHER_MAX_PHONEMEB equ 29
AL_VOCAL_MORPHER_DEFAULT_PHONEMEB equ 10
AL_VOCAL_MORPHER_PHONEME_A equ 0
AL_VOCAL_MORPHER_PHONEME_E equ 1
AL_VOCAL_MORPHER_PHONEME_I equ 2
AL_VOCAL_MORPHER_PHONEME_O equ 3
AL_VOCAL_MORPHER_PHONEME_U equ 4
AL_VOCAL_MORPHER_PHONEME_AA equ 5
AL_VOCAL_MORPHER_PHONEME_AE equ 6
AL_VOCAL_MORPHER_PHONEME_AH equ 7
AL_VOCAL_MORPHER_PHONEME_AO equ 8
AL_VOCAL_MORPHER_PHONEME_EH equ 9
AL_VOCAL_MORPHER_PHONEME_ER equ 10
AL_VOCAL_MORPHER_PHONEME_IH equ 11
AL_VOCAL_MORPHER_PHONEME_IY equ 12
AL_VOCAL_MORPHER_PHONEME_UH equ 13
AL_VOCAL_MORPHER_PHONEME_UW equ 14
AL_VOCAL_MORPHER_PHONEME_B equ 15
AL_VOCAL_MORPHER_PHONEME_D equ 16
AL_VOCAL_MORPHER_PHONEME_F equ 17
AL_VOCAL_MORPHER_PHONEME_G equ 18
AL_VOCAL_MORPHER_PHONEME_J equ 19
AL_VOCAL_MORPHER_PHONEME_K equ 20
AL_VOCAL_MORPHER_PHONEME_L equ 21
AL_VOCAL_MORPHER_PHONEME_M equ 22
AL_VOCAL_MORPHER_PHONEME_N equ 23
AL_VOCAL_MORPHER_PHONEME_P equ 24
AL_VOCAL_MORPHER_PHONEME_R equ 25
AL_VOCAL_MORPHER_PHONEME_S equ 26
AL_VOCAL_MORPHER_PHONEME_T equ 27
AL_VOCAL_MORPHER_PHONEME_V equ 28
AL_VOCAL_MORPHER_PHONEME_Z equ 29
AL_VOCAL_MORPHER_MIN_PHONEMEB_COARSE_TUNING equ -24
AL_VOCAL_MORPHER_MAX_PHONEMEB_COARSE_TUNING equ 24
AL_VOCAL_MORPHER_DEFAULT_PHONEMEB_COARSE_TUNING equ 0
AL_VOCAL_MORPHER_MIN_WAVEFORM equ 0
AL_VOCAL_MORPHER_MAX_WAVEFORM equ 2
AL_VOCAL_MORPHER_DEFAULT_WAVEFORM equ 0
AL_VOCAL_MORPHER_WAVEFORM_SINUSOID equ 0
AL_VOCAL_MORPHER_WAVEFORM_TRIANGLE equ 1
AL_VOCAL_MORPHER_WAVEFORM_SAWTOOTH equ 2
; AL_VOCAL_MORPHER_MIN_RATE equ 0
; AL_VOCAL_MORPHER_MAX_RATE equ 10.0
; AL_VOCAL_MORPHER_DEFAULT_RATE equ 1.41
AL_PITCH_SHIFTER_MIN_COARSE_TUNE equ -12
AL_PITCH_SHIFTER_MAX_COARSE_TUNE equ 12
AL_PITCH_SHIFTER_DEFAULT_COARSE_TUNE equ 12
AL_PITCH_SHIFTER_MIN_FINE_TUNE equ -50
AL_PITCH_SHIFTER_MAX_FINE_TUNE equ 50
AL_PITCH_SHIFTER_DEFAULT_FINE_TUNE equ 0
; AL_RING_MODULATOR_MIN_FREQUENCY equ 0
; AL_RING_MODULATOR_MAX_FREQUENCY equ 8000.0
; AL_RING_MODULATOR_DEFAULT_FREQUENCY equ 440.0
; AL_RING_MODULATOR_MIN_HIGHPASS_CUTOFF equ 0
; AL_RING_MODULATOR_MAX_HIGHPASS_CUTOFF equ 24000.0
; AL_RING_MODULATOR_DEFAULT_HIGHPASS_CUTOFF equ 800.0
AL_RING_MODULATOR_MIN_WAVEFORM equ 0
AL_RING_MODULATOR_MAX_WAVEFORM equ 2
AL_RING_MODULATOR_DEFAULT_WAVEFORM equ 0
AL_RING_MODULATOR_SINUSOID equ 0
AL_RING_MODULATOR_SAWTOOTH equ 1
AL_RING_MODULATOR_SQUARE equ 2
; AL_AUTOWAH_MIN_ATTACK_TIME equ 0.0001
; AL_AUTOWAH_MAX_ATTACK_TIME equ 1.0
; AL_AUTOWAH_DEFAULT_ATTACK_TIME equ 0.06
; AL_AUTOWAH_MIN_RELEASE_TIME equ 0.0001
; AL_AUTOWAH_MAX_RELEASE_TIME equ 1.0
; AL_AUTOWAH_DEFAULT_RELEASE_TIME equ 0.06
; AL_AUTOWAH_MIN_RESONANCE equ 2.0
; AL_AUTOWAH_MAX_RESONANCE equ 1000.0
; AL_AUTOWAH_DEFAULT_RESONANCE equ 1000.0
; AL_AUTOWAH_MIN_PEAK_GAIN equ 0.00003
; AL_AUTOWAH_MAX_PEAK_GAIN equ 31621.0
; AL_AUTOWAH_DEFAULT_PEAK_GAIN equ 11.22
AL_COMPRESSOR_MIN_ONOFF equ 0
AL_COMPRESSOR_MAX_ONOFF equ 1
AL_COMPRESSOR_DEFAULT_ONOFF equ 1
; AL_EQUALIZER_MIN_LOW_GAIN equ 0.126
; AL_EQUALIZER_MAX_LOW_GAIN equ 7.943
; AL_EQUALIZER_DEFAULT_LOW_GAIN equ 1.0
; AL_EQUALIZER_MIN_LOW_CUTOFF equ 50.0
; AL_EQUALIZER_MAX_LOW_CUTOFF equ 800.0
; AL_EQUALIZER_DEFAULT_LOW_CUTOFF equ 200.0
; AL_EQUALIZER_MIN_MID1_GAIN equ 0.126
; AL_EQUALIZER_MAX_MID1_GAIN equ 7.943
; AL_EQUALIZER_DEFAULT_MID1_GAIN equ 1.0
; AL_EQUALIZER_MIN_MID1_CENTER equ 200.0
; AL_EQUALIZER_MAX_MID1_CENTER equ 3000.0
; AL_EQUALIZER_DEFAULT_MID1_CENTER equ 500.0
; AL_EQUALIZER_MIN_MID1_WIDTH equ 0.01
; AL_EQUALIZER_MAX_MID1_WIDTH equ 1.0
; AL_EQUALIZER_DEFAULT_MID1_WIDTH equ 1.0
; AL_EQUALIZER_MIN_MID2_GAIN equ 0.126
; AL_EQUALIZER_MAX_MID2_GAIN equ 7.943
; AL_EQUALIZER_DEFAULT_MID2_GAIN equ 1.0
; AL_EQUALIZER_MIN_MID2_CENTER equ 1000.0
; AL_EQUALIZER_MAX_MID2_CENTER equ 8000.0
; AL_EQUALIZER_DEFAULT_MID2_CENTER equ 3000.0
; AL_EQUALIZER_MIN_MID2_WIDTH equ 0.01
; AL_EQUALIZER_MAX_MID2_WIDTH equ 1.0
; AL_EQUALIZER_DEFAULT_MID2_WIDTH equ 1.0
; AL_EQUALIZER_MIN_HIGH_GAIN equ 0.126
; AL_EQUALIZER_MAX_HIGH_GAIN equ 7.943
; AL_EQUALIZER_DEFAULT_HIGH_GAIN equ 1.0
; AL_EQUALIZER_MIN_HIGH_CUTOFF equ 4000.0
; AL_EQUALIZER_MAX_HIGH_CUTOFF equ 16000.0
; AL_EQUALIZER_DEFAULT_HIGH_CUTOFF equ 6000.0
; AL_MIN_AIR_ABSORPTION_FACTOR equ 0
; AL_MAX_AIR_ABSORPTION_FACTOR equ 10.0
; AL_DEFAULT_AIR_ABSORPTION_FACTOR equ 0
; AL_MIN_ROOM_ROLLOFF_FACTOR equ 0
; AL_MAX_ROOM_ROLLOFF_FACTOR equ 10.0
; AL_DEFAULT_ROOM_ROLLOFF_FACTOR equ 0
; AL_MIN_CONE_OUTER_GAINHF equ 0
; AL_MAX_CONE_OUTER_GAINHF equ 1.0
; AL_DEFAULT_CONE_OUTER_GAINHF equ 1.0
AL_MIN_DIRECT_FILTER_GAINHF_AUTO equ 0
AL_MAX_DIRECT_FILTER_GAINHF_AUTO equ 1
AL_DEFAULT_DIRECT_FILTER_GAINHF_AUTO equ 1
AL_MIN_AUXILIARY_SEND_FILTER_GAIN_AUTO equ 0
AL_MAX_AUXILIARY_SEND_FILTER_GAIN_AUTO equ 1
AL_DEFAULT_AUXILIARY_SEND_FILTER_GAIN_AUTO equ 1
AL_MIN_AUXILIARY_SEND_FILTER_GAINHF_AUTO equ 0
AL_MAX_AUXILIARY_SEND_FILTER_GAINHF_AUTO equ 1
AL_DEFAULT_AUXILIARY_SEND_FILTER_GAINHF_AUTO equ 1
; AL_MIN_METERS_PER_UNIT equ 0.00000001
; AL_MAX_METERS_PER_UNIT equ 99999999.9
; AL_DEFAULT_METERS_PER_UNIT equ 1.0
AL_EAXREVERB_DENSITY equ 1
AL_EAXREVERB_DIFFUSION equ 2
AL_EAXREVERB_GAIN equ 3
AL_EAXREVERB_GAINHF equ 4
AL_EAXREVERB_GAINLF equ 5
AL_EAXREVERB_DECAY_TIME equ 6
AL_EAXREVERB_DECAY_HFRATIO equ 7
AL_EAXREVERB_DECAY_LFRATIO equ 8
AL_EAXREVERB_REFLECTIONS_GAIN equ 9
AL_EAXREVERB_REFLECTIONS_DELAY equ 10
AL_EAXREVERB_REFLECTIONS_PAN equ 11
AL_EAXREVERB_LATE_REVERB_GAIN equ 12
AL_EAXREVERB_LATE_REVERB_DELAY equ 13
AL_EAXREVERB_LATE_REVERB_PAN equ 14
AL_EAXREVERB_ECHO_TIME equ 15
AL_EAXREVERB_ECHO_DEPTH equ 16
AL_EAXREVERB_MODULATION_TIME equ 17
AL_EAXREVERB_MODULATION_DEPTH equ 18
AL_EAXREVERB_AIR_ABSORPTION_GAINHF equ 19
AL_EAXREVERB_HFREFERENCE equ 20
AL_EAXREVERB_LFREFERENCE equ 21
AL_EAXREVERB_ROOM_ROLLOFF_FACTOR equ 22
AL_EAXREVERB_DECAY_HFLIMIT equ 23
AL_EFFECT_EAXREVERB equ 8000h
; AL_EAXREVERB_MIN_DENSITY equ 0
; AL_EAXREVERB_MAX_DENSITY equ 1.0
; AL_EAXREVERB_DEFAULT_DENSITY equ 1.0
; AL_EAXREVERB_MIN_DIFFUSION equ 0
; AL_EAXREVERB_MAX_DIFFUSION equ 1.0
; AL_EAXREVERB_DEFAULT_DIFFUSION equ 1.0
; AL_EAXREVERB_MIN_GAIN equ 0
; AL_EAXREVERB_MAX_GAIN equ 1.0
; AL_EAXREVERB_DEFAULT_GAIN equ 0.32
; AL_EAXREVERB_MIN_GAINHF equ 0
; AL_EAXREVERB_MAX_GAINHF equ 1.0
; AL_EAXREVERB_DEFAULT_GAINHF equ 0.89
; AL_EAXREVERB_MIN_GAINLF equ 0
; AL_EAXREVERB_MAX_GAINLF equ 1.0
; AL_EAXREVERB_DEFAULT_GAINLF equ 1.0
; AL_EAXREVERB_MIN_DECAY_TIME equ 0.1
; AL_EAXREVERB_MAX_DECAY_TIME equ 20.0
; AL_EAXREVERB_DEFAULT_DECAY_TIME equ 1.49
; AL_EAXREVERB_MIN_DECAY_HFRATIO equ 0.1
; AL_EAXREVERB_MAX_DECAY_HFRATIO equ 2.0
; AL_EAXREVERB_DEFAULT_DECAY_HFRATIO equ 0.83
; AL_EAXREVERB_MIN_DECAY_LFRATIO equ 0.1
; AL_EAXREVERB_MAX_DECAY_LFRATIO equ 2.0
; AL_EAXREVERB_DEFAULT_DECAY_LFRATIO equ 1.0
; AL_EAXREVERB_MIN_REFLECTIONS_GAIN equ 0
; AL_EAXREVERB_MAX_REFLECTIONS_GAIN equ 3.16
; AL_EAXREVERB_DEFAULT_REFLECTIONS_GAIN equ 0.05
; AL_EAXREVERB_MIN_REFLECTIONS_DELAY equ 0
; AL_EAXREVERB_MAX_REFLECTIONS_DELAY equ 0.3
; AL_EAXREVERB_DEFAULT_REFLECTIONS_DELAY equ 0.007
; AL_EAXREVERB_MIN_LATE_REVERB_GAIN equ 0
; AL_EAXREVERB_MAX_LATE_REVERB_GAIN equ 10.0
; AL_EAXREVERB_DEFAULT_LATE_REVERB_GAIN equ 1.26
; AL_EAXREVERB_MIN_LATE_REVERB_DELAY equ 0
; AL_EAXREVERB_MAX_LATE_REVERB_DELAY equ 0.1
; AL_EAXREVERB_DEFAULT_LATE_REVERB_DELAY equ 0.011
; AL_EAXREVERB_MIN_ECHO_TIME equ 0.075
; AL_EAXREVERB_MAX_ECHO_TIME equ 0.25
; AL_EAXREVERB_DEFAULT_ECHO_TIME equ 0.25
; AL_EAXREVERB_MIN_ECHO_DEPTH equ 0
; AL_EAXREVERB_MAX_ECHO_DEPTH equ 1.0
; AL_EAXREVERB_DEFAULT_ECHO_DEPTH equ 0
; AL_EAXREVERB_MIN_MODULATION_TIME equ 0.04
; AL_EAXREVERB_MAX_MODULATION_TIME equ 4.0
; AL_EAXREVERB_DEFAULT_MODULATION_TIME equ 0.25
; AL_EAXREVERB_MIN_MODULATION_DEPTH equ 0
; AL_EAXREVERB_MAX_MODULATION_DEPTH equ 1.0
; AL_EAXREVERB_DEFAULT_MODULATION_DEPTH equ 0
; AL_EAXREVERB_MIN_AIR_ABSORPTION_GAINHF equ 0.892
; AL_EAXREVERB_MAX_AIR_ABSORPTION_GAINHF equ 1.0
; AL_EAXREVERB_DEFAULT_AIR_ABSORPTION_GAINHF equ 0.994
; AL_EAXREVERB_MIN_HFREFERENCE equ 1000.0
; AL_EAXREVERB_MAX_HFREFERENCE equ 20000.0
; AL_EAXREVERB_DEFAULT_HFREFERENCE equ 5000.0
; AL_EAXREVERB_MIN_LFREFERENCE equ 20.0
; AL_EAXREVERB_MAX_LFREFERENCE equ 1000.0
; AL_EAXREVERB_DEFAULT_LFREFERENCE equ 250.0
; AL_EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR equ 0
; AL_EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR equ 10.0
AL_EAXREVERB_DEFAULT_ROOM_ROLLOFF_FACTOR equ 0
AL_EAXREVERB_MIN_DECAY_HFLIMIT equ 0
AL_EAXREVERB_MAX_DECAY_HFLIMIT equ 1
AL_EAXREVERB_DEFAULT_DECAY_HFLIMIT equ 1

EXTERN alGenBuffers,alDeleteBuffers,alIsBuffer,alBufferData
EXTERN alBufferf,alBuffer3f,alBufferfv,alBufferi,alBuffer3i,alBufferiv
EXTERN alGetBufferf,alGetBuffer3f,alGetBufferfv,alGetBufferi,alGetBuffer3i,alGetBufferiv
EXTERN alGenSources,alDeleteSources,alIsSource
EXTERN alSourcef,alSource3f,alSourcefv,alSourcei,alSource3i,alSourceiv
EXTERN alGetSourcef,alGetSource3f,alGetSourcefv,alGetSourcei,alGetSource3i,alGetSourceiv
EXTERN alSourcePlay,alSourcePlayv
EXTERN alSourcePause,alSourcePausev
EXTERN alSourceStop,alSourceStopv
EXTERN alSourceRewind,alSourceRewindv
EXTERN alSourceQueueBuffers,alSourceUnqueueBuffers
EXTERN alListenerf,alListener3f,alListenerfv,alListeneri,alListener3i,alListeneriv
EXTERN alGetListenerf,alGetListener3f,alGetListenerfv,alGetListeneri,alGetListener3i,alGetListeneriv
EXTERN alEnable,alDisable,alIsEnabled
EXTERN alGetBoolean,alGetDouble,alGetFloat,alGetInteger
EXTERN alGetBooleanv,alGetDoublev,alGetFloatv,alGetIntegerv,alcGetIntegerv
EXTERN alGetString,alcGetString
EXTERN alDistanceModel,alDopplerFactor
EXTERN alDopplerVelocity ; Deprecated in v1.1: use alSpeedOfSound instead!
EXTERN alSpeedOfSound
EXTERN alGetError,alcGetError
EXTERN alIsExtensionPresent,alcIsExtensionPresent
EXTERN alGetProcAddress,alcGetProcAddress
EXTERN alGetEnumValue,alcGetEnumValue
EXTERN alcCreateContext,alcMakeContextCurrent,alcProcessContext,alcSuspendContext,alcDestroyContext
EXTERN alcGetCurrentContext,alcGetContextsDevice
EXTERN alcOpenDevice,alcCloseDevice
EXTERN alcCaptureOpenDevice,alcCaptureCloseDevice,alcCaptureStart,alcCaptureStop,alcCaptureSamples
