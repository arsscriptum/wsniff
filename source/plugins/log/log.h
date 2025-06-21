#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include "../../ws.h"

#ifdef __PLATFORM_X64__ 
#define TMP_ARG_TYPE size_t
#else 
#define TMP_ARG_TYPE int
#endif

#ifndef LOGGING_ENABLED

#define LOG(x,...) do { printf(x, ##__VA_ARGS__); printf("\n"); } while(0)
#define LOGn(x,...) do { printf(x, ##__VA_ARGS__); } while(0) //Log without newline

#else

#include <time.h>
FILE *logfile = NULL;

#define LOG(x,y,z) do { fwrite(x,y,z,logfile); fflush(logfile); } while(0)

#endif

#endif //__LOG_H__
