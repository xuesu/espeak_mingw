#include <stdio.h>
#include <stdarg.h>
#include "speech.h"
#include "debug.h"
#ifdef PLATFORM_WINDOWS
#include <sys/timeb.h>
#include <sys/types.h>
#include <winsock2.h>

int gettimeofday(struct timeval* t,void* timezone)
{       struct _timeb timebuffer;
        _ftime( &timebuffer );
        t->tv_sec=timebuffer.time;
        t->tv_usec=1000*timebuffer.millitm;
		return 0;
}
// from linux's sys/times.h

//#include <features.h>

#define __need_clock_t
#include <time.h>


/* Structure describing CPU time used by a process and its children.  */
struct tms
  {
    clock_t tms_utime;          /* User CPU time.  */
    clock_t tms_stime;          /* System CPU time.  */

    clock_t tms_cutime;         /* User CPU time of dead children.  */
    clock_t tms_cstime;         /* System CPU time of dead children.  */
  };

/* Store the CPU time used by this process and all its
   dead children (and their dead children) in BUFFER.
   Return the elapsed real time, or (clock_t) -1 for errors.
   All times are in CLK_TCKths of a second.  */
clock_t times (struct tms *__buffer){

	__buffer->tms_utime = clock();
	__buffer->tms_stime = 0;
	__buffer->tms_cstime = 0;
	__buffer->tms_cutime = 0;
	return __buffer->tms_utime;
}

typedef long long suseconds_t ;
#else
#include <sys/time.h>
#endif

//#include <unistd.h>

static FILE* fd_log=NULL;
static const char* FILENAME="/tmp/espeak.log";

void debug_init()
{
	if((fd_log = fopen(FILENAME,"a")) != NULL)
		setvbuf(fd_log, NULL, _IONBF, 0);
}

void debug_enter(const char* text)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);                  

  //  fd_log = fopen(FILENAME,"a");
  if (!fd_log)
    {
      debug_init();
    }

  if (fd_log)
    {
      fprintf(fd_log, "%03d.%03dms > ENTER %s\n",(int)(tv.tv_sec%1000), (int)(tv.tv_usec/1000), text);
      //      fclose(fd_log);
    }
}


void debug_show(const char *format, ...)
{
  va_list args;		
  va_start(args, format);
  //  fd_log = fopen(FILENAME,"a");
  if (!fd_log)
    {
      debug_init();
    }
  if (fd_log)
    {
      vfprintf(fd_log, format, args);
      //      fclose(fd_log);
    }  
  va_end(args);
}

void debug_time(const char* text)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);                  

  //  fd_log = fopen(FILENAME,"a");
  if (!fd_log)
    {
      debug_init();
    }
  if (fd_log)
    {
      fprintf(fd_log, "%03d.%03dms > %s\n",(int)(tv.tv_sec%1000), (int)(tv.tv_usec/1000), text);
      //      fclose(fd_log);
    }
}
