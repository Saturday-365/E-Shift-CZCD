#ifndef _XIAOMI_DRIVER_H_
#define _XIAOMI_DRIVER_H_
#include "main.h"
 
typedef enum {
    RunMode_idx = 0x7005,
    IqRef_idx = 0x7006,
    SpdRef_idx = 0x700A,
    LimitTorque_idx = 0x700B,
    CurKp_idx = 0x7010,
    CurKi_idx = 0x7011,
    CurFiltGain_idx = 0x7014,
    LocRef_idx = 0x7016,
    LimitSpd_idx = 0x7017,
    LimitCur_idx = 0x7018,
    MechPos_idx = 0x7019,
    IqFilt_idx = 0x701A,
    MechVel_idx = 0x701B,
    Vbus_idx = 0x701C,
    Rotation_idx = 0x701D,
    LocKp_idx = 0x701E,
    SpdKp_idx = 0x7020,
    SpdKi_idx = 0x7020
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
#define Communication_Type_GetSingleParameter 0x11	//读取单个参数
#define Communication_Type_SetSingleParameter 0x12	//设定单个参数
#define Communication_Type_ErrorFeedback 0x15
 
enum CONTROL_MODE   //控制模式定义
{
    Motion_mode = 0,//运控模式
    Position_mode,  //位置模式
    Velcity_mode,     //位置模式
    Current_mode    //电流模式
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

