#ifndef __CZCD_CANTRAIN_H
#define __CZCD_CANTRAIN_H
#include "headfile.h"

void CZCD_CAN_Init(void);
void CZCD_CAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data); //can发送数据 
uint8_t CZCD_CAN_ReceiveFlag(void);   //can接收标志位
void CZCD_CAN_Receive(uint8_t *Data); //can接收数据
void CAN_INIT(void);
void CZCD_CAN_Runing(void);

void CAN_NVIC_Configuration(void);
void CZCD_CANData_tran(void);
void CZCD_CANData_Send(void);

#endif
