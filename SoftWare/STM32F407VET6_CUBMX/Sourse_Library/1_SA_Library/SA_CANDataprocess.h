#ifndef __SA_CANDATAPROCESS_H
#define __SA_CANDATAPROCESS_H

#include "main.h"
#include "SA_usart.h"
typedef struct{           //小米电机结构体
    float RPM;
    float MAP;
    float TPS;
    float CLT;
    float IAT;
    float ECUvlot;
    float GEAR;
    float LAMDA1;
    float OilPressure;
    float APPS;
    float IgnitionTiming;
}Data_Radio;

extern Data_Radio ECUDATA;

void CZCD_CANData_tran(Data_Radio *DATA,uint8_t CANRxData[8]);
void Init_DATA_CAN(void);


 

#endif
