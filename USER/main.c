#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"
#include "chinese.h"
#include "oledfont.h"
#include "oled.h"
#include "project_oled.h"
#include "exti_0234.h"
#include "at24c02.h"
#include "At24c02_Read.h"
#include "rtc.h"

extern	volatile int light;
extern	volatile int Count_Pro;
extern	RTC_InitTypeDef RTC_InitStruct;
extern	RTC_DateTypeDef RTC_DataStructure;
extern	RTC_TimeTypeDef RTC_TimeStructure;

int main(void)
{
	int ret = 1;
	int colun = 0;
	char buf[30];
//	RCC_ClocksTypeDef Get_RCC_Clocks;
//	uint32_t RCC_BASE_sel = *(uint32_t *)(0x40002834);
	
	/****************** 硬件初始化 ********************/
	Systick_Init();
	UART1_Init(115200); 
	Rtc_Init();
	Led_Init();
	OLED_ClearLogin();
	Exti_Init();
	AT24C02_Init();
	printf("HardWare Init Success!\r\n");
//	printf("%d ", RTC_TR_HT_0);
//	printf("%d\r\n", RTC_TR_HT_1);
//	printf("%d\r\n", RTC_TR_PM);
//	printf("%d\r\n", RCC_BASE_sel);
	
//	while (1)
//{
// RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
//	RTC_GetDate(RTC_Format_BIN, &RTC_DataStructure);
//	sprintf(buf,"20%0.2d/%0.2d/%0.2d", 
//	 RTC_DataStructure.RTC_Year, 
//	 RTC_DataStructure.RTC_Month, 
//	 RTC_DataStructure.RTC_Date);
//	printf("%s",buf);//输出时间到串口
//	printf("\r\n");//换行

//	sprintf(buf,"%0.2d:%0.2d:%0.2d", 
//	 RTC_TimeStructure.RTC_Hours, 
//	 RTC_TimeStructure.RTC_Minutes,
//	 RTC_TimeStructure.RTC_Seconds);
//	printf("%s",buf);//输出日期到串口
//	printf("\r\n");//换行

//	delay_ms(100);
//} 
	
/****************** 实现的功能 ********************/
	/*
	*	功能说明：	1，开机进入欢迎页；显示作者姓名，功能说明
	*						2，等待2秒进入温度，湿度监控页面，实时监控
	*						3，按 s1 键在 监控页-设置页 来回切换，切回监控页保存比较值
	*						4，在设置页下按 s2 键上下移动箭头指定修改项
	*						5，在设置页下按 s3 增加数值； s4 减少数值
	*	有待完善：	
	*						1，来回切换等待时间长，看着不流畅。（为消抖做出的牺牲）
	*/
	
	/*********************** 从AT24C02获取温度、湿度比较值 ****************************/
	Modify_TH(1);
	
	/*******************************************************************************/
	
	Login(author, role);	//欢迎页，author作者名；role功能说明
	light = 1;
	delay_s(2);						//2,延时2s
	OLED_ClearLogin();
	
	while(1){
		/*********************** 消抖、存储 ************************/
		if(ret != light){
			delay_ms(5);
			Count_Pro = 0;
			ret = light;
			
			//修改AT24C02中比较值
			Modify_TH(0);
		}
		/********************************************************/
		
		if(light == 1){							//true进入监控页
			if(Count_Pro == 0){				//判断是否全屏清空
				OLED_Clear();
			}
			
			//3,获取温度，湿度
			if(colun % 10 == 0){
				Get_Data();
				colun = 0;
			}
			
			colun++;
			Start_Proj1();
			Count_Pro = 2;						//标志下次不全屏清空
		}else{											//进入设置页面
			if(Count_Pro == 0){
				OLED_Clear();
			}
			
			Set_Config();
		}
	}

}
