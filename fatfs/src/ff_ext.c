#include "ffconf.h"
#include "type.h"
#include "ff.h"
#include "debug.h"
#include "wchar.h"

#if _MAX_SS != 512
#define	SS(fs)	((fs)->ssize)	/* Variable sector size */
#else
#define	SS(fs)	512U			/* Fixed sector size */
#endif


FRESULT f_scandir(
    const TCHAR* path , FILINFO*  fileInfo ,unsigned int *nEntrys  /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    DIR dir;
    res = f_opendir(&dir, path);                       /* Open the directory */

    if (FR_OK == res) {
        for (unsigned int i = 0;i<*nEntrys;i++) {
            res = f_readdir(&dir, fileInfo+i);             /* Read a directory item */
            if (res != FR_OK || fileInfo[i].fname[0] == 0){
                *nEntrys = i;
                return res;  /* Break on error or end of dir */
            }
        }
    }

    return res;
}


FRESULT f_copy(const TCHAR *scrpath,const TCHAR * despath, void *workbuf,unsigned int sz_workbuf){
   FRESULT res;
   ASSERT(sz_workbuf%512==0);
   FIL scrfile,desfile;
   res = f_open(&scrfile,scrpath,FA_OPEN_EXISTING|FA_READ);
   if (res!=0) {
      return res;
   }
   res = f_open(&desfile,despath,FA_CREATE_ALWAYS | FA_WRITE);
   if (res!=0) {
      f_close(&scrfile);
      return res;
   }
   unsigned int ss = SS(desfile.fs);
   ASSERT(0==sz_workbuf%ss);
   unsigned int rb,wb;
   while (1) {
      res = f_read(&scrfile, workbuf, sz_workbuf, &rb);
      if (res != 0) {
         goto ERROR;
      }
      res = f_write(&desfile, workbuf, rb, &wb);
      if (res != 0) {
         goto ERROR;
      }
      if (rb!=wb) {
         return FR_DENIED;
      }
      if (rb < sz_workbuf) {
         f_close(&scrfile);
         f_close(&desfile);
         return  FR_OK;
      }
   }

ERROR:
   f_close(&scrfile);
   f_close(&desfile);
   return res;
}


unsigned int long long  getpartitionfree(const TCHAR* driverpath){
   unsigned long  freeclst;
   FATFS *fs;
   FRESULT r ;
   r = f_getfree(driverpath,&freeclst,&fs);
   if (FR_OK!=r) {
      return 0;
   }
   return (long long)freeclst * fs->csize * SS(fs);
}


void wpathTowfilename(wchar_t *filename, const wchar_t *path){
    uint32 len = wcslen(path);
    const wchar_t *point;
    int i;
    for(i=0;i<len;i++){
        if (path[len-i-1]==L'\\'|| path[len-i-1]==L'/'){
            break ;
        }
    }
    point = path+len-i;
    wcscpy(filename,point);
}

bool fileFixNameReplace(wchar_t *name,const wchar *fixname){
    wchar_t *point = wcsrchr(name,L'.');
    if (point==NULL) {
        return false;
    }else{
        wcscpy(point,fixname);
    }
    return true;
}

void filePrename(wchar_t *prename,const wchar *name){
    wcscpy(prename,name);
    wchar_t *point = wcsrchr(prename,L'.');
    if (point!=NULL) {
        *point = 0;
    }
}

