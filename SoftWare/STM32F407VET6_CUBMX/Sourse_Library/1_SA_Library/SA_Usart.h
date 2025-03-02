#ifndef __SA_USART_H
#define __SA_USART_H
#include "main.h"
 #define LENGTH 10     //接受缓冲区大小
void ATK_Uart_Init(void);
void VOFA_Uart_Init(void);
uint8_t Report_stage(void);
void Vofa_PID_Adjust(void);
void JustFloat_4(float data1,float data2,float data3,float data4);
void JustFloat_8(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8);

#endif
