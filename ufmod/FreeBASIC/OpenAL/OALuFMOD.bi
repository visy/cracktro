'  uFMOD header file
'  Target OS: Linux (i386)
'  Compiler:  FreeBASIC
'  Driver:    OpenAL

'  function uFMOD_OALPlaySong(
'     byval lpXM    as any ptr,
'     byval param   as uinteger,
'     byval fdwSong as uinteger,
'     byval source  as ALuint
'  ) as uinteger
'  ---
'  Description:
'  ---
'     Loads the given XM song and starts playing it immediately,
'     unless XM_SUSPENDED is specified. It will stop any currently
'     playing song before loading the new one.
'  ---
'  Parameters:
'  ---
'    lpXM
'       Specifies the song to play. If this parameter is 0, any
'       currently playing song is stopped. In such a case, function
'       does not return a meaningful value. fdwSong parameter
'       determines whether this value is interpreted as a filename
'       or as a pointer to an image of the song in memory.
'    param
'       If XM_MEMORY is specified, this parameter should be the size
'       of the image of the song in memory.
'       If XM_FILE is specified, this parameter is ignored.
'    fdwSong
'       Flags for playing the song. The following values are defined:
'       XM_FILE      lpXM points to filename. param is ignored.
'       XM_MEMORY    lpXM points to an image of a song in memory.
'                    param is the image size. Once, uFMOD_OALPlaySong
'                    returns, it's safe to free/discard the memory
'                    buffer.
'       XM_NOLOOP    An XM track plays repeatedly by default. Specify
'                    this flag to play it only once.
'       XM_SUSPENDED The XM track is loaded in a suspended state,
'                    and will not play until the uFMOD_Resume function
'                    is called. This is useful for preloading a song
'                    or testing an XM track for validity.
'    source
'       An OpenAL source name.
'  ---
'  Return Values:
'  ---
'     Returns FALSE on error.
'  ---
'  Remarks:
'  ---
'     If no valid song is specified and there is one currently being
'     played, uFMOD_OALPlaySong just stops playback.
'     Once playback has started, it's not necessary to check for "buffer
'     starvation", since uFMOD performs buffer recovering automatically.
declare function uFMOD_OALPlaySong alias "uFMOD_OALPlaySong" (byval as any ptr, byval as uinteger, byval as uinteger, byval as uinteger) as uinteger
#define uFMOD_StopSong() uFMOD_OALPlaySong(0, 0, 0, 0)

'  sub uFMOD_Jump2Pattern(
'     byval pat as uinteger
'  )
'  ---
'  Description:
'  ---
'     Jumps to the specified pattern index.
'  ---
'  Parameters:
'  ---
'     pat
'        Next zero based pattern index.
'  ---
'  Remarks:
'  ---
'     uFMOD doesn't automatically perform Note Off effects before jumping
'     to the target pattern. In other words, the original pattern will
'     remain in the mixer until it fades out. You can use this feature to
'     your advantage. If you don't like it, just insert leading Note Off
'     commands in all patterns intended to be used as uFMOD_Jump2Pattern
'     targets.
'     if the pattern index lays outside of the bounds of the pattern order
'     table, calling this function jumps to pattern 0, effectively
'     rewinding playback.
declare sub uFMOD_Jump2Pattern alias "uFMOD_Jump2Pattern" (byval as uinteger)
#define uFMOD_Rewind() uFMOD_Jump2Pattern(0)

'  sub uFMOD_Pause
'  ---
'  Description:
'  ---
'     Pauses the currently playing song, if any.
'  ---
'  Remarks:
'  ---
'     While paused you can still control the volume (uFMOD_SetVolume) and
'     the pattern order (uFMOD_Jump2Pattern). The RMS volume coefficients
'     (uFMOD_GetStats) will go down to 0 and the progress tracker
'     (uFMOD_GetTime) will "freeze" while the song is paused.
'     uFMOD_Pause doesn't perform the request immediately. Instead, it
'     signals to pause when playback reaches next chunk of data, which may
'     take up to about 40ms. This way, uFMOD_Pause performs asynchronously
'     and returns very fast. It is not cumulative. So, calling
'     uFMOD_Pause many times in a row has the same effect as calling it
'     once.
'     If you need synchronous pause/resuming, you can use
'     alSourcePause/alSourcePlay functions.
declare sub uFMOD_Pause alias "uFMOD_Pause"

'  sub uFMOD_Resume
'  ---
'  Description:
'  ---
'     Resumes the currently paused song, if any.
'  ---
'  Remarks:
'  ---
'     uFMOD_Resume doesn't perform the request immediately. Instead, it
'     signals to resume when an internal thread gets a time slice, which
'     may take some milliseconds to happen. uFMOD_Resume is not
'     cumulative. So, calling it many times in a row has the same effect
'     as calling it once. If you need synchronous pause/resuming, you can
'     use alSourcePause/alSourcePlay functions.
declare sub uFMOD_Resume alias "uFMOD_Resume"

'  function uFMOD_GetStats as uinteger
'  ---
'  Description:
'  ---
'     Returns the current RMS volume coefficients in (L)eft and (R)ight
'     channels.
'        low-order word: RMS volume in R channel
'        hi-order word:  RMS volume in L channel
'     Range from 0 (silence) to $7FFF (maximum) on each channel.
'  ---
'  Remarks:
'  ---
'     This function is useful for updating a VU meter, like the one
'     included in the example application. It's recommended to rescale
'     the output to log10 (decibels or dB for short), because human ears
'     track volume changes in a dB scale. You may call uFMOD_GetStats()
'     as often as you like, but take in mind that uFMOD updates both
'     channel RMS volumes every 20-40ms, depending on the output sampling
'     rate. So, calling uFMOD_GetStats about 16 times a second whould be
'     quite enough to track volume changes very closely.
declare function uFMOD_GetStats alias "uFMOD_GetStats" as uinteger

'  function uFMOD_GetRowOrder as uinteger
'  ---
'  Description:
'  ---
'     Returns the currently playing row and order.
'        low-order word: row
'        hi-order word:  order
'  ---
'  Remarks:
'  ---
'     This function is useful for synchronization. uFMOD updates both
'     row and order values every 20-40ms, depending on the output sampling
'     rate. So, calling uFMOD_GetRowOrder about 16 times a second whould be
'     quite enough to track row and order progress very closely.
declare function uFMOD_GetRowOrder alias "uFMOD_GetRowOrder" as uinteger

'  function uFMOD_GetTime as uinteger
'  ---
'  Description:
'  ---
'     Returns the time in milliseconds since the song was started.
'  ---
'  Remarks:
'  ---
'     This function is useful for synchronizing purposes. Multimedia
'     applications can use uFMOD_GetTime to synchronize GFX to sound, for
'     example. An XM player can use this function to update a progress meter.
declare function uFMOD_GetTime alias "uFMOD_GetTime" as uinteger

'  function uFMOD_GetTitle as zstring ptr
'  ---
'  Description:
'  ---
'     Returns the current song's title.
'  ---
'  Remarks:
'  ---
'     Not every song has a title, so be prepared to get an empty string.
declare function uFMOD_GetTitle alias "uFMOD_GetTitle" as zstring ptr

'  sub uFMOD_SetVolume(
'     byval vol as uinteger
'  )
'  ---
'  Description:
'  ---
'     Sets the global volume. The volume scale is linear.
'  ---
'  Parameters:
'  ---
'     vol
'        New volume. Range: from uFMOD_MIN_VOL (muting) to uFMOD_MAX_VOL
'        (maximum volume). Any value above uFMOD_MAX_VOL maps to maximum
'        volume.
'  ---
'  Remarks:
'  ---
'     uFMOD internally converts the given values to a logarithmic scale (dB).
'     Maximum volume is set by default. The volume value is preserved across
'     uFMOD_OALPlaySong calls. You can set the desired volume level before
'     actually starting to play a song.
'     You can use OpenAL alSourcef(source, AL_GAIN, gain) function to
'     control the volume in a floating point scale.
declare sub uFMOD_SetVolume alias "uFMOD_SetVolume" (byval as uinteger)

#define XM_MEMORY         1
#define XM_FILE           2
#define XM_NOLOOP         8
#define XM_SUSPENDED      16
#define uFMOD_MIN_VOL     0
#define uFMOD_MAX_VOL     25
#define uFMOD_DEFAULT_VOL 25
