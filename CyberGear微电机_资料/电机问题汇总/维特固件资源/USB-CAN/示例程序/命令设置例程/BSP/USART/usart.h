#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"  

#define USART1_REC_MAXLEN 200	//最大接收数据长度

void USART1_Init_Config(u32 bound);
void USART1_SendData(u8* buff, u16 len);
void ClearCmd(void);

#endif


