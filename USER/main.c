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

extern	volatile int light;
extern	volatile int Count_Pro;
extern volatile int Temperature_Standard;	//温度比较值
extern volatile int Humidity_Standard;		//湿度比较值

int main(void)
{
	int ret = 1;
	uint8_t pBuf[10] = {0};
	uint8_t fBuf[20];
	
	/****************** 硬件初始化 ********************/
	Systick_Init();
	UART1_Init(115200); 
	Led_Init();
	OLED_Clear();
	Exti_Init();
	AT24C02_Init();
	printf("HardWare Init Success!\r\n");
	
/****************** 实现的功能 ********************/
	/*
	*	功能说明：	1，开机进入欢迎页；显示作者姓名，功能说明
	*						2，等待2秒进入温度，湿度监控页面，实时监控
	*						3，按 s1 键在 监控页-设置页 来回切换，切回监控页保存比较值
	*						4，在设置页下按 s2 键上下移动箭头指定修改项
	*						5，在设置页下按 s3 增加数值； s4 减少数值
	*	有待完善：	1，存在屏幕闪烁
	*						2，来回切换等待时间长，看着不流畅。（为削抖做出的牺牲）
	*/
	
	/*********************** 从AT24C02获取温度、湿度比较值 ****************************/
	
	memset(fBuf,0,sizeof(fBuf));
	AT24C02_PageReadData(0 ,fBuf ,7);
	delay_ms(5);
	if(fBuf[0] == 'F'){
		memset(fBuf,0,sizeof(fBuf));
		AT24C02_PageReadData(0 ,fBuf ,7);
		Temperature_Standard = ((int)fBuf[1] - '0')*100 + ((int)fBuf[2] - '0')*10 + ((int)fBuf[3] - '0');
		Humidity_Standard = ((int)fBuf[4] - '0')*100 + ((int)fBuf[5] - '0')*10 + ((int)fBuf[6] - '0');
	}else{																		
		//首次运行创建存储，默认30、31
		pBuf[0] = 'F';
		pBuf[1] = Temperature_Standard / 100 + '0';
		pBuf[2] = Temperature_Standard % 100 / 10 + '0';
		pBuf[3] = Temperature_Standard % 10 + '0';
		pBuf[4] = Humidity_Standard / 100 + '0';
		pBuf[5] = Humidity_Standard % 100 / 10 + '0';
		pBuf[6] = Humidity_Standard % 10 + '0';
		AT24C02_PageWriteData(0 ,pBuf,strlen((char *)pBuf));
	}
	delay_ms(5);
	/*******************************************************************************/
	
	Login(author, role);	//欢迎页，author作者名；role功能说明
	light = 1;
	
	delay_s(2);						//2,延时2s
	
	while(1){
		
		/*********************** 削抖、存储 ************************/
		if(ret != light){
			delay_ms(10);
			Count_Pro = 0;
			ret = light;
			
			//修改AT24C02中比较值
			pBuf[0] = 'F';
			pBuf[1] = Temperature_Standard / 100 + '0';
			pBuf[2] = Temperature_Standard % 100 / 10 + '0';
			pBuf[3] = Temperature_Standard % 10 + '0';
			pBuf[4] = Humidity_Standard / 100 + '0';
			pBuf[5] = Humidity_Standard % 100 / 10 + '0';
			pBuf[6] = Humidity_Standard % 10 + '0';
			AT24C02_PageWriteData(0 ,pBuf,strlen((char *)pBuf));
		}
		/********************************************************/
		
		if(light == 1){							//true进入监控页
			if(Count_Pro == 0){				//判断是否全屏清空
				OLED_Clear();
			}
			
			Start_Proj1();
			
			Count_Pro = 2;						//标志下次不全屏清空
		}else{											//进入设置页面
			
			if(Count_Pro == 0){
				OLED_Clear();
			}
			
			Set_Config();
		}
		
		delay_s(1);									//给温度模块采集留时间
	}

	
		
	
}
