#ifndef _SA_E_SHIFT_H
#define _SA_E_SHIFT_H
#include "main.h"
#include "SA_CANDataprocess.h"
#include "Cybergear_Control.h"

#define func_limit_ab(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

void Motor_Init(void);
void EShift_move(uint8_t upordown,Data_Radio *DATA);
//uint16_t Gear_data(Data_Radio *DATA);
uint16_t Gear_data(uint16_t GEAR);
uint16_t Gear_ready(uint16_t aim_GEAR,Data_Radio *DATA,Cyber_Motor *Motor);
uint16_t stabilize_gear(uint16_t GearDataIN);

void UPSHIFT_flag(uint8_t flag1);
void DOWNSHIFT_flag(uint8_t flag2);

void Radio_Data_Send(Cyber_Motor *Motor1,Cyber_Motor *Motor2,Data_Radio *DATA,uint16_t Gear,uint8_t mode);
extern uint16_t Real_Gear;
//void testdata(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3,float data1);

#endif



