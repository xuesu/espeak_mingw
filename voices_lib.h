#ifndef VOICES_LIB_H
#define VOICES_LIB_H
#include "common.h"

#ifdef __cplusplus
extern "C"
#endif
ESPEAK_API const espeak_VOICE **espeak_ListVoices(espeak_VOICE *voice_spec);
/* Reads the voice files from espeak-data/voices and creates an array of espeak_VOICE pointers.
   The list is terminated by a NULL pointer

   If voice_spec is NULL then all voices are listed.
   If voice spec is given, then only the voices which are compatible with the voice_spec
   are listed, and they are listed in preference order.
*/

#ifdef __cplusplus
extern "C"
#endif
ESPEAK_API espeak_VOICE *espeak_GetCurrentVoice(void);
/* Returns the espeak_VOICE data for the currently selected voice.
   This is not affected by temporary voice changes caused by SSML elements such as <voice> and <s>
*/
#endif