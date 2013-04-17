#ifndef __UTF8__H__
#define __UTF8__H__


extern unsigned char  UTF8toUCS2(const unsigned char * utf8_code, unsigned short* ucs2_code);
extern unsigned char  UCS2toUTF8(unsigned short ucs2_code, unsigned char* utf8_code);
extern unsigned int   UTF8toUCS2_string(const unsigned char* utf8_string, unsigned short* ucs2_string,unsigned int stringlen);
extern unsigned int   strLen_UTF8(const unsigned char *utf8_string);


#endif

