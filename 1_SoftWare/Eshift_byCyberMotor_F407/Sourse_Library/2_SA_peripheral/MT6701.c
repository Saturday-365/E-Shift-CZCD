#include "MT6701.h"
#include "stdio.h"
#include "SA_motor.h"
#define PI 3.14159265359f

//unsigned char mt6701_write_reg(unsigned char reg, unsigned char value)
//{
//	return HAL_I2C_Mem_Write(&hi2c2, MT6701_SLAVE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, MT6701_Timeout);
//}

//unsigned char mt6701_write_regs(unsigned char reg, unsigned char *value, unsigned char len)
//{
//	return HAL_I2C_Mem_Write(&hi2c2, MT6701_SLAVE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, value, len, MT6701_Timeout);
//}
//unsigned char mt6701_read_reg1(unsigned char reg, unsigned char* buf, unsigned short len)
//{
//	return HAL_I2C_Mem_Read(&hi2c1, MT6701_SLAVE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, MT6701_Timeout);
//}
//unsigned char mt6701_read_reg2(unsigned char reg, unsigned char* buf, unsigned short len)
//{
//	return HAL_I2C_Mem_Read(&hi2c2, MT6701_SLAVE_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buf, len, MT6701_Timeout);
//}

//void mt6701_delay(unsigned int ms)
//{
//	HAL_Delay(ms);
//}

//    uint8_t temp[2];
//// 14Bit角度信息，存储在0x03[13:6]、0x04[5:0]两个寄存器中，高位在前，原始读数0~16383，对应0-360°
//float i2c_mt6701_get_angle1(void)
//{
//    mt6701_read_reg1(MT6701_REG_ANGLE_14b, temp, 2); //读取的时候地址会自动加一
//    int16_t angle = ((int16_t)temp[0] << 6) | (temp[1] >> 2);
//    float angle_f = (float)angle * 360 / 16384;
//    float rad=angle_f*PI/180;
////    float rad=angle_f;
//    return rad;
//}
//float i2c_mt6701_get_angle2(void)
//{
//    mt6701_read_reg2(MT6701_REG_ANGLE_14b, temp, 2); //读取的时候地址会自动加一
//    int16_t angle = ((int16_t)temp[0] << 6) | (temp[1] >> 2);
//    float angle_f = (float)angle * 360 / 16384;
//    float rad=angle_f*PI/180;
////    float rad=angle;
//    return rad;
//}

/*=====================全局变量部分======================*/
	GPIO_InitTypeDef GPIO_InitStruct;	//修改GPIO用的结构体
	
/*------------记录初始化函数传来的两线管脚信息----------*/
	GPIO_TypeDef *SDA_GPIO = NULL;		//sda总线数据
	uint16_t SDA_Pin;					//sda管脚数据
	GPIO_TypeDef *SCL_GPIO = NULL;		//scl总线数据
	uint16_t SCL_Pin;					//scl管脚数据
	uint8_t Address = Low_Address;		//BH1750地址数据，默认为低电平
 
/*======================读取MT6701编码器数据====================*/
/*	参数列表：							            					*/
/*		GPIO_TypeDef *sda_GPIO		为SDA总线信息						*/
/*		uint16_t sda_Pin					为SDA管脚信息				*/
/*		GPIO_TypeDef *scl_GPIO		为SCL总线信息						*/
/*		uint16_t scl_Pin					为SCL管脚信息				*/
/*		uint8_t Number						为数据信息，0为弧度 1为角度*/
/*==============================================================*/
double MT6701_RecvData(GPIO_TypeDef *sda_GPIO, uint16_t sda_Pin, GPIO_TypeDef *scl_GPIO, uint16_t scl_Pin, uint8_t Number)
{
	uint8_t RecvData = 0;
	uint32_t MT6701_Data = 0;
/*---------------将数据存入到全局变量方便移植----------------*/
	SDA_GPIO = sda_GPIO;
	SDA_Pin = sda_Pin;
	SCL_GPIO = scl_GPIO;
	SCL_Pin = scl_Pin;
//	if(Number == 1)
//		Address = High_Address;
//	else if(Number == 0)
//		Address = Low_Address;
    Address = Low_Address;
/*------------------------开始读取数据------------------------*/
/*-----------------------第一次读取数据-----------------------*/
	MT6701_Start();								//启动信号
	MT6701_SendByte(Address);					//发送地址与写指令
	MT6701_SendByte(ReadAddress1);			    //发送指令读取0x03寄存器
	MT6701_Start();								//发送第二个启动信号开始读数
	MT6701_SendByte(Address | 1);				//发送地址与读命令
	RecvData = MT6701_RecvByte();				//读取0x03寄存器的数据
//	printf("data1=%d\n", RecvData);
	MT6701_Stop();								//停止信号
	MT6701_Data = RecvData;					    //将数据赋值
/*-----------------------第二次读取数据-----------------------*/
	MT6701_Start();								//启动信号
	MT6701_SendByte(Address);					//发送地址与写指令
	MT6701_SendByte(ReadAddress2);			    //发送指令读取0x04寄存器
	MT6701_Start();								//发送第二个启动信号开始读数
	MT6701_SendByte(Address | 1);				//发送地址与读命令
	RecvData = MT6701_RecvByte();				//读取0x04寄存器的数据
//	printf("data2%d\n", RecvData);
	MT6701_Stop();								//停止信号
	MT6701_Data = (MT6701_Data << 8) + RecvData;
	MT6701_Data >>= 2;							//取数据高14位
//	printf("data3%d\n", MT6701_Data);
    if(Number == 0)
        {return (MT6701_Data*360.0)/16384.0*PI/180;}
    else
        {return (MT6701_Data*360.0)/16384.0;}
}
 
/*========================功能函数：开始信号======================*/
/*开始信号为SCL高时SDA由高拉低至少250nm，SCL跟着拉低				 */
/*===============================================================*/
void MT6701_Start()
{
    HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_SET);       //拉高数据线
    HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);       //拉高时钟线
//在HAL库的初始阶段设置两个GPIO为输出模式高电平，这里为了保险再次设置；
		My_Delay_us(10);                
    HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin ,GPIO_PIN_RESET);    //产生下降沿
    My_Delay_us(10);                
    HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_RESET);     //拉低时钟线
		My_Delay_us(10);
}
 
 
/*=======================功能函数：停止信号======================*/
/*停止信号为SCL高时SDA由低拉高至少250nm的动作						*/
/*==============================================================*/
void MT6701_Stop()
{
    HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_RESET);     //拉低数据线
    HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);       //拉高时钟线
    My_Delay_us(10);
    HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin ,GPIO_PIN_SET);      //产生上升沿
    My_Delay_us(10);
}
 
/*==========================发送应答信号=========================*/
/*			参数列表:ack (0:ACK 1:NAK)							*/
/*==============================================================*/
void MT6701_SendACK(int ack)
{
	GPIO_Write();
    if(ack == 1)   										//写应答信号
			HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_SET); 
		else if(ack == 0)
			HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_RESET);
		else
			return;
			
    HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);     
    My_Delay_us(10);              
    HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_RESET);      
    My_Delay_us(10);               
}
 
/*=======================接收应答信号========================*/
/*		这里没有起到任何作用，没有ACK照样跑~~			   		*/
/*==========================================================*/
int MT6701_RecvACK()
{		
	int ACK = 0;
	GPIO_Read();
  HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);    //拉高时钟线
  My_Delay_us(10);               
	if(HAL_GPIO_ReadPin(SDA_GPIO, SDA_Pin) == 1 )		//读应答信号
        ACK = 1 ;  
    else
        ACK = 0 ;			
  HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_RESET);  //拉低时钟线
  My_Delay_us(10);
		GPIO_Write();
    return ACK;
}
 
/*=================向iic总线发送一个字节数据==================*/
/*					参数列表：dat——想要发送的数据			    */
/*===========================================================*/
void MT6701_SendByte(uint8_t dat)
{
    uint8_t i;
    for (i=0; i<8; i++)         //8位计数器
    {
			if( 0X80 & dat )
					HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_SET);
			else
					HAL_GPIO_WritePin(SDA_GPIO, SDA_Pin,GPIO_PIN_RESET);
			dat <<= 1;
			HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);               //拉高时钟线
			My_Delay_us(10);            
			HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_RESET);                //拉低时钟线
			My_Delay_us(10);           
    }
    MT6701_RecvACK();
}
 
/*===================从iic总线读取一个字节地址================*/
/*			返回值为读取到的一个字节数据						*/
/*===========================================================*/
uint8_t MT6701_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
	  uint8_t bit;
	
		GPIO_Read();
 
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_SET);               //拉高时钟线
        My_Delay_us(10);            
	   if( SET == HAL_GPIO_ReadPin(SDA_GPIO, SDA_Pin) )
             bit = 0X01;
       else
             bit = 0x00;  
        dat |= bit;             //读数据 
        HAL_GPIO_WritePin(SCL_GPIO, SCL_Pin,GPIO_PIN_RESET);             //拉低时钟线
        My_Delay_us(10);           
    }
		MT6701_SendACK(1);					//函数内部有GPIO_Write();
    return dat;
}
/*======================将SDA设置为输出模式====================*/
void GPIO_Write(void)
{
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO, &GPIO_InitStruct); 
}
 
/*======================将SDA设置为输入模式====================*/
void GPIO_Read(void)
{
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO, &GPIO_InitStruct);
}

