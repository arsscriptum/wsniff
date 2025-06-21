#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include "ws.h"


void MYAPI log_ws(SOCKET *s, const char *buf, unsigned int *len, int *flags);

#define DBGLOG(x,...) do { printf(x, ##__VA_ARGS__); printf("\n"); } while(0)
#define DBGLOGn(x,...) do { printf(x, ##__VA_ARGS__); } while(0) //Log without newline


#endif //__LOG_H__
