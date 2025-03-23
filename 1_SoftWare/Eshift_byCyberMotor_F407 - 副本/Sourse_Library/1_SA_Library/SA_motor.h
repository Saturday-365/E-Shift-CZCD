#ifndef _SA_MOTOR_H
#define _SA_MOTOR_H
#include "main.h" 
#define func_limit_ab(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
void Motor_init(void);
void Motor_run(uint16_t speed,uint8_t dir);

//void testdata(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3,float data1);

#endif



