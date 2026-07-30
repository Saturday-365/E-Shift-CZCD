#ifndef __PCCOM_H
#define __PCCOM_H
 
#include "stm32f4xx_hal.h"  // 根据你的STM32系列调整，如stm32f1xx_hal.h
#include "dma.h"            // 包含 DMA 初始化和配置的头文件
#include <string.h>         // 用于 strlen 函数
#include <stdarg.h>         // 用于可变参数函数
#include <stdio.h>
#include "usart.h"
 
#ifdef __cplusplus
extern "C" {
#endif
    
void PC_Communication(UART_HandleTypeDef* huart, const char* format, ...);
void curve_Display(float a, float b, float c, float d);
    
#ifdef __cplusplus
}
#endif
 
#endif
/////////
///**
//  * @brief          hal库TIM回调函数,发送数据到电脑
//  * @param[in]      hcan:TIM句柄指针
//  * @retval         none
//  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM2) // 检查是哪个定时器产生了中断
//    {
//        motor_info[0] = Cyber.des_pos;
//        motor_info[1] = Cyber.pre_pos;
//        motor_info[2] = Cyber.pre_vel;
//        motor_info[3] = Cyber.pre_tor;
//        //motor_info[4] = Cyber.pre_temperature;
//        //printf("Desired Position: %f\n", motor_info[0]);
//       //printf("Present Position: %f\n", motor_info[1]);
//        //printf("Present Velocity: %f\n", motor_info[2]);
//        //printf("Present Current: %f\n", motor_info[3]);
//        curve_Display(motor_info[0],motor_info[1],motor_info[2],motor_info[3]);
//    }
//}
