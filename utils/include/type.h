#ifndef  __type___h__
#define  __type___h__

#include <stddef.h> 

typedef unsigned char  uint8;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  int8;                                            /*  �з���8λ���ͱ���           */
typedef unsigned short uint16;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short int16;                                           /*  �з���16λ���ͱ���          */
typedef unsigned int   uint32;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   int32;                                           /*  �з���32λ���ͱ���          */
typedef float          fp32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         fp64;                                            /*  ˫���ȸ�������64λ���ȣ�    */
typedef unsigned long long uint64; 

typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned int   INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */
typedef signed long long int64; 

typedef    signed char I8;
typedef   unsigned char U8 ;  
typedef    signed short  I16;
typedef  unsigned short   U16;
typedef    signed long  I32;
typedef  unsigned long  U32 ;
typedef  I16    I16P ;        
typedef  U16   U16P;


#ifndef UNUSED
#define UNUSED(A)  (void) ((A))
#endif

#if defined(__IAR_SYSTEMS_ICC__)
typedef   char TEXTCHAR;
#define T 
#endif                       

#define TRUE  1
#define FALSE 0
#define FAIL  0
#define SUCCESS 1

#define __out
#define __int

typedef char BOOL;
typedef unsigned int size_t16;
typedef short wchar;
typedef struct {unsigned short x0,y0,x1,y1;} RECT_U16 ;
typedef struct {unsigned int x0,y0,x1,y1;} RECT_U32 ;
typedef struct {int x0,y0,x1,y1;} RECT_I32 ;
typedef struct {unsigned short x,y;} POINT_16;
typedef struct {unsigned short x,y;} POINT_32;

#endif





