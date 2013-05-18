/**
 *  \file   event.h
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup GUI
 *  @{
 *   
 */

//! @}


#ifndef __EVENT__H__
#define __EVENT__H__

#include "type.h"


/**
 * @brief 消息类型

 */
typedef enum tagMSGTYPE {
   MSG_KEYUP =1, 
   MSG_KEYDOWN,
   MSG_TOUCH
} MSGTYPE;


/**
 * @brief 消息结构体

 */
typedef struct tagMSG {
  MSGTYPE  message;       /**< @brief 消息类型 ,参考 MSGTYPE */
  unsigned int  param;    /**< @brief 消息参数 */
  unsigned int  time;     /**< @brief 消息产生的时间 */
  unsigned int  xpt;      /**< @brief 消息产生是当前光标位置的x坐标 */
  unsigned int  ypt;      /**< @brief 消息产生是当前光标位置的y坐标 */
} MSG;


extern BOOL GetMessage (MSG *msg);
extern BOOL SendMessage (MSG *msg);
extern void initMessageRing(void);



#endif
//! @}