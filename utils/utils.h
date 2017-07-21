#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#include "common.h"

char *getpwd(char *buf, size_t size);
size_t strcat2(char **dst_out, ...);

int load_config(const char *pchConfig, config *pCon);
int write_to_file(const char *pchFilePath, const char* buffer);

#endif
