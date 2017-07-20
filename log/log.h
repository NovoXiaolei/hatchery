#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void set_log_path(char *log_path);
void log_print(char* filename, int line, char *fmt,...);

#define LOG_PRINT(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )

#endif
