#include "oled.h"



void Led_Init()
{
	IIC_Init();
	
	Out_Oled(0xAE,MOC_Commend);//--AE显示，AF关
	Out_Oled(0x00,MOC_Commend);//---页地址模式下设置列起始地址低位(Set Lower Column Start Address For Page Addressing Mode)(00h~0Fh)
	Out_Oled(0x10,MOC_Commend);//---页地址模式下设置列起始地址高位(Set Higher Column Start Address For Page Addressing Mode)(10h~1Fh)
	Out_Oled(0x40,MOC_Commend);//--设置开始行地址  
	Out_Oled(0xB0,MOC_Commend);//--设置页面地址
	Out_Oled(0x81,MOC_Commend);//设置对比度
	Out_Oled(0xFF,MOC_Commend);//--128   
	Out_Oled(0xA1,MOC_Commend);//设置段重映射 
	Out_Oled(0xA6,MOC_Commend);//--A6正常/A7反向
	Out_Oled(0xA8,MOC_Commend);//--设置多路复用率(1到64)
	Out_Oled(0x3F,MOC_Commend);//--1/32 duty
	Out_Oled(0xC8,MOC_Commend);//设置列输出扫描方向(Set COM Output Scan Direction)(C0h/C8h)
	Out_Oled(0xD3,MOC_Commend);//设置显示偏移(Set Display Offset)(D3h)
	Out_Oled(0x00,MOC_Commend);//
	
	Out_Oled(0xD5,MOC_Commend);//设置显示时钟分频值/震荡频率
	Out_Oled(0x80,MOC_Commend);//
	
	Out_Oled(0xD8,MOC_Commend);//设置区域颜色模式(set area color mode off)
	Out_Oled(0x05,MOC_Commend);//
	
	Out_Oled(0xD9,MOC_Commend);//设置预充电周期
	Out_Oled(0xF1,MOC_Commend);//
	 
	Out_Oled(0xDA,MOC_Commend);//设置列引脚硬件配置(Set COM Pins Hardware Configuration)(DAh)
	Out_Oled(0x12,MOC_Commend);//
	
	Out_Oled(0xDB,MOC_Commend);//设置VCOMH反压值(Set VCOMH Deselect Level)(DBh)
	Out_Oled(0x30,MOC_Commend);//
	
	Out_Oled(0x8D,MOC_Commend);//电荷泵设置启用（set charge pump enable）(8Dh)
	Out_Oled(0x14,MOC_Commend);//
	
	Out_Oled(0x26,MOC_Commend); //水平滚动设置
	Out_Oled(0x00,MOC_Commend);
	Out_Oled(0x00,MOC_Commend);
	Out_Oled(0x07,MOC_Commend);
	Out_Oled(0x07,MOC_Commend);
	Out_Oled(0x00,MOC_Commend);
	Out_Oled(0xff,MOC_Commend);
	
	//Out_Oled(0x2F,MOC_Commend);//激活滚动
	Out_Oled(0x2E,MOC_Commend);//关闭滚动
	
	//Out_Oled(0xD6,MOC_Commend); //放大
	//Out_Oled(0x00,MOC_Commend); 
	
	Out_Oled(0xD6,MOC_Commend); //放大禁止
	Out_Oled(0x00,MOC_Commend); 
	
	Out_Oled(0xAF,MOC_Commend);//打开显示(turn on oled panel)(AFh)
	
}
									
/*
 * 功  能：输出一个字节的数据到OLED
 * 参  数：
 *		MOC		--->命令
 * 		data	--->数据
 * 返回值：无
 */
void Out_Oled(uint8_t data, uint8_t MOC )
{
	//开始发送
	IIC_Start();
	
	//发送器件地址及写操作
	IIC_WriteByte(0x78);
	if(IIC_WaitAck()){
		IIC_Stop();
		printf("发送器件地址失败...\r\n");
		return;
	}
	
	//发送命令
	IIC_WriteByte(MOC);
	if(IIC_WaitAck()){
		IIC_Stop();
		printf("发送字地址失败...\r\n");
		return;
	}
	
	//发送数据
	IIC_WriteByte(data);
	if(IIC_WaitAck()){
		IIC_Stop();
		printf("发送数据失败...\r\n");
		return;
	}
	//停止
	IIC_Stop();
}

/*
 * 功  能：OLED显示
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Zh(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num)
{
	
	
	int i;
	OLED_SetPos(x, y);
	for(i = 0; i < 16 ; i++){
		Out_Oled(ch[i + (32*num)], MOC_Data );
	}
	
	OLED_SetPos(x, y+1);
	for(i = 0; i < 16; i++){
		Out_Oled(ch[i + 16 + (32*num)], MOC_Data );
	}
}

/*
 * 功  能：OLED显示8*16位
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Num(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num)
{
	
	
	int i;
	OLED_SetPos(x, y);
	for(i = 0; i < 8 ; i++){
		Out_Oled(ch[i + (16*num)], MOC_Data );
	}
	
	OLED_SetPos(x, y+1);
	for(i = 0; i < 8; i++){
		Out_Oled(ch[i + 8 + (16*num)], MOC_Data );
	}
}

/*
 * 功  能：OLED显示32位
 * 参  数：
 *		x		--->显示区域左上角x轴坐标
 *		y		--->显示区域左上角y轴坐标
 * 		ch	--->数据
 * 		num	--->显示汉字个数
 * 返回值：无
 */
void Show_Zh32(uint8_t x ,uint8_t y ,char ch[] ,uint8_t num)
{
	
	
	int i;
	OLED_SetPos(x, y);
	for(i = 0; i < 32 ; i++){
		Out_Oled(ch[i + (128*num)], MOC_Data );
	}
	
	OLED_SetPos(x, y+1);
	for(i = 0; i < 32; i++){
		Out_Oled(ch[i + 32 + (128*num)], MOC_Data );
	}
	
	OLED_SetPos(x, y+2);
	for(i = 0; i < 32; i++){
		Out_Oled(ch[i + 64 + (128*num)], MOC_Data );
	}
	
	OLED_SetPos(x, y+3);
	for(i = 0; i < 32; i++){
		Out_Oled(ch[i + 96 + (128*num)], MOC_Data );
	}
}

/*
 * 功  能：OLED设置坐标函数
 * 参  数：
 *		x		--->显示位置的横坐标：取值范围：0~127
 * 		y		--->显示位置的纵坐标：取值范围：0~7
 * 返回值：无
 */
void OLED_SetPos(uint8_t x ,uint8_t y)
{ 	
	//1、参数检测
//	if((x>127||x<0) || (y>7||y<0) )
//	{
//		return;
//	}
	
	//2、设置纵坐标
	Out_Oled(0xb0+y,MOC_Commend);
	
	//3、设置横坐标
	Out_Oled(((x&0xf0)>>4)|0x10,MOC_Commend);
	Out_Oled((x&0x0f),MOC_Commend); 
}

/*
 * 功  能：OLED清屏函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_ClearLogin(void)
{
	uint8_t i ,m;
	
	for(m=0;m<8;m++)
	{
		OLED_SetPos(0 ,m);  //确定页
		
		for(i=0;i<128;i++)
		{
			Out_Oled(0x00,MOC_Data);	
		}
	}
}

/*
 * 功  能：OLED清屏函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_Clear(void)
{
	uint8_t i ,m;
	
	for(m=4;m<6;m++)
	{
		OLED_SetPos(0 ,m);  //确定页
		
		for(i=0;i<128;i++)
		{
			Out_Oled(0x00,MOC_Data);	
		}
	}
}

/*
 * 功  能：OLED清M页XY区域函数
 * 参  数：无
 * 返回值：无
 */ 
void OLED_ClearXY(uint8_t x, uint8_t y, uint8_t M)
{
	uint8_t i;
	OLED_SetPos(x ,M);  //确定页
	for(i=x;i<y;i++)
	{
		Out_Oled(0x00,MOC_Data);	
	}
}














