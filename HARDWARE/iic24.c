#include "iic24.h"

/*
	PB8		--->IIC_SCL
	PB9		--->IIC_SDA
*/

/*
 * 功  能：IIC初始化  
 * 参  数：无
 * 返回值：无
 */
void IIC24_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef  GPIO_InitStruct24;
	
	//1、开启 GPIOB 时钟		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//2、配置 GPIO
	GPIO_InitStruct24.GPIO_Mode  =GPIO_Mode_OUT;    	//输出模式
	GPIO_InitStruct24.GPIO_OType = GPIO_OType_PP;   	//推挽
	GPIO_InitStruct24.GPIO_PuPd  = GPIO_PuPd_UP;    	//上拉
	GPIO_InitStruct24.GPIO_Speed = GPIO_Speed_100MHz; 	//输出速度
	GPIO_InitStruct24.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_8;      	//第 9 8 个引脚
	GPIO_Init(GPIOB, &GPIO_InitStruct24);

	//初始高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_8);																	
}

/*
 * 功  能：IIC_SDA输出模式  
 * 参  数：无
 * 返回值：无
 */
void IIC24_SDAModeOut(void)
{
	//定义结构体变量
	GPIO_InitTypeDef  GPIO_InitStruct24;
	
	//1、开启 GPIOB 时钟		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//2、配置 GPIO
	GPIO_InitStruct24.GPIO_Mode  =GPIO_Mode_OUT;    	//输出模式
	GPIO_InitStruct24.GPIO_OType = GPIO_OType_PP;   	//推挽
	GPIO_InitStruct24.GPIO_PuPd  = GPIO_PuPd_UP;    	//上拉
	GPIO_InitStruct24.GPIO_Speed = GPIO_Speed_100MHz; 	//输出速度
	GPIO_InitStruct24.GPIO_Pin   = GPIO_Pin_9;      	//第 9 个引脚
	GPIO_Init(GPIOB, &GPIO_InitStruct24);																
}

/*
 * 功  能：IIC_SDA输入模式  
 * 参  数：无
 * 返回值：无
 */
void IIC24_SDAModeIn(void)
{
	//定义结构体变量
	GPIO_InitTypeDef  GPIO_InitStruct24;
	
	//1、开启 GPIOB 时钟		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	//2、配置 GPIO
	GPIO_InitStruct24.GPIO_Mode  =GPIO_Mode_IN;    	//输入模式
	GPIO_InitStruct24.GPIO_PuPd  = GPIO_PuPd_UP;    	//上拉
	GPIO_InitStruct24.GPIO_Pin   = GPIO_Pin_9;      	//第 9 个引脚
	GPIO_Init(GPIOB, &GPIO_InitStruct24);																
}

/*
 * 功  能：IIC起始信号函数  ---》当 SCL 为高电平时，SDA 由高变低
 * 参  数：无
 * 返回值：无
 */
void IIC24_Start(void)
{
	//1、设置 SDA 为输出模式
	IIC24_SDAModeOut();
	
	IIC24_SDA_OUT(1);
	IIC24_SCL_OUT(1); 
	delay_us(5);
	
	IIC24_SDA_OUT(0);
	delay_us(5);
	
	IIC24_SCL_OUT(0); 
}

/*
 * 功  能：IIC停止信号函数   ---》当 SCL 为高电平时，SDA 由低变高
 * 参  数：无
 * 返回值：无
 */
void IIC24_Stop(void)	
{
	//1、设置 SDA 为输出模式
	IIC24_SDAModeOut();
	
	IIC24_SDA_OUT(0);
	IIC24_SCL_OUT(0); 
	delay_us(5);
	
	IIC24_SCL_OUT(1);  //执行这一行的时候 SDA 是低电平
	delay_us(5);
	
	IIC24_SDA_OUT(1);
}

/*
 * 功  能：IIC写一个字节函数  --->STM32 写给 24C02
 * 参  数：
 * 		data	---》你要写入的一个字节数据
 * 返回值：无
 */
void IIC24_WriteByte(uint8_t data)	 //01101011
{
	uint8_t i;
	
	//1、设置 SDA 为输出模式
	IIC24_SDAModeOut();
	
	IIC24_SCL_OUT(0); 	//告诉对方不要去识别 SDA 的电平
	delay_us(5);
	
	for(i=0;i<8;i++)	//循环 8次 每次传输一位 ,高位先出
	{
		//准备数据
		if(data & (0x1<<(7-i)))	
		{
			IIC24_SDA_OUT(1);
		}else{
			IIC24_SDA_OUT(0);
		}
		
		IIC24_SCL_OUT(1); 	//告诉对方要去识别 SDA 的电平
		delay_us(5);
		
		IIC24_SCL_OUT(0); 	//告诉对方不要去识别 SDA 的电平
		
	}
}

/*
 * 功  能：IIC读一个字节函数  --->24C02 写给 STM32 
 * 参  数：无
 * 返回值：成功读取的一个字节数据
 */
uint8_t IIC24_ReadByte(void)	 //01101011
{
	uint8_t i;
	uint8_t data = 0;
	
	//1、设置 SDA 为输入模式
	IIC24_SDAModeIn();
	
	IIC24_SCL_OUT(0); 	//当前stm32不识别SDA电平 ，对方现在开始准备数据
	delay_us(5);
	
	for(i=0;i<8;i++)	//循环 8次 每次传输一位 ,高位先出
	{
		IIC24_SCL_OUT(1); 	//当前stm32开始识别SDA电平 ，对方不可以准备数据
		delay_us(5);
		
		if( IIC24_SDA_IN() )
		{
			data |= (0x1<<(7-i));
		}else{
			data &= ~(0x1<<(7-i));
		}
		
		IIC24_SCL_OUT(0); 	//当前stm32不识别SDA电平 ，对方现在开始准备数据
		delay_us(5);      
	}	
	
	return data;
}


/*
 * 功  能：IIC应答函数  --->24C02 写给 STM32 数据，STM32 给 24c02 一个回复
 * 参  数：无
 * 返回值：成功读取的一个字节数据
 */
void IIC24_Ack(uint8_t ack)
{
	//1、设置 SDA 为输出模式
	IIC24_SDAModeOut();
	
	IIC24_SCL_OUT(0); 	//告诉对方不要去识别 SDA 的电平
	delay_us(5);
	
	if(ack)
	{
		IIC24_SDA_OUT(1);
	}else{
		IIC24_SDA_OUT(0);  
	}
	
	IIC24_SCL_OUT(1); 	//告诉对方要去识别 SDA 的电平 
	delay_us(5);
	
	IIC24_SCL_OUT(0); 
}

/*
 * 功  能：IIC等待应答函数  ---> STM32 写给 24C02 数据，24C02给STM32一个回复
 * 参  数：无
 * 返回值：成功得到的回复 
 * 		1	--->不应答
 * 		0	--->应答
 */
uint8_t IIC24_WaitAck(void)
{
	uint8_t ack;
	
	//1、设置 SDA 为输入模式
	IIC24_SDAModeIn();
	
	IIC24_SCL_OUT(0); 	//当前stm32不识别SDA电平 ，对方现在开始准备回复信号
	delay_us(5);
	
	IIC24_SCL_OUT(1); 	//当前stm32开始识别SDA电平
	delay_us(5);
	
	if( IIC24_SDA_IN() )
	{
		ack = 1;
	}else{
		ack = 0;
	}
	
	IIC24_SCL_OUT(0);
	
	return ack;
}






