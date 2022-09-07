/***************************************************************************
 *   Copyright (C) 2005 to 2007 by Jonathan Duddington                     *
 *   email: jonsd@users.sourceforge.net                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write see:                           *
 *               <http://www.gnu.org/licenses/>.                           *
 ***************************************************************************/

#ifndef SPEECH_H
#define SPEECH_H

#include <sys/types.h>

// conditional compilation options
#define INCLUDE_KLATT
#define INCLUDE_MBROLA
#define INCLUDE_SONIC

#if defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN
#define ARCH_BIG
#endif

#ifdef __QNX__
#define NEED_GETOPT
#define NO_VARIADIC_MACROS
#endif

#define PATHSEP  '/'
// USE_PORTAUDIO or USE_PULSEAUDIO are now defined in the makefile
//#define USE_PORTAUDIO
//#define USE_PULSEAUDIO
#define USE_NANOSLEEP
#define __cdecl
//#define ESPEAK_API  extern "C"

#ifdef LIBRARY
#define USE_ASYNC
#endif

#ifdef _ESPEAKEDIT
#define USE_PORTAUDIO
#define USE_ASYNC
#define LOG_FRAMES      // write keyframe info to log-espeakedit
#endif

// will look for espeak_data directory here, and also in user's home directory
#ifndef PATH_ESPEAK_DATA
   #define PATH_ESPEAK_DATA  "/usr/share/espeak-data"
#endif

typedef struct {
   const char *mnem;
   int  value;
} MNEM_TAB;
int LookupMnem(MNEM_TAB *table, const char *string);


#ifdef PLATFORM_WINDOWS
#define N_PATH_HOME  230
// sleep(seconds)
// Sleep(millisecond = 1/1000 s), windows api, kernel32.dll
// usleep(microseconds = 1/1000000 s)
#include <windows.h>
#define sleep(x) Sleep(1000*x)
// we are lucky, in current code usleep is always used in multiplies of 1000
#define usleep(x) Sleep((x+500)/1000)
#else /* PLATFORM_POSIX */
#define PLATFORM_POSIX
#define N_PATH_HOME  160
#endif

extern char path_home[N_PATH_HOME];    // this is the espeak-data directory



#endif // SPEECH_H
