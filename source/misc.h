#ifndef __MISC_H__
#define __MISC_H__


BOOL apply_patch(BYTE eType, DWORD dwAddress, const void *pTarget,DWORD *orig_size, BYTE *replaced);
void exec_copy(DWORD addr, BYTE *replaced, DWORD orig_size);

#endif // __MISC_H__