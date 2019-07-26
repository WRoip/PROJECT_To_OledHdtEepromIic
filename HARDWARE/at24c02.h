#ifndef _AT24C02_H_ 
#define _AT24C02_H_

//头文件
#include "stm32f4xx.h"
#include "iic24.h"

//宏定义


//函数声明
void AT24C02_Init(void);
/*
 * 功  能：AT24C02实现页写 
 * 参  数：
 *		addr	--->将数据写入 AT24C02 的 addr 这个地址中
 * 		pBuf	--->保存要写入的数据的首地址
 * 		length	--->你要写入的字节数
 * 返回值：
 *		失败	--->-1
 * 		成功	--->成功写入的字节数
 */
int AT24C02_PageWriteData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length);

#endif
