#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include "../../ws.h"

#define LOG(x,...) do { __mingw_printf(x, ##__VA_ARGS__); __mingw_printf("\n"); } while(0)
#define LOGn(x,...) do { __mingw_printf(x, ##__VA_ARGS__); } while(0) //Log without newline

inline void handle_chat(unsigned char *buf, size_t size);
inline void handle_chat_2(unsigned char *buf, size_t size);
#endif //__LOG_H__
