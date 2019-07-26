#include "systick.h"

//初始化滴答定时器
void Systick_Init(void)
{
	//得到的Systick时钟 21MHZ   --->每隔 1/21us 计数一次
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
}

//微秒延时,nus最大值:798915
void delay_us(u32 nus)
{
	u32 temp = 0;
	//往自动重装载除值寄存器写入延时nus  SysTick->LOAD最大值0xFFFFFF
	SysTick->LOAD = nus*21;
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启计数
	do
	{
		//读控制寄存器
		temp = SysTick->CTRL;
	}while( (!(temp & (1<<16))) && (temp&0x01) );
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数
	SysTick->VAL  = 0x00;
}

//毫秒延时,nms最大值，nms最大值798.915
void delay_ms(u32 nms)
{
	u32 temp = 0;
	//往自动重装载除值寄存器写入延时nms  SysTick->LOAD最大值0xFFFFFF
	SysTick->LOAD = nms*21000;
	SysTick->VAL  = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开启计数
	do
	{
		//读控制寄存器
		temp = SysTick->CTRL;
	}while( (!(temp & (1<<16))) && (temp&0x01) );
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数
	SysTick->VAL  = 0x00;

}

//秒延时
void delay_s(u32 ns)
{
	for(; ns>0; ns--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}
