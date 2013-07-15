 
#include "lib_gui.h"
#include "pf_lcd.h"
#include "type.h"
#include "mmath.h"


//extern GUI_FONT  GUI_Font_ascii_16,GUI_Font_ascii_20;


extern GUI_CONTEXT GUI_Context;
 
/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIPROP_FindChar
*/
static const GUI_FONT_PROP   * GUIPROP_FindChar(const GUI_FONT_PROP  * pProp, U16P c) {
  for (; pProp; pProp = pProp->pNext) {
    if ((c>=pProp->First) && (c<=pProp->Last))
      break;
  }
  return pProp;
}



/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUIPROP_DispChar
*
* Purpose:
*   This is the routine that displays a character. It is used by all
*   other routines which display characters as a subroutine.
*/
void GUIPROP_DispChar(U16P c) {
  const GUI_FONT_PROP   * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  if (!pProp){
     c = '?';
     pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  }
  const GUI_CHARINFO   * pCharInfo = pProp->paCharInfo+(c-pProp->First);
  unsigned short x = GUI_Context.DispPosX;
  unsigned short y = GUI_Context.DispPosY;
  unsigned int addr =(unsigned int)(pCharInfo->pData);
  unsigned int width = pCharInfo->XSize;
  unsigned int dis = pCharInfo->XDist;
  unsigned int nbyteperline = pCharInfo->BytesPerLine;
  for (int i = 0; i < GUI_Context.pAFont->YSize; i++) {    
      unsigned int mask = htonlAt((void*)(addr+i*nbyteperline));
      for(int j=0;j<width;j++){
        GUI_COLOR color = (mask & (1<<(31-j))) ?  GUI_Context.Color: GUI_Context.BkColor;
        LCD_SetPixel(x + j, y + i, color);
      }
  }
   GUI_Context.DispPosX += dis;
}


/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_GetCharDistX(U16P c){
  const GUI_FONT_PROP   * pProp = GUIPROP_FindChar(GUI_Context.pAFont->p.pProp, c);
  return (pProp) ? (pProp->paCharInfo+(c-pProp->First))->XSize * GUI_Context.pAFont->XMag : 0;
}

/*********************************************************************
*
*       GUIPROP_GetFontInfo
*/
void GUIPROP_GetFontInfo(const GUI_FONT   * pFont, GUI_FONTINFO* pfi) {
  UNUSED(pFont);
  pfi->Flags = GUI_FONTINFO_FLAG_PROP;
}

/*********************************************************************
*
*       GUIPROP_IsInFont
*/
char GUIPROP_IsInFont(const GUI_FONT * pFont, U16 c) {
  const GUI_FONT_PROP   * pProp = GUIPROP_FindChar(pFont->p.pProp, c);
  return (pProp==NULL) ? 0 : 1;
}






/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
