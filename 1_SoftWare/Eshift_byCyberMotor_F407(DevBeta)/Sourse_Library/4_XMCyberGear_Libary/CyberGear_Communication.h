//#ifndef __CYBERGEAR_H
//#define __CYBERGEAR_H
// 
//#include "main.h"
//#include "can.h"
//#include "xiaomi_driver.h"
//#include "pc_communication.h"
// #include <math.h>

//#ifdef __cplusplus
//extern "C" {
//#endif
//    
//extern volatile float motor_info[4];
// 
////主机CANID设置
//#define Master_CAN_ID 0x04                      //主机ID
// 
////参数读取宏定义
// 
//#define Gain_Angle 720/32767.0
//#define Bias_Angle 0x8000
//#define Gain_Speed 30/32767.0
//#define Bias_Speed 0x8000
//#define Gain_Torque 12/32767.0
//#define Bias_Torque 0x8000
//#define Temp_Gain   0.1
// 
//#define Motor_Error 0x00
//#define Motor_OK 0X01
// 
//typedef enum 
//{
//	Unit_Default,
//	Unit_Deg=1,
//	Unit_RPM=1,
//	Unit_mA=1
//}Unit;
// 
//typedef struct{
//    
//    volatile float freq;
//    volatile float amp;//角度值
//    volatile float delay;
//    volatile float offset;
//    volatile float downRatio;
//    
//} SinMotion;
// 
//typedef struct{           //小米电机结构体
//	uint8_t CAN_ID;       //CAN ID
//    uint8_t MCU_ID;       //MCU唯一标识符[后8位，共64位]
// 
//	float des_cur;
//	float des_vel;
//	float des_pos;
//    
//    float pre_cur;
//    float pre_vel;
//    float pre_pos;
//    float pre_tor;
//    float pre_temperature;
//    uint8_t error_code;
//    
//    //零位选择通过Init_CyberZero()设置
//    float ini_tor;
//    float ini_vel;
//    float ini_pos;
//    float ini_kp, sin_kp;
//    float ini_kd, sin_kd;
//    
//    //最大限制
//    float max_tor;
//    float max_pos;
//    float max_vel;
//    float tor_threshold;
//    
//	float zero_pos;
//    
//    SinMotion MotionPara;
//    
// 
//}Cyber_Motor;
// 
// 
//    
//extern Cyber_Motor Cyber;
///*****************************初始化*****************************/
//void Init_Cyber(Cyber_Motor *Motor, uint8_t Can_Id);
//void Start_Cyber(Cyber_Motor *Motor);
//void Stop_Cyber(Cyber_Motor *Motor, uint8_t clear_error);
// 
///*****************************设置电机参数*****************************/
//void Set_Cyber_Mode(Cyber_Motor *Motor, uint8_t Mode);
//void Set_Cyber_ZeroPos(Cyber_Motor *Motor);
//void Set_Cyber_Pos(Cyber_Motor *Motor, float value);//仅位置模式, value单位为Deg
//void Set_Cyber_limitSp(Cyber_Motor *Motor, float value);//同时设置电机结构体中的max_spd, value单位为RPM
//void Set_Cyber_limitTor(Cyber_Motor *Motor, float value);//同时设置电机结构体中的max_tor, value单位为Nm
//void Set_Cyber_RotNum(Cyber_Motor *Motor, float value);//设置电机当前圈数
//void Read_Cyber_Pos(Cyber_Motor *Motor);//获得反馈，发送位置可以时刻获得位置，这个呢
///*****************************运动控制模式*****************************/
//void Cyber_ControlMode(Cyber_Motor *Motor,float tor, float vel_rads, float pos_rad, float kp, float kd);//仅运控模式
// 
///*****************************反馈帧处理回调函数 负责接回传信息 *****************************/
//void request_motor_feedback(Cyber_Motor *Motor);
//void Motor_Data_Handler(Cyber_Motor *Motor,uint8_t DataFrame[8],uint32_t IDFrame);

//void CANtest(Cyber_Motor *Motor);

///*****************************暂时没用，电机参数读取设置*****************************/
//void Check_Cyber(uint8_t ID);
// 
//void set_CANID_cybergear(Cyber_Motor *Motor, uint8_t CAN_ID);
//void Set_Cyber_Cur(Cyber_Motor *Motor, float Current);
//uint32_t Get_Motor_ID(uint32_t CAN_ID_Frame);
// 
///*****************************电机协议*****************************/
//float uint16_to_float(uint16_t x,float x_min,float x_max,int bits);
//int float_to_uint(float x, float x_min, float x_max, int bits);
//void Set_Cyber_Parameter(Cyber_Motor *Motor,uint16_t Index,float Value,char Value_type);
//void Set_Cyber_Parameter(Cyber_Motor *Motor,uint16_t Index,float Value,char Value_type);
//void Read_Cyber_Parameter(Cyber_Motor *Motor,uint16_t Index);
// //该文件应该包括 绝对零点设置；正弦运动设置；



///*****************************获取期望零位*****************************/
//void Init_CyberZero(Cyber_Motor *Motor);
//void Init_Sin(Cyber_Motor *Motor);
// 
//void Setting_AbsoluteZero(Cyber_Motor *Motor);
//void Motion_CyberSin(Cyber_Motor *Motor);
//  
//#ifdef __cplusplus
//  }
//#endif
//  
//#endif

//  
