#ifndef WAVEFILE_H
#define WAVEFILE_H
#include <stdio.h>
FILE* createWavFile(const char *path, int samplerate);
void finishWavFile(FILE * fout);
int continueWriteWavFileNoSplit(FILE * fout);
#endif