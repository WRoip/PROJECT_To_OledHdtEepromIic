#include "rtc.h"

RTC_InitTypeDef RTC_InitStruct;
RTC_DateTypeDef RTC_DataStructure;
RTC_TimeTypeDef RTC_TimeStructure;
/*
 * 功  能：RTC初始化  
 * 参  数：无
 * 返回值：无
 */
void Rtc_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);/*使能后备寄存器访问*/
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) == 0xA0A0)
	{
		RCC_LSEConfig(RCC_LSE_ON);/*使能LSE振荡器*/
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*等待就绪*/
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div2);/*选择RTC时钟源*/
		RCC_RTCCLKCmd(ENABLE);/*使能RTC时钟*/
		RTC_WaitForSynchro();/*等待RTC APB寄存器同步*/
		/*配置RTC数据寄存器和分频器*/
		RTC_InitStruct.RTC_AsynchPrediv	= IS_RTC_ASYNCH_PREDIV(0x7f);
		RTC_InitStruct.RTC_HourFormat		= RTC_HourFormat_24;
		RTC_InitStruct.RTC_SynchPrediv	= IS_RTC_SYNCH_PREDIV(0x7fff);
		RTC_Init(&RTC_InitStruct);
		/*设置年月日和星期*/
		RTC_DataStructure.RTC_Year = 0x19;
		RTC_DataStructure.RTC_Month = 0x7;
		RTC_DataStructure.RTC_Date = 0x06;
		RTC_DataStructure.RTC_WeekDay = 0x07;
		RTC_SetDate(RTC_Format_BCD,&RTC_DataStructure);
		/*设置时分秒 及显示格式*/
		RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours = 0x21;
		RTC_TimeStructure.RTC_Minutes = 0x30;
		RTC_TimeStructure.RTC_Seconds = 0x00;
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure);
		/*配置备份寄存器，表示已经设置过RTC*/
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xA0A0);
	}
	


}







