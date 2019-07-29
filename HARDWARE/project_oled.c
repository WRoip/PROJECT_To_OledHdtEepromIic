#include "project_oled.h"


extern	char					temperature	[];		//外部声明的字模
extern	char							humidity[];
extern	char								state	[];
extern	char								config[];
extern	const unsigned char	 F8X16[];

//extern	int	 Read_DHT11(char *);				//外部声明的函数
extern int AT24C02_PageReadData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length);
extern int AT24C02_PageWriteData(uint8_t addr ,uint8_t pBuf[] ,uint8_t length);
extern volatile	int Count_Pro;					//外部声明的变量
extern	volatile int light;

volatile int Temperature_Standard	= 30;	//温度比较值
volatile int Humidity_Standard		= 31;	//湿度比较值
volatile int Temperature;								//温度
volatile int Humidity;									//湿度

/*
 * 功  能：启动项目
 * 参  数：无
 * 返回值：无
 */
void Start_Proj1()
{
	int i;
	int j = 1;
	int Temperature_Obj;
	int Humidity_Obj;
	
	
	//4,界面2，采集打印温度 湿度 判断状态
	/******************** 显示温度 *********************/
	for(i = 0; i < 5 && Count_Pro== 0; i++){
			Show_Zh( 16*i, 0, temperature, i);
	}
	
	Temperature_Obj = Temperature;
	j = 1;
	while(1){
		if(Temperature_Obj / 10 != 0){
			j *= 10;
			Temperature_Obj /= 10;
		}else{
			break;
		}
	}
	if(light != 1)return;
	
	/************ 局部清屏 ************/
	if(Temperature < 10){
		OLED_ClearXY(96, 128, 0);
		OLED_ClearXY(96, 128, 1);
	}else if(Temperature < 100){
		OLED_ClearXY(104, 128, 0);
		OLED_ClearXY(104, 128, 1);
	}else{
		OLED_ClearXY(112, 128, 0);
		OLED_ClearXY(112, 128, 1);
	}
	
	i = 0;
	Temperature_Obj = Temperature;
	while(Temperature_Obj != 0 || j != 0){
		Show_Num(72 + 8*i, 0, (char *)F8X16, (Temperature_Obj / j + 16));
		i++;
		Temperature_Obj = Temperature_Obj % j;
		j /= 10;
	}
	
	Show_Zh(72 + 8*i, 0, temperature, 5);			//打印°C
	
	/********************* 显示湿度 ****************************/
	for(i = 0; i < 5 && Count_Pro== 0; i++){
			Show_Zh( 16*i, 2, humidity, i);
	}
	
	Humidity_Obj = Humidity;
	j = 1;
	while(1){
		if(Humidity_Obj / 10 != 0){
			j *= 10;
			Humidity_Obj /= 10;
		}else{
			break;
		}
	}
	if(light != 1)return;
	
	/************ 局部清屏 ************/
	if(Humidity < 10){
		OLED_ClearXY(104, 128, 2);
		OLED_ClearXY(104, 128, 3);
	}else{
		OLED_ClearXY(112, 128, 2);
		OLED_ClearXY(112, 128, 3);
	}
	
	i = 0;
	Humidity_Obj = Humidity;
	while(Humidity_Obj != 0 || j != 0){
		Show_Num(72 + 8*i, 2, (char *)F8X16, (Humidity_Obj / j + 16));
		i++;
		Humidity_Obj %= j;
		j /= 10;
	}
	
	Show_Num(72 + 8*i, 2, (char *)F8X16, 5);			//打印%RH
	i++;
	Show_Num(72 + 8*i, 2, (char *)F8X16, 50);
	i++;
	Show_Num(72 + 8*i, 2, (char *)F8X16, 40);
	if(light != 1)return;
	
	/************************* 显示状态 ***************************/
	for(i = 0; i < 5 && Count_Pro== 0; i++){
			Show_Zh( 16*i, 4, state, i);
	}
	if(Temperature_Standard > Temperature){
		Show_Zh( 16*5, 4, state, 5);
		Show_Zh( 16*6, 4, state, 6);

	}else{
		Show_Zh( 16*5, 4, state, 7);
		Show_Zh( 16*6, 4, state, 8);
	}
}

/*
 * 功  能：获取温度,湿度
 * 参  数：无
 * 返回值：无
 */
void Get_Data()
{
	char data[5];
	DHT11_Init();          				//
	Read_DHT11(data);
	Humidity		= (int)data[0];		//定义全局变量存储
	Temperature	= (int)data[2];
}

/*
 * 功  能：欢迎页
 * 参  数：
 *				name:作者名字
 *				role:功能说明
 * 返回值：无
 */
void Login(char name[], char role[])
{
	/******* 1,界面1，显示作者 功能 ***********/
	int i;
	for(i = 0; i < 3; i++){
			Show_Zh32( 32*i, 0, name, i);
	}
	Show_Zh32( 32*i - 16, 0, name, i);
	
	for(i = 0; i < 7; i++){
			Show_Zh( 16*i, 4, role, i);
	}
	
	for(i = 7; i < 14; i++){
			Show_Zh( 16*(i - 7), 6, role, i);
	}
}

/*
 * 功  能：设置温度，湿度比较值
 * 参  数：无
 * 返回值：无
 */
void Set_Config()
{
	int i;
	int j = 0;
	int Temperature_Standard_Obj;
	int Humidity_Standard_Obj;
	
	
	//5,界面3，调整温度，湿度比较值
	/************** 显示温度比较值 ******************/
	for(i = 0; i < 2 && Count_Pro == 0; i++){
			Show_Zh( 16*i, 0, config, i);
	}
	
	for(i = 4; i < 8 && Count_Pro == 0; i++){
			Show_Zh( 16*(i - 2), 0, config, i);
	}
	
	Temperature_Standard_Obj = Temperature_Standard;
	j = 1;
	while(1){
		if(Temperature_Standard_Obj / 10 != 0){
			j *= 10;
			Temperature_Standard_Obj /= 10;
		}else{
			break;
		}
	}
	
	i = 0;
	Temperature_Standard_Obj = Temperature_Standard;
	while(Temperature_Standard_Obj != 0 || j != 0){
		Show_Num(88 + 8*i, 0, (char *)F8X16, (Temperature_Standard_Obj / j + 16));
		i++;
		Temperature_Standard_Obj = Temperature_Standard_Obj % j;
		j /= 10;
	}
	
	//显示箭头
	if(Count_Pro == 2 || Count_Pro == 0){
			OLED_ClearXY(104, 128, 2);
			OLED_ClearXY(104, 128, 3);
			Show_Zh(88 + 8*i , 0, config, 8);
	}
	if(light == 1)return;
	
	/**************** 显示湿度比较值 ********************/
	for(i = 2; i < 8 && Count_Pro == 0; i++){
			Show_Zh( 16*(i - 2), 2, config, i);
	}
	
	Humidity_Standard_Obj = Humidity_Standard;
	j = 1;
	while(1){
		if(Humidity_Standard_Obj / 10 != 0){
			j *= 10;
			Humidity_Standard_Obj /= 10;
		}else{
			break;
		}
	}
	
	i = 0;
	Humidity_Standard_Obj = Humidity_Standard;
	while(Humidity_Standard_Obj != 0 || j != 0){
		Show_Num(88 + 8*i, 2, (char *)F8X16, (Humidity_Standard_Obj / j + 16));
		i++;
		Humidity_Standard_Obj = Humidity_Standard_Obj % j;
		j /= 10;
	}
	
		//显示箭头
	if(Count_Pro == 3){
			OLED_ClearXY(104, 128, 0);
			OLED_ClearXY(104, 128, 1);
			Show_Zh(88 + 8*i , 2, config, 8);
	}else{
		Count_Pro = 2;
	}
	
}

/*
 * 功  能：修改或初始化AT24C02
 * 参  数：
 *				首次进入：choose == 1
 *				修改参数：choose == 0
 * 返回值：无
 */
void Modify_TH(uint8_t choose)
{
	uint8_t pBuf[10] = {0};
	uint8_t fBuf[20];
	if(choose == 0){
			pBuf[0] = 'F';
			pBuf[1] = Temperature_Standard / 100 + '0';
			pBuf[2] = Temperature_Standard % 100 / 10 + '0';
			pBuf[3] = Temperature_Standard % 10 + '0';
			pBuf[4] = Humidity_Standard / 100 + '0';
			pBuf[5] = Humidity_Standard % 100 / 10 + '0';
			pBuf[6] = Humidity_Standard % 10 + '0';
			AT24C02_PageWriteData(0 ,pBuf,strlen((char *)pBuf));
	}else if(choose == 1){
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
			Modify_TH(0);
		}
	}
}










