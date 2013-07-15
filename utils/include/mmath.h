#ifndef    __mmath__h____
#define    __mmath__h____



#define MIN(A,B)  (((A)>(B))?(B):(A))
#define MAX(A,B)  (((A)>(B))?(A):(B))

#define ABS(A)    (((A)>0 )?(A):(-(A)))
#define BOUND(A,B)  (A+B-1)/B*B 

#define NARRAY(array) (sizeof((array))/sizeof((array[0])))

#define BIT(x) (1 << x)


static inline unsigned int htonl(unsigned int h){
  return h<<24 | h>>24 | ((h&0x00ff0000)>>8) | ((h&0x0000ff00)<<8);
}

#endif