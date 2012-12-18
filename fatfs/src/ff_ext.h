#ifndef   __ff_ext__h
#define __ff_ext__h

#include "ff.h"

FRESULT f_scandir(char* path , FILINFO*  fileInfo ,unsigned int *nEntrys );
FRESULT f_copy(TCHAR *scrpath,TCHAR * despath,unsigned char *workbuf,unsigned int sz_workbuf);

#endif



