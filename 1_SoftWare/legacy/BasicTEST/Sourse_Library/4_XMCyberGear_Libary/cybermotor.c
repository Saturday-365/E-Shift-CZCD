#include "cybermotor.h"

const float pii = 3.14159265358979323846f;
 
 
//@brief   绝对零点设置，包括电机初始化，运控模式达到极限值；
void Setting_AbsoluteZero(Cyber_Motor *Motor)
{
    Init_Cyber(Motor, 0x7F);
    //设置当前位置为0位
    Init_CyberZero(Motor);//初始化电机结构体，即运控模式的参数Cyber_ControlMode
    Set_Cyber_Mode(Motor,Position_mode);
    //Set_Cyber_RotNum(Motor, 0);
    Start_Cyber(Motor);//使能电机
    Set_Cyber_RotNum(Motor, 0);
    Set_Cyber_ZeroPos(Motor);
    Set_Cyber_limitSp(Motor,1);
    Set_Cyber_limitTor(Motor,0.2);
    HAL_Delay(100);
    Set_Cyber_Pos(Motor, 10);
    HAL_Delay(100);
    Set_Cyber_Pos(Motor, 20);
    HAL_Delay(100);
    Set_Cyber_Pos(Motor, 40);
    HAL_Delay(100);
    Set_Cyber_Pos(Motor, 130);
    HAL_Delay(100);
    
    
    
    while (1)
    {
        //Set_Cyber_Pos(Motor, 30);
        Set_Cyber_RotNum(Motor, 0);
        
        // 检查力矩和速度
        if (Motor->pre_vel < 0.01)
        {
            Set_Cyber_ZeroPos(Motor);
            Set_Cyber_limitSp(Motor,6);
            Set_Cyber_Pos(Motor, -60);
            HAL_Delay(3000);
            //Set_Cyber_ZeroPos(Motor);
            //break;
 
            Motion_CyberSin(Motor);
 
        }
    }
}
 
 
 
// 控制电机正弦运动的函数
void Motion_CyberSin(Cyber_Motor *Motor) 
{
    Init_Sin(Motor);
    uint32_t current_time;
    float position;
    //HAL_Delay(1000);
    
    float period = 1/Motor->MotionPara.freq;
    //float periodPositive = period * Motor->downRatio;
	//float periodNegative = period - periodPositive;
    
//    Init_Cyber(Motor, 0x7F);
//    Set_Cyber_ZeroPos(Motor);
    Set_Cyber_limitSp(Motor, 5);
 
    // 循环持续指定的时间
    while(1)
    {
        //float amp_rad = Motor->MotionPara.amp * pi/180;
        current_time = HAL_GetTick(); // 更新当前时间
        position = Motor->MotionPara.amp * sinf(2 * pii * Motor->MotionPara.freq * (current_time / 1000.0f))-60;
        Motor->des_pos = position / pii * 180;
 
        Set_Cyber_Pos(Motor, position);
 
        // 添加延时来控制更新频率
        HAL_Delay(3);
    }
}
 
 
/*****************************零位初始化*****************************/
void Init_CyberZero(Cyber_Motor *Motor)
{
    Motor->ini_tor=0.1;
    Motor->ini_vel=0;
    Motor->ini_pos=120;
    Motor->ini_kp=0.8;
    Motor->ini_kd=0.3;
    Motor->tor_threshold=0.15;
}
 
/*****************************实际驱动函数*****************************/
void Init_Sin(Cyber_Motor *Motor)
{
    Motor->sin_kp = 3;
    Motor->sin_kd = 0.5;
    Motor->MotionPara.amp=20;//角度值
    Motor->MotionPara.freq=3;
    Motor->MotionPara.delay=0;
    Motor->MotionPara.offset=0;
    Motor->MotionPara.downRatio=0.5;//上下扑动的duration相同
}


