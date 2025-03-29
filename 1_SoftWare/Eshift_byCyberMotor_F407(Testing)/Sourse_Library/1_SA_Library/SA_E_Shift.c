#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SA_E_Shift.h"
#include "Cybergear_Control.h"
#include "SA_CANDataprocess.h"
#include "SA_Usart.h"
//**********************定义可调参数***************************//
#define Clutch_pos_up  0    //升档离合角度
#define Clutch_pos_down -75 //降档离合角度
#define errtime 50     //换挡超时判断时间 单位 /20ms
#define radio_mode 1  //定义数据传输模式 0为无线串口传输  1为数传电台传输

#define Clutch_speed 100    //离合电机最大速度
#define Clutch_tor 11.9     //离合电机最大扭矩
#define Shift_speed 100     //换挡电机最大速度   
#define Shift_tor 11.9      //换挡电机最大扭矩




//**********************定义可调参数***************************//

//**********************定义换挡变量***************************//
uint8_t Eshift_flag_UP=0,Eshift_flag_DOWM=0; //状态机flag
float Shift_pos_UP,Shift_pos_DOWM;  //定义升降档换挡电机输入角度参数
//**********************定义换挡变量***************************//

//*********************主  要  函  数**************************//
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

/**
  * @brief          电机上电初始化
  * @param[in]      none
  * @retval         none
  */
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

/**
  * @brief          定义一个挡位data 避免档位传感器数据出现问题影响电机操作
  * @param[in]      real GEAR
  * @retval         0-6 GEAR
  */
uint16_t last_gear=0;
//uint16_t Gear_data(Data_Radio *DATA){
//    if (DATA->GEAR>=0&&DATA->GEAR<=6){
//        if (DATA->GEAR==0) {last_gear=0; return 0;}
//        else if (DATA->GEAR==1) {last_gear=1; return 1;}
//        else if (DATA->GEAR==2) {last_gear=2; return 2;}
//        else if (DATA->GEAR==3) {last_gear=3; return 3;}
//        else if (DATA->GEAR==4) {last_gear=4; return 4;}
//        else if (DATA->GEAR==5) {last_gear=5; return 5;}
//        else if (DATA->GEAR==6) {last_gear=6; return 6;}
//    }
//    else return last_gear;
//}

uint16_t Gear_data(uint16_t GEAR){ //基本档传数据处理
    if (GEAR<=6){
        if (GEAR==0 && last_gear!=3 && last_gear!=4&&last_gear!=5) {last_gear=0; return 0;}
        else if (GEAR==1) {last_gear=1; return 1;}
        else if (GEAR==2) {last_gear=2; return 2;}
        else if (GEAR==3) {last_gear=3; return 3;}
        else if (GEAR==4) {last_gear=4; return 4;}
        else if (GEAR==5) {last_gear=5; return 5;}
        else {last_gear=6; return 6;}
    }
    else return last_gear;
}

uint16_t stabilize_gear(uint16_t GearDataIN) {
    static int16_t current_gear = -1;        // 当前稳定档位
    static int16_t candidate_gear = -1;      // 候选档位
    static uint16_t counter = 0;     // 连续采样计数器
    const uint16_t threshold = 3;    // 连续采样阈值（需根据实际情况调整）
    const int16_t valid_gears[] = {0, 1, 2, 3, 4, 5, 6};
    const int16_t num_gears = sizeof(valid_gears)/sizeof(valid_gears[0]);

    // 验证输入是否为有效档位
    int16_t valid = 0;
    for (int i = 0; i < num_gears; i++) {
        if (GearDataIN == valid_gears[i]) {
            valid = 1;
            break;
        }
    }
    if (!valid) return current_gear;  // 无效输入保持当前档位

    // 初始化处理
    if (current_gear == -1) {
        current_gear = GearDataIN;
        return current_gear;
    }

    // 相同输入重置计数器
    if (GearDataIN == current_gear) {
        candidate_gear = -1;
        counter = 0;
        return current_gear;
    }

    // 检查是否为合法相邻档位
    int gear_diff = abs(GearDataIN - current_gear);
    if (gear_diff == 1) {  // 仅允许相邻档位变化
        if (GearDataIN == candidate_gear) {
            if (++counter >= threshold) {
                current_gear = GearDataIN;   // 更新稳定档位
                candidate_gear = -1;
                counter = 0;
            }
        } 
        else {
            candidate_gear = GearDataIN;     // 设置新的候选档位
            counter = 1;
        }
    } 
    else {  // 非法跳变
        candidate_gear = -1;
        counter = 0;
    }

    return current_gear;
}


/**
  * @brief          超时判断 
                    overtime_tick在20ms定时器中断中调用进行自增
                    errtime在本文件上方 define
  * @param[in]      none
  * @retval         bool 0 1
  */
void Set_Start_ottick(){
    overtime_tick=100;
}
uint8_t judge_ottick(){
    if (overtime_tick==100+errtime) return 1;
    else return 0;
}
//*********************升降档规则表定义*********************//
float Shift_pos_list[2][6]=
    {
        {  37  , -45 , -54 , -38 , -38 , -38 },//0 降档
       //37// 1->0  2->1  3->2  4->3  5->4  6->5     
        { -47 ,  58 ,  52 ,  49 ,  41 ,  47 },//1 升档
//        { -47,  58 ,  41 ,  50 ,  40 ,  48 },//1 升档
        // 0->1  1->2  2->3  3->4  4->5  5->6
    };//升降档规则表 Pos
    
    /**
  * @brief          判断挡位是否
  * @param[in]      目标挡位 实时挡位
  * @retval         bool 0 1
  */
uint16_t Real_Gear;
//uint16_t Gear_ready(uint16_t aim_GEAR,Data_Radio *DATA,Cyber_Motor *Motor){
//    //Real_Gear=stabilize_gear(DATA->GEAR);
//    if (aim_GEAR==0 && Motor->pre_pos>=32) return 1;
//    else if (aim_GEAR!=0 && aim_GEAR==Real_Gear) return 1;
//    else return 0;
//}

uint16_t Gear_ready(uint16_t aim_GEAR,Data_Radio *DATA,Cyber_Motor *Motor){
    //Real_Gear=stabilize_gear(DATA->GEAR);
    if (aim_GEAR==0 && Motor->pre_pos>=32) return 1;
    else if (aim_GEAR!=0 && aim_GEAR==DATA->RealGEAR) return 1;
    else return 0;
}
/**
  * @brief          升档规则表查表 
  * @param[in]      upordowm 升档还是降档 目前挡位
  * @retval         升档规则表数据
  */
float GET_Shift_pos(uint8_t upordown,uint16_t Gear){
    if (upordown==1) return Shift_pos_list[1][Gear];
    else if (upordown==0) return Shift_pos_list[0][Gear-1];
    else return 0;
}

/**
  * @brief          换挡动作 
  * @param[in]      upordowm 升档还是降档 ecu数据结构体
  * @retval         none
  */
uint16_t aim_gear; //定义目标档位

void EShift_move(uint8_t upordown,Data_Radio *DATA)
    {   //while写的状态机，后续可以考虑使用更加清晰的状态机编写
    //升档动作开始
    if (upordown==1&&DATA->GEAR<=5){    
        Eshift_flag_UP=1;
        //Real_Gear=stabilize_gear(DATA->GEAR);
        Real_Gear=DATA->RealGEAR;
        aim_gear=Real_Gear+1;//设置目标档位
        Shift_pos_UP=GET_Shift_pos(1,Real_Gear); // 根据档位得到特定角度回传给电机              
        Set_Start_ottick();
        while(Eshift_flag_UP){
            Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode);//电台发送数据             
            Set_Cyber_Pos(&Clutch_Cyber,Clutch_pos_up);  //设定离合位置         
            UPSHIFT_flag(1);                                //升档断火信号发送
            while(pre_pos_ready(&Clutch_Cyber,Clutch_pos_up,0)){//等待离合拉到 指定位置-提前量              
                Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode); //电台发送数据                   
                Set_Cyber_Pos(&Shift_Cyber,Shift_pos_UP);  //传递电机信号
                while(Gear_ready(aim_gear,&ECUDATA,&Shift_Cyber) || judge_ottick()/*pre_pos_ready(&Shift_Cyber,Shift_pos_UP,1)*/){//等待挡位传感器回传数据-提前量                  
                    Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode);  //电台发送数据   
                    Set_Cyber_Pos(&Shift_Cyber,1);  //电机归位
                    UPSHIFT_flag(0);
                    Eshift_flag_UP=0;
                    break;
                }
                break;
            }       
        }
    }//升档动作if结束
    //降档动作开始
    if (upordown==0&&DATA->GEAR>=1){    
        Eshift_flag_DOWM=1;
        //Real_Gear=stabilize_gear(DATA->GEAR);
        Real_Gear=DATA->RealGEAR;
        aim_gear=Real_Gear-1;//设置目标档位
        Shift_pos_DOWM=GET_Shift_pos(0,Real_Gear); // 根据档位得到特定角度回传给电机              
        Set_Start_ottick();
        while(Eshift_flag_DOWM){
            Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode);//电台发送数据             
            Set_Cyber_Pos(&Clutch_Cyber,Clutch_pos_down);  //设定离合位置
            DOWNSHIFT_flag(1);                                //降档补油信号发送
            while(pre_pos_ready(&Clutch_Cyber,Clutch_pos_down,30)){//等待离合拉到 指定位置-提前量              
                Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode); //电台发送数据                   
                Set_Cyber_Pos(&Shift_Cyber,Shift_pos_DOWM);  //传递电机信号
                while(Gear_ready(aim_gear,&ECUDATA,&Shift_Cyber) || judge_ottick()){//等待挡位传感器回传数据-提前量                  
                    
                    Radio_Data_Send(&Clutch_Cyber,&Shift_Cyber,&ECUDATA,Real_Gear,radio_mode);  //电台发送数据   
                    Set_Cyber_Pos(&Shift_Cyber,1);  //电机归位
                    DOWNSHIFT_flag(0);
                    Eshift_flag_DOWM=0;
                    break;
                }
                break;
            }
        }
    }//升档动作if结束

}

void Radio_Data_Send(Cyber_Motor *Motor1,Cyber_Motor *Motor2,Data_Radio *DATA,uint16_t Gear,uint8_t mode)
{
//    if(mode==1)
//            JustFloat_10_rs232(Motor1->pre_pos,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
//                       Motor2->pre_pos,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
//                       DATA->GEAR,DATA->RPM);
//    else    JustFloat_10(Motor1->pre_pos,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
//                       Motor2->pre_pos,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
//                       DATA->GEAR,DATA->RPM);
    if(mode==1)
            JustFloat_10_rs232(Motor1->pre_pos,Motor1->pre_temperature,
                       Motor2->pre_pos,Motor2->pre_temperature,
                       DATA->GEAR,DATA->RealGEAR,Gear,DATA->RPM,DATA->APPS,DATA->LAMDA1);
    else    JustFloat_10(Motor1->pre_pos,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
                       Motor2->pre_pos,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
                       DATA->GEAR,DATA->RPM);
    //    if(mode==1)           //发送16个数据vofa出现卡顿故先使用10个数据进行调试电机
//            JustFloat_16_rs232(Motor1->pre_pos,Motor1->pre_vel,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
//                       Motor2->pre_pos,Motor2->pre_vel,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
//                       DATA->GEAR,DATA->RPM,DATA->CLT,DATA->TPS,DATA->ECUvlot,DATA->LAMDA1);
//    else    JustFloat_16(Motor1->pre_pos,Motor1->pre_vel,Motor1->pre_tor,Motor1->pre_temperature,Motor1->error_code,
//                       Motor2->pre_pos,Motor2->pre_vel,Motor2->pre_tor,Motor2->pre_temperature,Motor2->error_code, 
//                       DATA->GEAR,DATA->RPM,DATA->CLT,DATA->TPS,DATA->ECUvlot,DATA->LAMDA1);

}
