#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Config{
    char* ip;
    int port;
    char* log_file;
    char* daemon;
}config;

#endif
