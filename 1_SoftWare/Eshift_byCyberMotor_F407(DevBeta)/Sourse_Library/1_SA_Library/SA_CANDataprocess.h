#ifndef __SA_CANDATAPROCESS_H
#define __SA_CANDATAPROCESS_H

#include "main.h"
#include "SA_usart.h"
typedef struct{           //小米电机结构体
    uint16_t RPM;
    uint16_t MAP;
    uint16_t TPS;
    uint16_t CLT;
    uint16_t IAT;
    uint16_t ECUvlot;
    uint16_t GEAR;
    uint16_t RealGEAR;
    uint16_t LAMDA1;
    uint16_t OilPressure;
    uint16_t APPS;
    uint16_t IgnitionTiming;
}Data_Radio;

extern Data_Radio ECUDATA;

void CZCD_CANData_tran(Data_Radio *DATA,uint8_t CANRxData[8]);
void Init_DATA_CAN(void);


 

#endif
