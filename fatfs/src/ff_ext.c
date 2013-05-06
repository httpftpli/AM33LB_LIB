#include "ffconf.h"
#include "type.h"
#include "ff.h"
#include "debug.h"

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



FRESULT f_copy(const TCHAR *scrpath,const TCHAR * despath, unsigned char *workbuf,unsigned int sz_workbuf){
   FRESULT res;
   mdAssert(sz_workbuf%512==0);
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
   mdAssert(0==sz_workbuf%ss);
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
