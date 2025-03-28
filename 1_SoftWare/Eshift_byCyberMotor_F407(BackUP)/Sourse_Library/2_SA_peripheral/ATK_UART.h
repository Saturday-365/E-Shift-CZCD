#ifndef __MPU_UART_H
#define __MPU_UART_H
#include "headfile.h"

typedef struct
{
    float roll;                                     
    float pitch;                                    
    float yaw;                                      
} atk_ms601m_attitude_data_t;



uint8_t IMU_uart_callback_BYTE(uint8_t uart_tran[8] );

void IMU_uart_callback_BIT(uint8_t uart_tran );

void Get_IMUuartData(atk_ms601m_attitude_data_t *attitude_dat);
void Get_IMUuartData_BYTE(atk_ms601m_attitude_data_t *attitude_dat,uint8_t uart_tran[8] );

void IMUuartData(atk_ms601m_attitude_data_t *attitude_tdat );
float Get_IMUData(atk_ms601m_attitude_data_t *attitude_tdat,unsigned char mod);

extern atk_ms601m_attitude_data_t IMUDATA;



#endif
