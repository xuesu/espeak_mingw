#ifndef ESPEAK_UTILS_H
#define ESPEAK_UTILS_H
void strncpy0(char *to,const char *from, int size);
int  GetFileLength(const char *filename);
char *Alloc(int size);
void Free(void *ptr);
#endif