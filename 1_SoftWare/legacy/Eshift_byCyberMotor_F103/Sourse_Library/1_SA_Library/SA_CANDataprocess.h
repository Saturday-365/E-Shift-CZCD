#ifndef __SA_CANDATAPROCESS_H
#define __SA_CANDATAPROCESS_H

#include "main.h"
#include "SA_usart.h"
//void CZCD_CAN_Init(void);
//void CZCD_CAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data); //can发送数据 
//uint8_t CZCD_CAN_ReceiveFlag(void);   //can接收标志位
//void CZCD_CAN_Receive(uint8_t *Data); //can接收数据
//void CAN_INIT(void);
//void CZCD_CAN_Runing(void);

//void CAN_NVIC_Configuration(void);
void CZCD_CANData_tran(uint8_t CANRxData[8]);
//void CZCD_CANData_Send(void);
extern float RPM;
extern float MAP;
extern float TPS;
extern float CLT;
extern float IAT;
extern float ECUvlot;
extern float GEAR;
extern float LAMDA1;
extern float OilPressure;
extern float APPS;
extern float IgnitionTiming;
#endif
