#include <windows.h>
#include <dirent.h>
#include <string.h>
#include "tracelogger.h"
#include "plugins.h"

void load_plugins(LPCTSTR directory, struct WS_plugins *list)
{
	DIR *dir;
	struct dirent *ent;
	struct WS_plugins *t;	
	LOG_TRACE("wsniff::load_plugins","DIR %s",directory);

	if ((dir = opendir(directory)) != NULL) 
	{
		LOG_TRACE("wsniff::load_plugins::opendir","opendir %s",directory);
		while((ent = readdir(dir)) != NULL)
		{
			char *str = (char*)calloc(MAX_PATH,sizeof(char));
			strcat(str,directory);
			strcat(str,ent->d_name);
			LOG_TRACE("wsniff::load_plugins::loading","loading %s",str);
			HMODULE h = LoadLibrary(str);
			if(h != NULL)
			{
				t = (struct WS_plugins*)malloc(sizeof(struct WS_plugins));
				t->plugin = h;
				list_add(&(t->plugins),&(list->plugins));
			}
		}
		closedir (dir);
	}
	return;
}
