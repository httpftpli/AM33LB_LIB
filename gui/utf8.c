
#include "debug.h"
#include "type.h"



unsigned char  UTF8toUCS2(const char * utf8_code, unsigned short* ucs2_code){
       unsigned short temp1, temp2;
       unsigned char utf8size = 0 ;
       
      if(0x00 ==*utf8_code){
            return 0;
      }else if(0x00 == (*utf8_code & 0x80)){
      /* 1 byte UTF-8 Charater.*/
             *ucs2_code= (unsigned short)*utf8_code;
             utf8size = 1;
      }
      else if(0xc0 == (*utf8_code & 0xe0) &&
               0x80 == (*(utf8_code + 1) & 0xc0)
              ){
      /* 2 bytes UTF-8 Charater.*/  
             temp1 = (unsigned short)(*utf8_code & 0x1f);
             temp1 <<= 6;
             temp1 |= (unsigned short)(*(utf8_code + 1) & 0x3f);
             *ucs2_code = temp1;
             utf8size = 2;
      }
      else if( 0xe0 == (*utf8_code & 0xf0) &&
                0x80 == (*(utf8_code +1) & 0xc0) &&
                0x80 == (*(utf8_code + 2) & 0xc0)
              ){
      /* 3bytes UTF-8 Charater.*/
             temp1 = (unsigned short)(*utf8_code &0x0f);
             temp1 <<= 12;
             temp2 = (unsigned short)(*(utf8_code+1) & 0x3F);
             temp2 <<= 6;
             temp1 = temp1 | temp2 | (unsigned short)(*(utf8_code+2) & 0x3F);
             *ucs2_code = temp1;
             utf8size  = 3;
       }
       else{
       /* unrecognize byte. */ 
             *ucs2_code = 0x22e0;
             utf8size = 0;
     }
     return utf8size;
}



unsigned int strLen_UTF8(const char *utf8_string) {
   unsigned int len = 0;
   while (1) {
      if (0x00 == *utf8_string) {
         break;
      } else if (0x00 == (*utf8_string & 0x80)) {
         utf8_string++;
      } else if (0xc0 == (*utf8_string & 0xe0) &&
                 0x80 == (*(utf8_string + 1) & 0xc0)) {
         utf8_string += 2;

      } else if (0xe0 == (*utf8_string & 0xf0) &&
                 0x80 == (*(utf8_string + 1) & 0xc0) &&
                 0x80 == (*(utf8_string + 2) & 0xc0)) {
         utf8_string += 3;
      }
      len++;   
   }
   return len;
}


const  char * strForward_UTF8(const  char *utf8_string,unsigned int forward ){
   unsigned short dummy;
   unsigned int w = 0;
   for (int i=0;i<forward;i++) {
      w += UTF8toUCS2(utf8_string+w,&dummy);      
      if (0==dummy) {
         return NULL;
      }
   }
   return utf8_string+w;
}
    
  

unsigned char UCS2toUTF8(unsigned short ucs2_code, char* utf8_code){
      int length = 0;
      char * out = utf8_code;
      if(!utf8_code){
           return length;
      }
      if(0x0080 > ucs2_code){
      /* 1 byte UTF-8 Character.*/
          *out = (unsigned char)ucs2_code;
          length++;   
      }
      else if(0x0800 > ucs2_code){
      /*2 bytes UTF-8 Character.*/
           *out = (( unsigned char)(ucs2_code >> 6)) | 0xc0;
           *(out+1) = (( unsigned char)(ucs2_code & 0x003F)) | 0x80;
           length += 2;
     }
     else{
     /* 3 bytes UTF-8 Character .*/
        *out = (( unsigned char)(ucs2_code >> 12)) | 0xE0;
        *(out+1) = (( unsigned char)((ucs2_code & 0x0FC0)>> 6)) | 0x80;
        *(out+2) = (( unsigned char)(ucs2_code & 0x003F)) | 0x80;
        length += 3;               
    }
    return length;
}


unsigned int  UTF8toUCS2_string(const char * utf8_string, unsigned short* ucs2_string,unsigned int stringlen){
   unsigned int p=0,i=0;
   if (0==stringlen) {
      stringlen = 0xffffffff;
   }
   for(i=0;i<stringlen;i++){
     p = UTF8toUCS2(utf8_string, &ucs2_string[i]);
     if (p==0){
        break;
     } else{
        utf8_string += p;
     } 
   }
   ucs2_string[i] = 0;
   return i;
}



