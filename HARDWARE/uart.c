#include "uart.h"



/******************************** 实现printf输出 ********************************/
#pragma import(__use_no_semihosting) 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 

void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);    //0100 0000
	USART1->DR = (u8) ch;      
	return ch;
}
/*******************************************************************************/



/*
 * UART1_TX		--->PA9
 * UART1_RX		--->PA10
 */

/*
 * 功  能：UART1 的初始化
 * 参  数：
 * 		BaudRate	--->波特率，就是常见的那些 115200 9600 38400 ...
 * 返回值：无
 * 说  明：收发双方的波特率必须一致
 */
void UART1_Init(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef  USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	//1、开启 UART1 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//2、使能 UART1 的 GPIO 口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//3、将 PA9 PA10 复用为 UART1 的特殊功能引脚
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9  , GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10  ,GPIO_AF_USART1);
	
	//4、配置 PA9 PA10 为复用模式
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;    //复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//5、配置 UART1 的信息
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //关闭硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;   //能收能发
	USART_InitStruct.USART_Parity = USART_Parity_No;  //无校验
	USART_InitStruct.USART_StopBits   = USART_StopBits_1; //一位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_Init(USART1, &USART_InitStruct);
	
	//6、开启 UART1 的接收中断   USART_IT_RXNE  --->接收数据寄存器不为空
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//7、初始化 NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	//8、使能 UART1
	USART_Cmd(USART1, ENABLE);
}

uint8_t tempData[20] = {0};     //保存接收的数据
uint8_t RecvData[20] = {0};     //保存接收的数据
uint8_t count = 0;
uint8_t RecvFlag = 0;


//有数据到来就触发这个中断  			--->自定义协议，使用 \r\n 作为结束标志
void USART1_IRQHandler(void)
{
	//进一步判断 UART1 的接收中断是否产生
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		//中断的执行内容
		tempData[count++] = USART_ReceiveData(USART1);
		if( (tempData[count-2]=='\r') && (tempData[count-1]=='\n'))
		{
			strcpy((char *)RecvData,(char *)tempData);
			
			//清空接收内存
			memset(tempData,0,sizeof(tempData));
			count = 0;
			
			RecvFlag = 1;  //接收成功标志
		}
		//清除中断标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}













