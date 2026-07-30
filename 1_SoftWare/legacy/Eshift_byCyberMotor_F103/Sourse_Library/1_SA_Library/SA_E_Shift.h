#ifndef _SA_E_SHIFT_H
#define _SA_E_SHIFT_H
#include "main.h" 
#define func_limit_ab(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

void Motor_init(void);
void Motor_run(uint16_t speed,uint8_t dir);
void UPSHIFT_flag(uint8_t flag1);
void DOWNSHIFT_flag(uint8_t flag2);

//void testdata(FOCmotor_Typedef*foc1,FOCmotor_Typedef*foc2,FOCmotor_Typedef*foc3,float data1);

#endif



