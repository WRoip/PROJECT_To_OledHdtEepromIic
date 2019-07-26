#ifndef _KEY_H_
#define _KEY_H_
#define IF_KEY if(1 == Key_Open())goto Led_Off;\
								if(2 == Key_Open())goto Led_On;\
								if(4 == Key_Open())goto beep_Off;\
								if(8 == Key_Open())goto beep_On;

#include "stm32f4xx.h"

 /*
  * 功  能：判断按键是否触发
  * 参  数：void
  * 返回值：uint8_t-->1:按s1; 2:按s2; 4:按s3; 8:按s4
  *
 */
uint8_t Key_Open(void);

 /*
  * 功  能：初始化按键
  * 参  数：void
  * 返回值：void
  *
 */
void Init_Key(void);

#endif
