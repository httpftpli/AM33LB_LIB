unsigned char bcd2hex_2(unsigned char byte){
   return (byte>>4)*10+(byte&0x0f);
}

unsigned short hex2bcd_byte(unsigned char byte){
   unsigned char h,t,i;
   h = byte/100;
   t = byte%100/10;
   i = byte%10;
   return (h<<8)|(t<<4)|(i);
}

unsigned short bcd2hex_4(unsigned short s){
   return ((s>>12)*1000)|(((s&(0x0f<<8))>>8)*100)
           |(((s&(0x0f<<4))>>4)*10)|(s&0x0f);
}

