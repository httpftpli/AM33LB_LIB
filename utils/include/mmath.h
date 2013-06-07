#ifndef    __mmath__h____
#define    __mmath__h____



#define MIN(A,B)  (((A)>(B))?(B):(A))
#define MAX(A,B)  (((A)>(B))?(A):(B))

#define ABS(A)    (((A)>0 )?(A):(-(A)))

#define NARRAY(array) (sizeof((array))/sizeof((array[0])))



#endif