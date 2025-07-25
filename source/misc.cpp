#include <windows.h>
#include "misc.h"

//***************************
//http://stackoverflow.com/questions/13026220/c-function-hook-memory-address-only
#pragma pack(1)
struct patch_t
{
    BYTE nPatchType; //OP code, 0xE9 for JMP
    UINT_PTR dwAddress;
};
#pragma pack()

BOOL apply_patch(BYTE eType, UINT_PTR  dwAddress, const void *pTarget, DWORD *orig_size, BYTE *replaced)
{
	DWORD dwOldValue, dwTemp;
	struct patch_t pWrite =
	{
		eType,
		(UINT_PTR)pTarget - (dwAddress + sizeof(UINT_PTR) + sizeof(BYTE))
	};
	VirtualProtect((LPVOID)dwAddress,sizeof(DWORD),PAGE_EXECUTE_READWRITE,&dwOldValue);
#ifdef __PLATFORM_X64__ 
ReadProcessMemory(GetCurrentProcess(),(LPVOID)dwAddress,(LPVOID)replaced,sizeof(pWrite),(SIZE_T*)orig_size);
#else 
ReadProcessMemory(GetCurrentProcess(),(LPVOID)dwAddress,(LPVOID)replaced,sizeof(pWrite),(PDWORD)orig_size);
#endif

	 //Keep track of the bytes we replaced
	BOOL bSuccess = WriteProcessMemory(GetCurrentProcess(),(LPVOID)dwAddress,&pWrite,sizeof(pWrite),NULL);
	VirtualProtect((LPVOID)dwAddress,sizeof(DWORD),dwOldValue,&dwTemp);
	
    return bSuccess;
}

inline void exec_copy(UINT_PTR addr, BYTE *replaced, DWORD orig_size)
{
		DWORD old_val, temp;
		VirtualProtect((LPVOID)addr,sizeof(DWORD),PAGE_EXECUTE_READWRITE,&old_val);
		memcpy((void*)addr,replaced,orig_size);
		VirtualProtect((LPVOID)addr,sizeof(DWORD),old_val,&temp);
}