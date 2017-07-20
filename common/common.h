#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Config{
    const char* ip;
    int port;
    const char* log_file;
    const char* log_path;
}config;

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

#endif
