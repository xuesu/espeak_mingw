#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void strncpy0(char *to,const char *from, int size)
{//===================================================
// strcpy with limit, ensures a zero terminator
	strncpy(to,from,size);
	to[size-1] = 0;
}

int GetFileLength(const char *filename)
{//====================================
	struct stat statbuf;

	if(stat(filename,&statbuf) != 0)
		return(0);

	if((statbuf.st_mode & S_IFMT) == S_IFDIR)
//	if(S_ISDIR(statbuf.st_mode))
		return(-2);  // a directory

	return(statbuf.st_size);
}  // end of GetFileLength


char *Alloc(int size)
{//==================
	char *p;
	if((p = (char *)malloc(size)) == NULL)
		fprintf(stderr,"Can't allocate memory\n");
	return(p);
}

void Free(void *ptr)
{//=================
	if(ptr != NULL)
		free(ptr);
}