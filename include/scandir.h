#ifndef _SCANDIR_
#define _SCANDIR_ 

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

extern struct dirent **namelist;

int search(const char * name);
int jpgfilter(const struct dirent *filename); 

int searchmusic(const char *name);
int musicfilter(const struct dirent *filename);

int searchvideo(const char *name);
int videofilter(const struct dirent *filename);


#endif