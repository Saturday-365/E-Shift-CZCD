
#ifndef __MT6701_H
#define __MT6701_H
 
#include "main.h"
#include "SA_Delay.h"
#include "stm32f1xx_hal.h"
//#include "i2c.h"
#include <stdio.h>

#define MT6701_SLAVE_ADDR         0x06 << 1
#define MT6701_Timeout            50

#define MT6701_REG_ANGLE_14b      0x03    // 14Bit角度信息，存储在0x03[13:6]、0x04[5:0]两个寄存器中，高位在前，原始读数0~16383

#define mt6701_log		printf

float i2c_mt6701_get_angle1(void);
float i2c_mt6701_get_angle2(void);


//softiic
/*=========================地址信息=========================*/
#define Low_Address   	    0X0c        //00001100默认低地址
#define	High_Address		0X8c		//10001100可调高地址
#define ReadAddress1        0X03        //数据高位寄存器地址
#define ReadAddress2        0X04        //数据低位寄存器地址
 
double MT6701_RecvData(GPIO_TypeDef *sda_GPIO, uint16_t sda_Pin, GPIO_TypeDef *scl_GPIO, uint16_t scl_Pin, uint8_t Number);
/*-----------------函数内部需要的支持函数------------------*/
void MT6701_Start(void);						//开始信号
void MT6701_Stop(void);							//结束信号
void GPIO_Write(void);							//设置SDA为输出模式
void GPIO_Read(void);							//设置SDA为输入模式
void MT6701_SendACK(int ack);				    //发送ACK信号
int MT6701_RecvACK(void);						//读取ACK信号
uint8_t MT6701_RecvByte(void);			        //读取一个字节的数据
void MT6701_SendByte(uint8_t dat);	            //写入一个字节的数据
//softiic 
 
#endif


//————————————————

//                            版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
//                        
//原文链接：https://blog.csdn.net/Mark_md/article/details/126910658
//#define My_I2C_SDA_GPIO_Port            GPIOB
//#define My_I2C_SDA_Pin                  GPIO_PIN_9
//#define My_I2C_SCL_GPIO_Port            GPIOB
//#define My_I2C_SCL_Pin                  GPIO_PIN_8

///*=========================地址信息=========================*/
//#define Low_Address   	    0X0c        //00001100默认低地址
//#define	High_Address		0X8c		//10001100可调高地址
//#define ReadAddress1        0X03        //数据高位寄存器地址
//#define ReadAddress2        0X04        //数据低位寄存器地址
// 
//double MT6701_RecvData(GPIO_TypeDef *sda_GPIO, uint16_t sda_Pin, GPIO_TypeDef *scl_GPIO, uint16_t scl_Pin, uint8_t Number);
///*-----------------函数内部需要的支持函数------------------*/
//void MT6701_Start(void);						//开始信号
//void MT6701_Stop(void);							//结束信号
//void GPIO_Write(void);							//设置SDA为输出模式
//void GPIO_Read(void);							//设置SDA为输入模式
//void MT6701_SendACK(int ack);				    //发送ACK信号
//int MT6701_RecvACK(void);						//读取ACK信号
//uint8_t MT6701_RecvByte(void);			        //读取一个字节的数据
//void MT6701_SendByte(uint8_t dat);	            //写入一个字节的数据
// 
// 
//#endif
