#ifndef __SYSTICK_H_
#define __SYSTICK_H_

//头文件
#include "stm32f4xx.h"

//宏定义

//函数声明
void Systick_Init(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);
void delay_s(u32 ns);


#endif
