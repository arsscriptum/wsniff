#ifndef __MISC_H__
#define __MISC_H__


BOOL apply_patch(BYTE eType, UINT_PTR dwAddress, const void *pTarget,DWORD *orig_size, BYTE *replaced);
void exec_copy(UINT_PTR addr, BYTE *replaced, DWORD orig_size);

#endif // __MISC_H__