#ifndef _AT24C02_READ_H_ 
#define _AT24C02_READ_H_

//头文件
#include "stm32f4xx.h"
#include "iic24.h"

//宏定义


//函数声明
/*
 * 功  能：AT24C02实现读
 * 参  数：
 *		addr	--->从 AT24C02 的 addr 这个地址中将数据读出
 * 		pBuf	--->保存读出的数据的首地址
 * 		length	--->你要读出的字节数
 * 返回值：
 *		失败	--->-1
 * 		成功	--->成功读出的字节数
 */
int AT24C02_PageReadData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length);

#endif
