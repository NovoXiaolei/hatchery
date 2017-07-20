#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

char *getpwd(char *buf, size_t size);
size_t strcat2(char **dst_out, ...);

#endif
