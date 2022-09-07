#ifndef COMMON_H
#define COMMON_H
#include <cstddef>

typedef unsigned short USHORT;
typedef unsigned char  UCHAR;
typedef double DOUBLEX;
typedef unsigned long long long64;   // use this for conversion between pointers and integers
// values for 'value' in espeak_SetParameter(espeakRATE, value, 0), nominally in words-per-minute
#define espeakRATE_MINIMUM  80
#define espeakRATE_MAXIMUM  450
#define espeakRATE_NORMAL   175

#define espeakCHARS_AUTO   0
#define espeakCHARS_UTF8   1
#define espeakCHARS_8BIT   2
#define espeakCHARS_WCHAR  3
#define espeakCHARS_16BIT  4

#define espeakSSML        0x10
#define espeakPHONEMES    0x100
#define espeakENDPAUSE    0x1000
#define espeakKEEP_NAMEDATA 0x2000


#ifdef __WIN32__
#define ESPEAK_API __declspec(dllexport)
#else
#define ESPEAK_API
#endif

#define ESPEAK_API_REVISION  9
#define espeakINITIALIZE_PHONEME_IPA   0x0002   // move this to speak_lib.h, after eSpeak version 1.46.02


#define N_PHONEME_LIST  1000    // enough for source[N_TR_SOURCE] full of text, else it will truncate

#define MAX_HARMONIC  400           // 400 * 50Hz = 20 kHz, more than enough
#define N_SEQ_FRAMES   25           // max frames in a spectrum sequence (real max is ablut 8)
#define STEPSIZE  64                // 2.9mS at 22 kHz sample rate

// flags set for frames within a spectrum sequence
#define FRFLAG_KLATT           0x01   // this frame includes extra data for Klatt synthesizer
#define FRFLAG_VOWEL_CENTRE    0x02   // centre point of vowel
#define FRFLAG_LEN_MOD         0x04   // reduce effect of length adjustment
#define FRFLAG_BREAK_LF        0x08   // but keep f3 upwards
#define FRFLAG_BREAK           0x10   // don't merge with next frame
#define FRFLAG_BREAK_2         0x18   // FRFLAG_BREAK_LF or FRFLAG_BREAK
#define FRFLAG_FORMANT_RATE    0x20   // Flag5 allow increased rate of change of formant freq
#define FRFLAG_MODULATE        0x40   // Flag6 modulate amplitude of some cycles to give trill
#define FRFLAG_DEFER_WAV       0x80   // Flag7 defer mixing WAV until the next frame
#define FRFLAG_LEN_MOD2      0x4000   // reduce effect of length adjustment, used for the start of a vowel
#define FRFLAG_COPIED        0x8000   // This frame has been copied into temporary rw memory

#define SFLAG_SEQCONTINUE      0x01   // a liquid or nasal after a vowel, but not followed by a vowel
#define SFLAG_EMBEDDED         0x02   // there are embedded commands before this phoneme
#define SFLAG_SYLLABLE         0x04   // vowel or syllabic consonant
#define SFLAG_LENGTHEN         0x08   // lengthen symbol : included after this phoneme
#define SFLAG_DICTIONARY       0x10   // the pronunciation of this word was listed in the xx_list dictionary
#define SFLAG_SWITCHED_LANG    0x20   // this word uses phonemes from a different language
#define SFLAG_PROMOTE_STRESS   0x40   // this unstressed word can be promoted to stressed

#define SFLAG_PREV_PAUSE     0x1000   // consider previous phoneme as pause
#define SFLAG_NEXT_PAUSE     0x2000   // consider next phoneme as pause

// embedded command numbers
#define EMBED_P     1   // pitch
#define EMBED_S     2   // speed (used in setlengths)
#define EMBED_A     3   // amplitude/volume
#define EMBED_R     4   // pitch range/expression
#define EMBED_H     5   // echo/reverberation
#define EMBED_T     6   // different tone for announcing punctuation (not used)
#define EMBED_I     7   // sound icon
#define EMBED_S2    8   // speed (used in synthesize)
#define EMBED_Y     9   // say-as commands
#define EMBED_M    10   // mark name
#define EMBED_U    11   // audio uri
#define EMBED_B    12   // break
#define EMBED_F    13   // emphasis
#define EMBED_C    14   // capital letter indication

#define N_EMBEDDED_VALUES    15
extern int embedded_value[N_EMBEDDED_VALUES];
extern int embedded_default[N_EMBEDDED_VALUES];


#define N_PEAKS   9
#define N_PEAKS2  9     // plus Notch and Fill (not yet implemented)
#define N_MARKERS 8

#define N_KLATTP   10   // this affects the phoneme data file format
#define N_KLATTP2  14   // used in vowel files, with extra parameters for future extensions

#define KLATT_AV      0
#define KLATT_FNZ     1    // nasal zero freq
#define KLATT_Tilt    2
#define KLATT_Aspr    3
#define KLATT_Skew    4

#define KLATT_Kopen   5
#define KLATT_AVp     6
#define KLATT_Fric    7
#define KLATT_FricBP  8
#define KLATT_Turb    9

typedef enum {
  espeakEVENT_LIST_TERMINATED = 0, // Retrieval mode: terminates the event list.
  espeakEVENT_WORD = 1,            // Start of word
  espeakEVENT_SENTENCE = 2,        // Start of sentence
  espeakEVENT_MARK = 3,            // Mark
  espeakEVENT_PLAY = 4,            // Audio element
  espeakEVENT_END = 5,             // End of sentence or clause
  espeakEVENT_MSG_TERMINATED = 6,  // End of message
  espeakEVENT_PHONEME = 7,         // Phoneme, if enabled in espeak_Initialize()
  espeakEVENT_SAMPLERATE = 8       // internal use, set sample rate
} espeak_EVENT_TYPE;



typedef struct {
	espeak_EVENT_TYPE type;
	unsigned int unique_identifier; // message identifier (or 0 for key or character)
	int text_position;    // the number of characters from the start of the text
	int length;           // word length, in characters (for espeakEVENT_WORD)
	int audio_position;   // the time in mS within the generated speech output data
	int sample;           // sample id (internal use)
	void* user_data;      // pointer supplied by the calling program
	union {
		int number;        // used for WORD and SENTENCE events.
		const char *name;  // used for MARK and PLAY events.  UTF8 string
		char string[8];    // used for phoneme names (UTF8). Terminated by a zero byte unless the name needs the full 8 bytes.
	} id;
} espeak_EVENT;
/*
   When a message is supplied to espeak_synth, the request is buffered and espeak_synth returns. When the message is really processed, the callback function will be repetedly called.


   In RETRIEVAL mode, the callback function supplies to the calling program the audio data and an event list terminated by 0 (LIST_TERMINATED).

   In PLAYBACK mode, the callback function is called as soon as an event happens.

   For example suppose that the following message is supplied to espeak_Synth:
   "hello, hello."


   * Once processed in RETRIEVAL mode, it could lead to 3 calls of the callback function :

   ** Block 1:
   <audio data> +
   List of events: SENTENCE + WORD + LIST_TERMINATED

   ** Block 2:
   <audio data> +
   List of events: WORD + END + LIST_TERMINATED

   ** Block 3:
   no audio data
   List of events: MSG_TERMINATED + LIST_TERMINATED


   * Once processed in PLAYBACK mode, it could lead to 5 calls of the callback function:

   ** SENTENCE
   ** WORD (call when the sounds are actually played)
   ** WORD
   ** END (call when the end of sentence is actually played.)
   ** MSG_TERMINATED


   The MSG_TERMINATED event is the last event. It can inform the calling program to clear the user data related to the message.
   So if the synthesis must be stopped, the callback function is called for each pending message with the MSG_TERMINATED event.

   A MARK event indicates a <mark> element in the text.
   A PLAY event indicates an <audio> element in the text, for which the calling program should play the named sound file.
*/



typedef enum {
	POS_CHARACTER = 1,
	POS_WORD,
	POS_SENTENCE
} espeak_POSITION_TYPE;


typedef enum {
	/* PLAYBACK mode: plays the audio data, supplies events to the calling program*/
	AUDIO_OUTPUT_PLAYBACK,

	/* RETRIEVAL mode: supplies audio data and events to the calling program */
	AUDIO_OUTPUT_RETRIEVAL,

	/* SYNCHRONOUS mode: as RETRIEVAL but doesn't return until synthesis is completed */
	AUDIO_OUTPUT_SYNCHRONOUS,

	/* Synchronous playback */
	AUDIO_OUTPUT_SYNCH_PLAYBACK

} espeak_AUDIO_OUTPUT;


typedef enum {
	EE_OK=0,
	EE_INTERNAL_ERROR=-1,
	EE_BUFFER_FULL=1,
	EE_NOT_FOUND=2
} espeak_ERROR;

// voice table
typedef struct {
	const char *name;      // a given name for this voice. UTF8 string.
	const char *languages;       // list of pairs of (byte) priority + (string) language (and dialect qualifier)
	const char *identifier;      // the filename for this voice within espeak-data/voices
	unsigned char gender;  // 0=none 1=male, 2=female,
	unsigned char age;     // 0=not specified, or age in years
	unsigned char variant; // only used when passed as a parameter to espeak_SetVoiceByProperties
	unsigned char xx1;     // for internal use
	int score;       // for internal use
	void *spare;     // for internal use
} espeak_VOICE;

typedef enum {
  espeakSILENCE=0, /* internal use */
  espeakRATE=1,
  espeakVOLUME=2,
  espeakPITCH=3,
  espeakRANGE=4,
  espeakPUNCTUATION=5,
  espeakCAPITALS=6,
  espeakWORDGAP=7,
  espeakOPTIONS=8,   // reserved for misc. options.  not yet used
  espeakINTONATION=9,

  espeakRESERVED1=10,
  espeakRESERVED2=11,
  espeakEMPHASIS,   /* internal use */
  espeakLINELENGTH, /* internal use */
  espeakVOICETYPE,  // internal, 1=mbrola
  N_SPEECH_PARAM    /* last enum */
} espeak_PARAMETER;

typedef enum {
  espeakPUNCT_NONE=0,
  espeakPUNCT_ALL=1,
  espeakPUNCT_SOME=2
} espeak_PUNCT_TYPE;

typedef int (t_espeak_callback)(short*, int, espeak_EVENT*);


enum t_espeak_type
  {
    ET_TEXT,
    ET_MARK,
    ET_KEY,
    ET_CHAR,
    ET_PARAMETER,
    ET_PUNCTUATION_LIST,
    ET_VOICE_NAME,
    ET_VOICE_SPEC,
    ET_TERMINATED_MSG
  };

typedef struct 
{
  unsigned int unique_identifier;
  void* text;
  size_t size;
  unsigned int position;
  espeak_POSITION_TYPE position_type;
  unsigned int end_position;
  unsigned int flags;
  void* user_data;
} t_espeak_text;

typedef struct 
{
  unsigned int unique_identifier;
  void* text;
  size_t size;
  const char* index_mark;
  unsigned int end_position;
  unsigned int flags;
  void* user_data;
} t_espeak_mark;

typedef struct 
{
	unsigned int unique_identifier;
	void* user_data;
	wchar_t character; 
} t_espeak_character;

typedef struct 
{
	unsigned int unique_identifier;
	void* user_data;
	const char* key_name; 
} t_espeak_key;


typedef struct 
{
  unsigned int unique_identifier;
  void* user_data;
} t_espeak_terminated_msg;


typedef struct 
{
  espeak_PARAMETER parameter; 
  int value; 
  int relative;
} t_espeak_parameter;

enum t_command_state 
{
  CS_UNDEFINED, // The command has just been created
  CS_PENDING, // stored in the fifo
  CS_PROCESSED // processed
};

typedef struct 
{
  enum t_espeak_type type;
  t_command_state state; 

  union command
  {
    t_espeak_text my_text; 
    t_espeak_mark my_mark; 
    t_espeak_key  my_key; 
    t_espeak_character my_char; 
    t_espeak_parameter my_param;
    const wchar_t* my_punctuation_list;
    const char *my_voice_name;
    espeak_VOICE my_voice_spec;
    t_espeak_terminated_msg my_terminated_msg;
  } u;
} t_espeak_command;

#endif