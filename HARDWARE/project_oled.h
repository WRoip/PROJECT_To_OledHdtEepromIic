#ifndef _PROJECT_OLED_H_ 
#define _PROJECT_OLED_H_

/************************* 头文件 *********************/
#include "stm32f4xx.h"
#include "systick.h"
#include "oled.h"
#include "dht.h"

/************************ 函数声明 *********************/
/*
 * 功  能：启动项目
 * 参  数：
 *				name:作者名字
 *				role:功能说明
 * 返回值：无
 */
void Start_Proj1(void);

/*
 * 功  能：获取温度，湿度
 * 参  数：无
 * 返回值：int
 */
void Get_Data(void);


/*
 * 功  能：欢迎页
 * 参  数：
 *				name:作者名字
 *				role:功能说明
 * 返回值：无
 */
void Login(char name[], char role[]);

/*
 * 功  能：设置温度，湿度比较值
 * 参  数：无
 * 返回值：无
 */
void Set_Config(void);

/*
 * 功  能：修改或初始化AT24C02
 * 参  数：
 *				首次进入：choose == 1
 *				修改参数：choose == 0
 * 返回值：无
 */
void Modify_TH(uint8_t choose);

#endif
