#include "math.h"
#include "stdio.h"
#include "string.h"
#include "SA_E_Shift.h"
#include "Cybergear_Control.h"
#include "SA_CANDataprocess.h"
#include "SA_Usart.h"

#define Clutch_pos_up -75
#define Clutch_pos_down -75

//#define UPshift_pos 38
//#define DOWNshift_pos  -38
uint16_t Shift_pos_UP,Shift_pos_DOWM;

#define Clutch_speed 100
#define Clutch_tor 11.9
#define Shift_speed 100
#define Shift_tor 11.9

uint8_t Eshift_up_flag=0,Eshift_dw_flag=0;

/**
  * @brief          升档信号设置
  * @param[in]      高电平触发 
  * @retval         none
  */
void UPSHIFT_flag(uint8_t flag1){
    if(flag1==1) HAL_GPIO_WritePin((GPIO_TypeDef *)UPSHIFTSIG_OUT_GPIO_Port, (uint16_t)UPSHIFTSIG_OUT_Pin, (GPIO_PinState)1);
    else HAL_GPIO_WritePin((GPIO_TypeDef *)UPSHIFTSIG_OUT_GPIO_Port, (uint16_t)UPSHIFTSIG_OUT_Pin, (GPIO_PinState)0);
}

/**
  * @brief          降档信号设置
  * @param[in]      高电平触发 
  * @retval         none
  */
void DOWNSHIFT_flag(uint8_t flag2){
    if(flag2==1) HAL_GPIO_WritePin((GPIO_TypeDef *)DOWNSHIFTSIG_OUT_GPIO_Port, (uint16_t)DOWNSHIFTSIG_OUT_Pin, (GPIO_PinState)1);
    else HAL_GPIO_WritePin((GPIO_TypeDef *)DOWNSHIFTSIG_OUT_GPIO_Port, (uint16_t)DOWNSHIFTSIG_OUT_Pin, (GPIO_PinState)0);
}


void Motor_Init()
{
    Init_Cyber(&Clutch_Cyber, 0x02);   //初始化电机参数
    Init_Cyber(&Shift_Cyber, 0x01);
    Init_MOTO_CAN();    //初始化can滤波器
    Init_DATA_CAN();
    HAL_Delay(20);
    Stop_Cyber(&Clutch_Cyber, 1);
    HAL_Delay(20);
    Stop_Cyber(&Shift_Cyber, 1);     //停止电机
    HAL_Delay(20);
    Set_Cyber_Mode(&Clutch_Cyber,1);    //设置电机模式
    HAL_Delay(20);
    Set_Cyber_Mode(&Shift_Cyber,1);
    HAL_Delay(20); 
    Set_Cyber_ZeroPos(&Clutch_Cyber);
    HAL_Delay(20);
    Set_Cyber_ZeroPos(&Shift_Cyber);    //设置当前位置为0位
    HAL_Delay(20);
    Start_Cyber(&Shift_Cyber);     
    HAL_Delay(20);
    Start_Cyber(&Clutch_Cyber);
    HAL_Delay(20);     
    Set_Cyber_Pos(&Clutch_Cyber,0) ;   //设置电机位置
    HAL_Delay(20);
    Set_Cyber_Pos(&Shift_Cyber,0) ;
    HAL_Delay(20);
    Set_Cyber_limitSp(&Clutch_Cyber,Clutch_speed) ;
    HAL_Delay(20);
    Set_Cyber_limitSp(&Shift_Cyber,Shift_speed) ;   
    Set_Cyber_limitTor(&Clutch_Cyber,Clutch_tor) ;
    HAL_Delay(20);
    Set_Cyber_limitTor(&Shift_Cyber,Shift_tor) ;
}

uint16_t last_gear;
uint16_t Gear_data(Data_Radio *DATA){
    if (DATA->GEAR>=0&&DATA->GEAR<=6){
        if (DATA->GEAR==0) {last_gear=0; return 0;}
        else if (DATA->GEAR==1) {last_gear=1; return 1;}
        else if (DATA->GEAR==2) {last_gear=2; return 2;}
        else if (DATA->GEAR==3) {last_gear=3; return 3;}
        else if (DATA->GEAR==4) {last_gear=4; return 4;}
        else if (DATA->GEAR==5) {last_gear=5; return 5;}
        else if (DATA->GEAR==6) {last_gear=6; return 6;}
    }
    else return last_gear;
}

void EShift_move(uint8_t upordown,Data_Radio *DATA)
{          
    Eshift_up_flag=1;
  while(Eshift_up_flag)
    {
        Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,0);             
        Set_Cyber_Pos(&Clutch_Cyber,Clutch_pos_up);
        UPSHIFT_flag(1);
        while(pre_pos_ready(&Clutch_Cyber,Clutch_pos_up,30))//等待离合拉到指定位置-提前量  
        {
            Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,0);     
            Shift_pos_DOWM;
            Set_Cyber_Pos(&Shift_Cyber,UPshift_pos);
                while(pre_pos_ready(&Shift_Cyber,UPshift_pos,0))//等待离合拉到指定位置-提前量  
            {
                Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,0);     
                Set_Cyber_Pos(&Shift_Cyber,0);
                UPSHIFT_flag(0);
                Eshift_up_flag=0;
                break;
            }
            break;
        }
    }

}

void Radio_Data_Send(Cyber_Motor *Motor1,Cyber_Motor *Motor2,Data_Radio *DATA,uint8_t mode)
{
    if(mode==1)
            JustFloat_16_rs232(Motor1->pre_pos,Motor1->pre_vel,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
                       Motor2->pre_pos,Motor2->pre_vel,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
                       DATA->GEAR,DATA->RPM,DATA->CLT,DATA->TPS,DATA->ECUvlot,DATA->LAMDA1);
    else    JustFloat_16(Motor1->pre_pos,Motor1->pre_vel,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
                       Motor2->pre_pos,Motor2->pre_vel,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
                       DATA->GEAR,DATA->RPM,DATA->CLT,DATA->TPS,DATA->ECUvlot,DATA->LAMDA1);


}
