#ifndef __DIO_H
#define __DIO_H 

//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/*
GPIOx 			GPIOA ... GPIOE
GPIO_Pin		GPIO_Pin_0 ... GPIO_Pin_15
GPIO_Mode 	GPIO_Mode_AIN,GPIO_Mode_IN_FLOATING,GPIO_Mode_IPD,GPIO_Mode_IPU,GPIO_Mode_Out_OD,
						GPIO_Mode_Out_PP,GPIO_Mode_AF_OD,GPIO_Mode_AF_PP
GPIO_Speed 	  GPIO_Speed_10MHz,GPIO_Speed_2MHz,GPIO_Speed_50MHz

e.g. DIO_Initial(GPIOA, GPIO_Pin_0,GPIO_Mode_Out_PP,GPIO_Speed_10MHz);
*/

struct SPin
{
	GPIO_TypeDef* GPIOx;
	unsigned long GPIO_Pin;
	GPIOMode_TypeDef GPIO_Mode;
	GPIOSpeed_TypeDef GPIO_Speed;
};

void DIO_Initial(struct SPin stcPin);
unsigned char ReadDI(struct SPin stcPin);
void WriteDO(struct SPin stcPin,unsigned char ucData);
unsigned char ReadDI(struct SPin stcPin);
void IOSleep(void);
void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_REVERSE(void);
void LED_STATE(unsigned char State);
#endif



