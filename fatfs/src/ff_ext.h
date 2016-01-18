#ifndef   __ff_ext__h
#define __ff_ext__h

#include "ff.h"

extern FRESULT f_scandir(char* path , FILINFO*  fileInfo ,unsigned int *nEntrys );
extern FRESULT f_copy(const TCHAR *scrpath,const TCHAR * despath,unsigned char *workbuf,unsigned int sz_workbuf);
extern FRESULT f_fdiskEx (BYTE pdrv,unsigned int beginLBA,const DWORD szt[],void* work);
extern unsigned long long  getpartitionfree(const TCHAR* driverpath);
extern unsigned long long f_getvolsize(const TCHAR *path);
extern void wpathTowfilename(wchar_t *filename,const wchar_t *path);
extern bool fileFixNameReplace(wchar_t *name,const wchar *fixname);
extern void filePrename(wchar_t *prename,const wchar *name);

#endif




