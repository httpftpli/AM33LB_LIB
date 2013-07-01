#ifndef   __ff_ext__h
#define __ff_ext__h

#include "ff.h"

FRESULT f_scandir(char* path , FILINFO*  fileInfo ,unsigned int *nEntrys );
FRESULT f_copy(const TCHAR *scrpath,const TCHAR * despath,unsigned char *workbuf,unsigned int sz_workbuf);
FRESULT f_fdiskEx (BYTE pdrv,unsigned int beginLBA,const DWORD szt[],void* work);
unsigned int long long  getpartitionfree(const TCHAR* driverpath);

#endif




