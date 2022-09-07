#include "wavefile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef PLATFORM_DOS
#ifdef PLATFORM_WINDOWS
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#endif

#include "synthesize.h"


FILE* createWavFile(const char *path, int samplerate)
//=================================================
{
	// Set the length of 0x7ffff000 for --stdout
	// This will be changed to the correct length for -w (write to file)
	static unsigned char wave_hdr[44] = {
		'R','I','F','F',0x24,0xf0,0xff,0x7f,'W','A','V','E','f','m','t',' ',
		0x10,0,0,0,1,0,1,0,  9,0x3d,0,0,0x12,0x7a,0,0,
		2,0,0x10,0,'d','a','t','a',  0x00,0xf0,0xff,0x7f};

	if(path == NULL)
		return NULL;

	while(isspace(*path)) path++;

	FILE * fout = NULL;
	if(path[0] != 0)
	{
		if(strcmp(path,"stdout")==0)
		{
#ifdef PLATFORM_WINDOWS
// prevent Windows adding 0x0d before 0x0a bytes
			_setmode(_fileno(stdout), _O_BINARY);
#endif
			fout = stdout;
		}
		else
			fout = fopen(path,"wb");
	}

	if(fout != NULL)
	{
		fwrite(wave_hdr,1,24,fout);
		Write4Bytes(fout,samplerate);
		Write4Bytes(fout,samplerate * 2);
		fwrite(&wave_hdr[32],1,12,fout);
		return fout;
	}
	return NULL;
}   //  end of OpenWaveFile


void finishWavFile(FILE * fout)
//=========================
{
   unsigned int pos;

   if((fout == NULL) || (fout == stdout))
      return;

   fflush(fout);
   pos = ftell(fout);

	fseek(fout,4,SEEK_SET);
	Write4Bytes(fout,pos - 8);

	fseek(fout,40,SEEK_SET);
	Write4Bytes(fout,pos - 44);


   fclose(fout);

} // end of CloseWaveFile

int continueWriteWavFileNoSplit(FILE * fout)
{//=========================
	int finished;
	unsigned char wav_outbuf[1024];
	char fname[210];

	out_ptr = out_start = wav_outbuf;
	out_end = wav_outbuf + sizeof(wav_outbuf);

	finished = WavegenFill(0);

	if(fout != NULL)
	{
		fwrite(wav_outbuf, 1, out_ptr - wav_outbuf, fout);
	}
	return finished;
}  // end of WavegenFile