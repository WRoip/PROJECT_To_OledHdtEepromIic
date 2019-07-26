#ifndef _IIC24_H
#define _IIC24_H 			    

//头文件
#include "stm32f4xx.h"
#include "uart.h"
#include "systick.h"

#define IIC24_SDA_OUT(a);		if(a) \
								GPIO_SetBits(GPIOB,GPIO_Pin_9); \
							else \
								GPIO_ResetBits(GPIOB,GPIO_Pin_9);

#define IIC24_SCL_OUT(a);		if(a) \
								GPIO_SetBits(GPIOB,GPIO_Pin_8); \
							else \
								GPIO_ResetBits(GPIOB,GPIO_Pin_8);
							
#define IIC24_SDA_IN()		GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_9)							
							
//函数声明
/*
 * 功  能：IIC初始化  
 * 参  数：无
 * 返回值：无
 */
void IIC24_Init(void);
							
/*
 * 功  能：IIC起始信号函数  ---》当 SCL 为高电平时，SDA 由高变低
 * 参  数：无
 * 返回值：无
 */
void IIC24_Start(void);
							
/*
 * 功  能：IIC停止信号函数   ---》当 SCL 为高电平时，SDA 由低变高
 * 参  数：无
 * 返回值：无
 */
void IIC24_Stop(void);

/*
 * 功  能：IIC写一个字节函数  --->STM32 写给 24C02
 * 参  数：
 * 		data	---》你要写入的一个字节数据
 * 返回值：无
 */
void IIC24_WriteByte(uint8_t data);

/*
 * 功  能：IIC读一个字节函数  --->24C02 写给 STM32 
 * 参  数：无
 * 返回值：成功读取的一个字节数据
 */
uint8_t IIC24_ReadByte(void);

/*
 * 功  能：IIC应答函数  --->24C02 写给 STM32 数据，STM32 给 24c02 一个回复
 * 参  数：无
 * 返回值：成功读取的一个字节数据
 */
void IIC24_Ack(uint8_t ack);

/*
 * 功  能：IIC等待应答函数  ---> STM32 写给 24C02 数据，24C02给STM32一个回复
 * 参  数：无
 * 返回值：成功得到的回复 
 * 		1	--->不应答
 * 		0	--->应答
 */
uint8_t IIC24_WaitAck(void);





#endif
