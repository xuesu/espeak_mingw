#ifndef ESPEAK_COMMAND_H
#define ESPEAK_COMMAND_H

#ifndef PLATFORM_WINDOWS
#include <unistd.h>
#endif

#include "common.h"

t_espeak_command* create_espeak_text(const void *text, size_t size, unsigned int position, espeak_POSITION_TYPE position_type, unsigned int end_position, unsigned int flags, void* user_data);

t_espeak_command* create_espeak_mark(const void *text, size_t size, const char *index_mark, unsigned int end_position, unsigned int flags, void* user_data);

t_espeak_command* create_espeak_terminated_msg(unsigned int unique_identifier, void* user_data);

t_espeak_command* create_espeak_key(const char *key_name, void *user_data);

t_espeak_command* create_espeak_char(wchar_t character, void *user_data);

t_espeak_command* create_espeak_parameter(espeak_PARAMETER parameter, int value, int relative);

t_espeak_command* create_espeak_punctuation_list(const wchar_t *punctlist);

t_espeak_command* create_espeak_voice_name(const char *name);

t_espeak_command* create_espeak_voice_spec(espeak_VOICE *voice_spec);

void process_espeak_command( t_espeak_command* the_command);

int delete_espeak_command( t_espeak_command* the_command);

void display_espeak_command(t_espeak_command* the_command);


espeak_ERROR sync_espeak_Synth(unsigned int unique_identifier, const void *text, size_t size, 
		      unsigned int position, espeak_POSITION_TYPE position_type, 
		      unsigned int end_position, unsigned int flags, void* user_data);
espeak_ERROR sync_espeak_Synth_Mark(unsigned int unique_identifier, const void *text, size_t size, 
			   const char *index_mark, unsigned int end_position, 
			   unsigned int flags, void* user_data);
void sync_espeak_Key(const char *key);
void sync_espeak_Char(wchar_t character);
void sync_espeak_SetPunctuationList(const wchar_t *punctlist);
void sync_espeak_SetParameter(espeak_PARAMETER parameter, int value, int relative);
int sync_espeak_SetVoiceByName(const char *name);
int sync_espeak_SetVoiceByProperties(espeak_VOICE *voice_selector);
espeak_ERROR SetVoiceByName(const char *name);
espeak_ERROR SetVoiceByProperties(espeak_VOICE *voice_selector);
void SetParameter(int parameter, int value, int relative);

int sync_espeak_terminated_msg(unsigned int unique_identifier, void* user_data);

//>
#endif
