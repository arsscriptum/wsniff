#include <winsock2.h>
#include <windows.h>
#include <psapi.h>
#include <stdio.h>

#include "ws.h"
#include "misc.h"
#include "plugins.h"

#define MAX_PACKET 4096

struct WS_plugins ws_plugins;
struct WS_handler ws_handlers;

typedef int (MYAPI *tWS)(SOCKET, const char*, unsigned int, int); //For base functions

static DWORD MYAPI initialize(LPVOID param);
static void revert();

static int MYAPI repl_recv(SOCKET s, const char *buf, unsigned int len, int flags);
static int MYAPI repl_send(SOCKET s, const char *buf, unsigned int len, int flags);

//Trampolenes
static int (MYAPI *pRecv)(SOCKET s, const char* buf, unsigned int len, int flags) = NULL; 
static int (MYAPI *pSend)(SOCKET s, const char* buf, unsigned int len, int flags) = NULL;

//Keep track to undo change before closing
static BYTE replaced_send[10];
static BYTE replaced_recv[10];
static DWORD orig_size_send = 0;
static DWORD orig_size_recv = 0;
static UINT_PTR  addr_send = 0; 
static UINT_PTR  addr_recv = 0;

LIBRARY_API DWORD register_handler2(tWS_plugin func, uint8_t tval, const char *comment)
{
	WS_HANDLER_TYPE type = (WS_HANDLER_TYPE)tval;
	return register_handler(func,type,comment);
}
LIBRARY_API void unregister_handler2(DWORD plugin_id, uint8_t tval)
{
	WS_HANDLER_TYPE type = (WS_HANDLER_TYPE)tval;
	return unregister_handler(plugin_id,type);	
}

LIBRARY_API DWORD register_handler(tWS_plugin func, WS_HANDLER_TYPE type, const char *comment)
{
	if(comment == NULL)
		comment = (char*)"";
	struct WS_handler *t = (struct WS_handler*)malloc(sizeof(struct WS_handler));
	t->func = func;
	t->comment = (char*)malloc(sizeof(char)*(strlen(comment)+1));
	strcpy(t->comment,comment);
	if(type & WS_HANDLER_SEND)
		list_add_tail(&(t->ws_handlers_send),&(ws_handlers.ws_handlers_send));
	else
		list_add_tail(&(t->ws_handlers_recv),&(ws_handlers.ws_handlers_recv));
	return (UINT_PTR)(t); //Returns pointer to node we just added
}

LIBRARY_API void unregister_handler(DWORD plugin_id, WS_HANDLER_TYPE type)
{
	if(!plugin_id)
		return;
	if(type & WS_HANDLER_SEND){
		uintptr_t p = (uintptr_t)&((struct WS_handler*)plugin_id)->ws_handlers_send;
		list_del_ptr(p);
	}else{
		uintptr_t p = (uintptr_t) &((struct WS_handler*)plugin_id)->ws_handlers_recv;
		list_del_ptr(p);
	}
	return;
}

BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
		{
#ifdef APPLICATION_NAME		
		 	char moduleName[MAX_PATH];
			GetModuleBaseName(GetCurrentProcess(), NULL, moduleName, MAX_PATH);
			if (strcmp(moduleName, APPLICATION_NAME))
				return FALSE;
#endif				
			CreateThread(NULL,0,initialize,NULL,0,NULL);
			break;
		}
		case DLL_PROCESS_DETACH:
			revert();
			list_for_each(t, &ws_plugins.plugins) //TODO: Change this to use unregister_handler instead, so it'll delete the lists properly :/
				FreeLibrary(list_entry(t, struct WS_plugins, plugins)->plugin);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return TRUE;
}

static DWORD MYAPI initialize(LPVOID param)
{
	DWORD addr;
	BYTE replaced[10];
	DWORD orig_size;


	addr_send = (UINT_PTR)GetProcAddress(GetModuleHandle(TEXT("WS2_32.dll")), "send");
	addr_recv = (UINT_PTR)GetProcAddress(GetModuleHandle(TEXT("WS2_32.dll")), "recv");
	
	//TODO: Clean this area up and move these to some inline function
	addr = addr_send;
	if(apply_patch(0xE9,addr,(void*)(&repl_send),&orig_size_send, replaced_send)) //Note we only store this replaced because this is the original winsock function code, which we need to put back upon closing
	{
		pSend = (tWS)VirtualAlloc(NULL, orig_size_send << 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		memcpy((void*)pSend,replaced_send,orig_size_send);
		apply_patch(0xE9,(UINT_PTR)pSend+orig_size_send,(void*)(addr+orig_size_send),&orig_size, replaced);
	}

	addr = addr_recv;
	if(apply_patch(0xE9,addr,(void*)(&repl_recv),&orig_size_recv, replaced_recv))
	{
		pRecv = (tWS)VirtualAlloc(NULL, orig_size_recv << 2, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		memcpy((void*)pRecv,replaced_recv,orig_size_recv); 
		apply_patch(0xE9,(UINT_PTR)pRecv+orig_size_recv,(void*)(addr+orig_size_recv),&orig_size, replaced); 
	}

	//Initialize lists
	INIT_LIST_HEAD(&ws_handlers.ws_handlers_send);
	INIT_LIST_HEAD(&ws_handlers.ws_handlers_recv);
	INIT_LIST_HEAD(&ws_plugins.plugins);
	load_plugins(PLUGINS_DIRECTORY, &ws_plugins);
	return 0;
}

static void revert()
{
	if(!orig_size_send && !orig_size_recv)
		return;
	if(addr_send)
		exec_copy(addr_send, replaced_send, orig_size_send);
	if(addr_recv)
		exec_copy(addr_recv, replaced_recv, orig_size_recv);
	return;
}

static int MYAPI repl_send(SOCKET s, const char *buf, unsigned int len, int flags)
{
	list_for_each(t, &ws_handlers.ws_handlers_send)
		list_entry(t, struct WS_handler, ws_handlers_send)->func(&s,buf,&len,&flags);
	return pSend(s,buf,len,flags);
}

static int MYAPI repl_recv(SOCKET s, const char *buf, unsigned int len, int flags)
{
	list_for_each(t, &ws_handlers.ws_handlers_recv)
		list_entry(t, struct WS_handler, ws_handlers_recv)->func(&s,buf,&len,&flags);
	return pRecv(s,buf,len,flags);
}
