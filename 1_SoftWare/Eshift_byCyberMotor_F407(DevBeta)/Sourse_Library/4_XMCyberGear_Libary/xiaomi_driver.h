#ifndef _XIAOMI_DRIVER_H_
#define _XIAOMI_DRIVER_H_
#include "main.h"
 
typedef enum {
    RunMode_idx = 0x7005,       // 电机模式
    IqRef_idx = 0x7006,         // 电流模式IQ指令
    SpdRef_idx = 0x700A,        // 转速模式转速指令
    LimitTorque_idx = 0x700B,   // 转矩限制
    CurKp_idx = 0x7010,         // 
    CurKi_idx = 0x7011,         //
    CurFiltGain_idx = 0x7014,   // 电流滤波系数
    LocRef_idx = 0x7016,        // 位置模式指令角度
    LimitSpd_idx = 0x7017,      // 位置模式速度限制
    LimitCur_idx = 0x7018,      // 位置模式电流限制
    MechPos_idx = 0x7019,       // 负载端记圈机械角度
    IqFilt_idx = 0x701A,        // iq滤波值
    MechVel_idx = 0x701B,       // 负载端转速
    Vbus_idx = 0x701C,          // 母线电压
    Rotation_idx = 0x701D,      // 圈数
    LocKp_idx = 0x701E,         // 位置kp
    SpdKp_idx = 0x7020,         // 速度kp
    SpdKi_idx = 0x7020          // 速度ki
} ControlTable;
 
typedef enum {
    RunMode_Typ = 's', 
    IqRef_Typ = 'f',
    SpdRef_Typ = 'f',
    LimitTorque_Typ = 'f',
    CurKp_Typ = 'f',
    CurKi_Typ = 'f',
    CurFiltGain_Typ = 'f',
    LocRef_Typ = 'f',
    LimitSpd_Typ = 'f',
    LimitCur_Typ = 'f',
    MechPos_Typ = 'f',
    IqFilt_Typ = 'f',
    MechVel_Typ = 'f',
    Vbus_Typ = 'f',
    Rotation_Typ = 's',
    LocKp_Typ = 'f',
    SpdKp_Typ = 'f',
    SpdKi_Typ = 'f'
} ControlTableType;
 
//控制参数最值，谨慎更改
#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -12.0f
#define T_MAX 12.0f
#define MAX_P 720
#define MIN_P -720
 
//控制命令宏定义
#define Communication_Type_GetID 0x00           //获取设备的ID和64位MCU唯一标识符
#define Communication_Type_MotionControl 0x01 	//用来向主机发送控制指令
#define Communication_Type_MotorRequest 0x02	//用来向主机反馈电机运行状态
#define Communication_Type_MotorEnable 0x03	    //电机使能运行
#define Communication_Type_MotorStop 0x04	    //电机停止运行
#define Communication_Type_SetPosZero 0x06	    //设置电机机械零位
#define Communication_Type_CanID 0x07	        //更改当前电机CAN_ID
#define Communication_Type_Control_Mode 0x12
#define Communication_Type_GetSingleParameter 0x11	//读取单个参数 原来是11  十六进制11对应十进制17
#define Communication_Type_SetSingleParameter 0x12	//设定单个参数  十六进制12对应 十进制18
#define Communication_Type_ErrorFeedback 0x15
 
enum CONTROL_MODE   //控制模式定义
{
    Motion_mode = 0,//运控模式
    Position_mode ,  //位置模式
    Velcity_mode ,     //位置模式
    Current_mode     //电流模式
};
enum ERROR_TAG      //错误回传对照
{
    OK                 = 0,//无故障
    BAT_LOW_ERR        = 1,//欠压故障
    OVER_CURRENT_ERR   = 2,//过流
    OVER_TEMP_ERR      = 3,//过温
    MAGNETIC_ERR       = 4,//磁编码故障
    HALL_ERR_ERR       = 5,//HALL编码故障
    NO_CALIBRATION_ERR = 6//未标定
};
 
#endif

