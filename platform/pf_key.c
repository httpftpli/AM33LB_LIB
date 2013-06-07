#define KEYSCANCODE_0     0x0402  
#define KEYSCANCODE_1     0x0001
#define KEYSCANCODE_2     0x0101
#define KEYSCANCODE_3     0x0201
#define KEYSCANCODE_4     0x0301
#define KEYSCANCODE_5     0x0401
#define KEYSCANCODE_6     0x0002
#define KEYSCANCODE_7     0x0102
#define KEYSCANCODE_8     0x0202
#define KEYSCANCODE_9     0x0302
#define KEYSCANCODE_DOT   0x0404
#define KEYSCANCODE_ZF    0x0304  
#define KEYSCANCODE_A     0x0003
#define KEYSCANCODE_B     0x0103
#define KEYSCANCODE_C     0x0203
#define KEYSCANCODE_D     0x0303
#define KEYSCANCODE_E     0x0403
#define KEYSCANCODE_F     0x0503
#define KEYSCANCODE_F1    0x0000
#define KEYSCANCODE_F2    0x0100
#define KEYSCANCODE_F3    0x0200
#define KEYSCANCODE_F4    0x0300
#define KEYSCANCODE_F5    0x0400
#define KEYSCANCODE_RIGHT 0x0701
#define KEYSCANCODE_LEFT  0x0501
#define KEYSCANCODE_UP    0x0600
#define KEYSCANCODE_DOWN  0x0602
#define KEYSCANCODE_QUK   0x0204
#define KEYSCANCODE_ESC   0x0700
#define KEYSCANCODE_ENTER 0x0702
#define KEYSCANCODE_PU    0x0703
#define KEYSCANCODE_PD    0x0603
#define KEYSCANCODE_USB   0x0004
#define KEYSCANCODE_CE    0x0601
#define KEYSCANCODE_POP   0x0502

#define KEY_0        	0  	
#define KEY_1        	1
#define KEY_2        	2
#define KEY_3        	3
#define KEY_4       	4
#define KEY_5        	5
#define KEY_6        	6
#define KEY_7        	7
#define KEY_8        	8
#define KEY_9        	9
#define KEY_A		10
#define KEY_B		11
#define KEY_C		12
#define KEY_D		13
#define KEY_E		14
#define KEY_F		15

#define KEY_UP      	16
#define KEY_DOWN    	17
#define KEY_LEFT     	18
#define KEY_RIGHT    	19
#define KEY_PU		20
#define KEY_PD		21

#define KEY_OK      	22
#define KEY_ESC      	23
#define KEY_F1		24
#define KEY_F2		25
#define KEY_F3		26
#define KEY_F4		27
#define KEY_F5		28
#define KEY_F6		29
#define KEY_CE		30		
#define KEY_POP		31

#define KEY_USB		32
#define KEY_MEM		33
#define KEY_QUK		34
#define KEY_ZF		35
#define KEY_POINT	36

#define KEY_RE1		37
#define KEY_RE2		38
#define KEY_RE3		39
#define KEY_No		0xff


unsigned int keyCode(unsigned int scancode){
  unsigned int key;
  switch(scancode){
  case KEYSCANCODE_0 : 
    key =  KEY_0;
    break;
 case KEYSCANCODE_1 :
   key =   KEY_1; 
   break;
 case KEYSCANCODE_2 :
   key =   KEY_2;
break;   
 case KEYSCANCODE_3 : 
key =   KEY_2;  
break;
 case KEYSCANCODE_4  :  
   key =   KEY_4;
   break;
 case KEYSCANCODE_5  :
   key =   KEY_5;
   break;
 case KEYSCANCODE_6  :  
   key =   KEY_6;
   break;
 case KEYSCANCODE_7 :  
   key =   KEY_7;
   break;
 case KEYSCANCODE_8 :  
   key =   KEY_8;
   break;
 case KEYSCANCODE_9 : 
   key =   KEY_9; 
   break;  
 case KEYSCANCODE_DOT:  
   key =   KEY_POINT;
   break;
 case KEYSCANCODE_ZF :
   key =   KEY_ZF;
   break;
 case KEYSCANCODE_A  : 
   key =   KEY_A;
   break;
 case KEYSCANCODE_B :  
   key =   KEY_B ;
   break;
 case KEYSCANCODE_C  :
  key =   KEY_C;
  break;
 case KEYSCANCODE_D : 
key =   KEY_D;
break;
 case KEYSCANCODE_E : 
key =   KEY_E;
break;
 case KEYSCANCODE_F : 
key =  KEY_F;
break;
 case KEYSCANCODE_F1:  
   key =   KEY_F1;
   break;
 case KEYSCANCODE_F2 : 
   key =   KEY_F2;
   break;
 case KEYSCANCODE_F3 : 
   key =   KEY_F3;
   break;
 case KEYSCANCODE_F4 : 
   key =  KEY_F4;
   break;
 case KEYSCANCODE_F5 : 
   key =   KEY_F5;
   break;
 case KEYSCANCODE_RIGHT:
   key =   KEY_RIGHT;
   break;
 case KEYSCANCODE_LEFT : 
   key =   KEY_LEFT;
   break;
 case KEYSCANCODE_DOWN: 
   key =   KEY_DOWN;
   break;
 case KEYSCANCODE_QUK : 
   key =   KEY_QUK;
   break;
 case KEYSCANCODE_ESC :  
   key =   KEY_ESC;
   break;
 case KEYSCANCODE_ENTER:
   key =   KEY_OK;
   break;
 case KEYSCANCODE_PU : 
   key =   KEY_PU;
   break;
 case KEYSCANCODE_PD : 
   key =   KEY_PD;
   break;
 case KEYSCANCODE_USB:  
   key =   KEY_USB;
   break;
 case KEYSCANCODE_CE : 
   key =   KEY_CE;
   break;
 //case KEYSCANCODE_MEM :
  // key =   KEY_MEM;
 //  break;
 case KEYSCANCODE_POP :  
   key =   KEY_POP;
   break;
  default:
   key =   KEY_No;
   break ; 
  }
  return key;
}
