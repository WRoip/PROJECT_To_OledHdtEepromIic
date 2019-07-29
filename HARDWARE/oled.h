#ifndef _OLED_H_ 
#define _OLED_H_

//头文件
#include "stm32f4xx.h"
#include "iic.h"

//宏定义
#define MOC_Data		0x40 
#define MOC_Commend	0x00 

//函数声明
/*
 * 功  能：传输一个字节的数据到OLED
 * 参  数：
 *		MOC		--->命令
 * 		data	--->数据
 * 返回值：无
 */
void Out_Oled(uint8_t data, uint8_t MOC );

/*
 * 功  能：OLED显示
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Zh(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num);

/*
 * 功  能：OLED显示8*16位
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Num(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num);

/*
 * 功  能：OLED显示32位
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Zh32(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num);

/*
 * 功  能：OLED设置坐标函数
 * 参  数：
 *		x		--->显示位置的横坐标：取值范围：0~127
 * 		y		--->显示位置的纵坐标：取值范围：0~7
 * 返回值：无
 */
void OLED_SetPos(uint8_t x ,uint8_t y);

/*
 * 功  能：初始化OLED
 * 参  数：无
 * 返回值：无
 */
void Led_Init(void);

/*
 * 功  能：OLED清屏函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_ClearLogin(void);

/*
 * 功  能：OLED清屏函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_Clear(void);

/*
 * 功  能：OLED清M页XY区域函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_ClearXY(uint8_t x, uint8_t y, uint8_t M);

#endif
