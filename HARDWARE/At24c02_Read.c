#include "At24c02_Read.h"

/*
 * 功  能：AT24C02实现读
 * 参  数：
 *		addr	--->从 AT24C02 的 addr 这个地址中将数据读出
 * 		pBuf	--->保存读出的数据的地址
 * 		length	--->你要读出的字节数
 * 返回值：
 *		失败	--->-1
 * 		成功	--->成功读出的字节数
 */
int AT24C02_PageReadData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length)
{
	uint8_t count = 0; //统计写入的字节数
	
	int i = 0;
	
	//1、STM32发送起始信号
	IIC24_Start();
	
	//2、STM32发送器件地址选择相应设备并且说明待会儿要执行写操作
	IIC24_WriteByte(0xA0);
	if( IIC24_WaitAck() )  
	{
		IIC24_Stop();
		printf("发送器件地址失败...\r\n");
		return -1;
	}
	
	//3、STM32从这个字地址开始读数据
	IIC24_WriteByte(addr);
	IIC24_Ack(0);
	
	
	//4、STM32发送器件地址选择相应设备并且说明待会儿要执行读操作
	IIC24_Start();
	IIC24_WriteByte(0xA1);
	IIC24_Ack(0);
	
	//5循环读lebght个数据
	for(;i < length; i++){
		pBuf[i] = IIC24_ReadByte();
		IIC24_Ack(0);
	}
	
	//6，结束
	//pBuf[i] = '\r';
	//pBuf[i+1] = '\n';
	IIC24_Ack(1);
	IIC24_Stop();
	
	
	return count;
}































