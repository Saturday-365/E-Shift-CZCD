#ifndef __SA_USART_H
#define __SA_USART_H
#include "main.h"
 #define LENGTH 10     //接受缓冲区大小
void ATK_Uart_Init(void);
void VOFA_Uart_Init(void);
uint8_t Report_stage(void);
void Vofa_PID_Adjust(void);

#endif
