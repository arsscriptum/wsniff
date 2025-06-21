#ifndef __WS_SEND_H__
#define __WS_SEND_H__

#include <windows.h>
#include "targetver.h"
#include "tracelogger.h"
#include "internal_list.h"


typedef void (MYAPI *tWS_plugin)(SOCKET*, const char*, unsigned int*, int*); //For plugin hooks, passes a pointer to all the relevant data EXCEPT buf because that's already a pointer; Pointers can be rather scary.

typedef enum
{
	WS_HANDLER_SEND = 0x1,
	WS_HANDLER_RECV = 0x2
} WS_HANDLER_TYPE;

//I swear the linux linked list implementation is demon magic
struct WS_handler
{
	tWS_plugin func;
	char *comment;
	struct list_head ws_handlers_send; //Contains ordered list of function handlers for send
	struct list_head ws_handlers_recv; //Contains ordered list of function handlers for recv

};

struct WS_plugins
{
    HMODULE plugin;
    struct list_head plugins;
};

LIBRARY_API DWORD register_handler(tWS_plugin func, WS_HANDLER_TYPE type, const char *comment);
LIBRARY_API void unregister_handler(DWORD plugin_id, WS_HANDLER_TYPE type);

EXT LIBRARY_VAR struct WS_plugins ws_plugins;
EXT LIBRARY_VAR struct WS_handler ws_handlers;


#endif //__WS_SEND_H__
