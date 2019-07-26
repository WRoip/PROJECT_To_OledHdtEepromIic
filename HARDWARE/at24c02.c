#include "at24c02.h"


/*
 * 功  能：AT24C02初始化  
 * 参  数：无
 * 返回值：无
 */
void AT24C02_Init(void)
{
	IIC24_Init();
}

/*
 * 功  能：AT24C02实现页写 
 * 参  数：
 *		addr	--->将数据写入 AT24C02 的 addr 这个地址中
 * 		pBuf	--->保存要写入的数据的首地址
 * 		length	--->你要写入的字节数
 * 返回值：
 *		失败	--->-1
 * 		成功	--->成功写入的字节数
 */
int AT24C02_PageWriteData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length)
{
	uint8_t count = 0; //统计写入的字节数
	uint8_t tempAddr = addr;  
	uint8_t i;
	
	for(i=0;i<length;i++)
	{
		//当第一次进入 或者 遇到页边界地址就重新喊开始
		if( (i==0) || (tempAddr%8==0) )
		{
			//STM32发送停止信号 关闭上一次的起始信号
			IIC24_Stop();
			delay_s(1);
			
			//1、STM32发送起始信号
			IIC24_Start();
			
			//2、STM32发送器件地址选择相应设备并且说明待会儿要执行写操作
			IIC24_WriteByte(0xA0);
			if( IIC24_WaitAck() )  //表达式满足说明接收到不应答信号
			{
				IIC24_Stop();
				printf("发送器件地址失败...\r\n");
				return -1;
			}
			//3、STM32发送字地址说明待会儿要把数据写入这个字地址
			IIC24_WriteByte(tempAddr);
			if( IIC24_WaitAck() )  //表达式满足说明接收到不应答信号
			{
				IIC24_Stop();
				printf("发送字地址失败...\r\n");
				return -1;
			}
		}
		
		//4、STM32发送 数据 给 AT24C02
		IIC24_WriteByte(pBuf[count]);
		if( IIC24_WaitAck() )  //表达式满足说明接收到不应答信号
		{
			IIC24_Stop();
			printf("发送数据失败...\r\n");
			return -1;
		}else{
			count++;		//统计写入的字节数
			tempAddr++;   	//将地址往后偏移
		}
	}
	
	//5、STM32发送停止信号
	IIC24_Stop();
	
	return count;
}































