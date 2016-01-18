

#ifndef __MEM__h__
#define __MEM__h__

#include <wchar.h>




#define MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT-1))


struct os_mem {                                             /* MEMORY CONTROL BLOCK                                   */
    unsigned int         Type;                              /* Should be set to OS_OBJ_TYPE_MEM                       */
    void                *AddrPtr;                           /* Pointer to beginning of memory partition               */
    char            *NamePtr;
    void                *FreeListPtr;                       /* Pointer to list of free memory blocks                  */
    unsigned int          BlkSize;                           /* Size (in bytes) of each block of memory                */
    uint16           NbrMax;                            /* Total number of blocks in this partition               */
    uint16           NbrFree;                           /* Number of memory blocks remaining in this partition    */
};


typedef  struct  os_mem              OS_MEM;


typedef  enum  mem_err {

    MEM_ERR_NONE                      =     0u,
    MEM_ERR_MEM_CREATE_ISR            = 22201u,
    MEM_ERR_MEM_FULL                  = 22202u,
    MEM_ERR_MEM_INVALID_P_ADDR        = 22203u,
    MEM_ERR_MEM_INVALID_BLKS          = 22204u,
    MEM_ERR_MEM_INVALID_PART          = 22205u,
    MEM_ERR_MEM_INVALID_P_BLK         = 22206u,
    MEM_ERR_MEM_INVALID_P_MEM         = 22207u,
    MEM_ERR_MEM_INVALID_P_DATA        = 22208u,
    MEM_ERR_MEM_INVALID_SIZE          = 22209u,
    MEM_ERR_NO_FREE_BLKS          = 22210u,
}MEM_ERR;



extern void memset16(void *s,unsigned short val,unsigned int n);
extern void memset32(void *s,unsigned int val, unsigned int n);
extern BOOL memis(void *mem,unsigned char dat,unsigned int memlen);
extern BOOL memis_32(void *mem,unsigned int dat,unsigned int memlen32);
extern BOOL strendwith(const char *s,const char *endwith);
extern BOOL wcsendwith(const wchar_t *s ,const wchar_t *endwith);



extern void  MemCreate (OS_MEM      *p_mem,
                   char    *p_name,
                   void        *p_addr,
                   unsigned int   n_blks,
                   unsigned int  blk_size,
                   MEM_ERR      *p_err);

extern void  *MemGet (OS_MEM  *p_mem, MEM_ERR  *p_err);

extern void  MemPut (OS_MEM  *p_mem, void    *p_blk, MEM_ERR  *p_err);


#endif

