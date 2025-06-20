#include "math.h"
#include "stdio.h"
#include "string.h"
#include "SA_Delay.h"
#include "tim.h"
#include "SA_motor.h"
/**
  * @brief          电机控制pwm初始化
  */
void Motor_init(){
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

/**
  * @brief          控制电机运动
  * @param[in]      speed 电机占空比 0-10000 dir 电机方向
  * @retval         none
  */
void Motor_run(uint16_t speed,uint8_t dir)
{
    if (dir==1)
        {
        HAL_GPIO_WritePin((GPIO_TypeDef *)MOTOR_PH_GPIO_Port, (uint16_t)MOTOR_PH_Pin, (GPIO_PinState)1);
        }
    else if (dir==0)
        {
        HAL_GPIO_WritePin((GPIO_TypeDef *)MOTOR_PH_GPIO_Port, (uint16_t)MOTOR_PH_Pin, (GPIO_PinState)0);
        }
    __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, speed); //设置CH4->PWM脉冲宽度    
}









