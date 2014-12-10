#ifndef    __mmath__h____
#define    __mmath__h____



#define MIN(A,B)  (((A)>(B))?(B):(A))
#define MAX(A,B)  (((A)>(B))?(A):(B))

#define ABS(A)    (((A)>0 )?(A):(-(A)))
#define BOUND(A,B)  (((A)+(B)-1)/(B)*(B))
#define BOUNDUP(A,B)  (((A)+(B)-1)/(B)*(B))
#define DIVUP(A,B)  (((A)+(B)-1)/(B))

#define NARRAY(array) (sizeof((array))/sizeof((array[0])))
#define lenthof(array) NARRAY((array))

#define BIT(x) (1 << x)


#define bitSet(A,B) do{(A)|=(1UL<<(B));} while(0)
#define bitClr(A,B) do{(A)&=~(1UL<<(B));} while(0)


static inline unsigned int htonl(unsigned int h){
   return h<<24 | h>>24 | ((h&0x00ff0000)>>8) | ((h&0x0000ff00)<<8);
}
static inline unsigned short hton(unsigned short h ){
   return (h<<8 | h>>8);
}

static inline unsigned int htonlAt(void *addr){
  return ((unsigned int)(((unsigned char *)addr)[0])<<24|
          (unsigned int)(((unsigned char *)addr)[1])<<16|
          (unsigned int)(((unsigned char *)addr)[2])<<8|
          (unsigned int)(((unsigned char *)addr)[3])<<0
           );
}

#endif