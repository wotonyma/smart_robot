#ifndef _TS_H_
#define _TS_H_

#include <stdio.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void get_xy(int *x, int *y);
int open_ts(void);
void get_xy(int *x, int *y);
void close_ts(void);

#endif
