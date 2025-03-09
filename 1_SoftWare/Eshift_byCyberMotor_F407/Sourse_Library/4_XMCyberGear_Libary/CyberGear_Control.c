#include "main.h"
#include "can.h"
#include "Cybergear_Control.h"
#include "math.h"
#include "string.h"
#include "SA_Usart.h"
#include "SA_CANDataprocess.h"
Cyber_Motor Clutch_Cyber;//小米电机定义
Cyber_Motor Shift_Cyber;//小米电机定义

CAN_RxHeaderTypeDef rxMsg;//发送接收结构体
CAN_TxHeaderTypeDef txMsg;//发送配置结构体

uint8_t rx_data[8];       //接收数据
uint32_t Motor_Can_ID;    //接收数据电机ID
uint8_t byte[4];          //转换临时数据
uint32_t send_mail_box = {0};//NONE
 
#define can1_txd() HAL_CAN_AddTxMessage(&hcan1, &txMsg, tx_data, &send_mail_box)//CAN发送宏定义

const float pi = 3.14159265358979323846f;


CAN_FilterTypeDef can_Filter = {0};
                           
//原文链接：https://blog.csdn.net/m0_61973119/article/details/141369404


//@brief          小米电机初始化参数
void Init_Cyber(Cyber_Motor *Motor,uint8_t Can_Id)
{
    txMsg.StdId = 0;            //配置CAN发送：标准帧清零 
    txMsg.ExtId = 0;            //配置CAN发送：扩展帧清零     
    txMsg.IDE = CAN_ID_EXT;     //配置CAN发送：扩展帧
    txMsg.RTR = CAN_RTR_DATA;   //配置CAN发送：数据帧
    txMsg.DLC = 0x08;           //配置CAN发送：数据长度
    
    Motor->CAN_ID=Can_Id;       //ID设置    
}
//@brief          小米电机初始化参数
void Init_MOTO_CAN()
{
/* 配置CAN过滤器 */
	CAN_FilterTypeDef sFilterConfig1;
    sFilterConfig1.FilterBank = 1;                             /* 过滤器0 */
    sFilterConfig1.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig1.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig1.FilterIdHigh = 0x0000;                      /* 32位ID */
    sFilterConfig1.FilterIdLow = 0x0000;
    sFilterConfig1.FilterMaskIdHigh = 0x0000;                  /* 32位MASK */
    sFilterConfig1.FilterMaskIdLow = 0x0000;
    sFilterConfig1.FilterFIFOAssignment = CAN_RX_FIFO0; // 给邮箱0配置的过滤器
    sFilterConfig1.FilterActivation = CAN_FILTER_ENABLE;       /* 激活滤波器0 */
    sFilterConfig1.SlaveStartFilterBank = 13;				//设置CAN2的起始过滤器组（对于单CAN的CPU或从CAN此参数无效；对于双CAN的CPU此参数为从CAN的起始过滤器组编号）
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig1);             /* 过滤器配置 */
                        
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    
    HAL_CAN_Start(&hcan1);//此语句不打开，则不能发送也不能接收数据
}

//**********************官方配置通信类型***************************//
//**********************官方配置通信类型***************************// 
//**********************官方配置通信类型***************************//

/**
  * @brief          通信类型0 小米电机ID检查 获取设备的id和64为MCU唯一标识符
  * @param[in]      CAN_ID_Frame:电机回复帧中的扩展CANID   
  * @retval         电机CANID
  */
void Check_cyber(uint8_t ID)
{
    uint8_t tx_data[8] = {0};
    txMsg.ExtId = Communication_Type_GetID<<24|Master_CAN_ID<<8|ID;
    can1_txd();
}

/**
  * @brief          通信类型1 小米运控模式指令  应答帧为通信类型2
  * @param[in]      Motor:对应控制电机结构体   
  * @param[in]      tor: 力矩对应-12N-12N
  * @param[in]      vel_rads:目标角速度 -30-30rad/s
  * @param[in]      pos_deg:目标角度 -720-720度（-4pi-4pi）
  * @param[in]      kp: 0-500
  * @param[in]      kd:0-5
  * @retval         None
  */
void Cyber_ControlMode(Cyber_Motor *Motor,float tor, float vel_rads, float pos_deg, float kp, float kd)
{
    float pos_rad = pos_deg * pi / 180;
//    float vel_rads = vel_rpm * 30 / pi;
    
    uint8_t tx_data[8];//发送数据初始化
    //装填发送数据
    tx_data[0]=float_to_uint(pos_rad,P_MIN,P_MAX,16)>>8;  
    tx_data[1]=float_to_uint(pos_rad,P_MIN,P_MAX,16);  
    tx_data[2]=float_to_uint(vel_rads,V_MIN,V_MAX,16)>>8;  
    tx_data[3]=float_to_uint(vel_rads,V_MIN,V_MAX,16);  
    tx_data[4]=float_to_uint(kp,KP_MIN,KP_MAX,16)>>8;  
    tx_data[5]=float_to_uint(kp,KP_MIN,KP_MAX,16);  
    tx_data[6]=float_to_uint(kd,KD_MIN,KD_MAX,16)>>8;  
    tx_data[7]=float_to_uint(kd,KD_MIN,KD_MAX,16); 
    
    txMsg.ExtId = Communication_Type_MotionControl<<24|float_to_uint(tor,T_MIN,T_MAX,16)<<8|Motor->CAN_ID;//装填扩展帧数据
    can1_txd();
}

/**
  * @brief          通信类型2 电机反馈数据  为通信类型1的应答帧
  * @retval         None
                    有点疑问 难道不是接受此帧？
  */
void request_motor_feedback(Cyber_Motor *Motor)
{
    uint8_t tx_data[8]={0};
	txMsg.ExtId = Communication_Type_MotorRequest<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    can1_txd();
}


/**
  * @brief          通信类型3 使能电机 应答帧为通信类型2
  * @retval         None
  */
void Start_Cyber(Cyber_Motor *Motor)
{
    uint8_t tx_data[8] = {0}; 
    txMsg.ExtId = Communication_Type_MotorEnable<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    can1_txd();
}
 
/**
  * @brief          通信类型4 停止电机 应答帧为通信类型2
  * @retval         None
  */
void Stop_Cyber(Cyber_Motor *Motor,uint8_t clear_error)
{
	uint8_t tx_data[8]={0};
	tx_data[0]=clear_error;//清除错误位设置 clear_error=1时清除故障
	txMsg.ExtId = Communication_Type_MotorStop<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    can1_txd();
}

/**
  * @brief          通信类型6 设置电机当前位置为零点（掉电丢失） 应答帧为通信类型2
  * @retval         None
  */
void Set_Cyber_ZeroPos(Cyber_Motor *Motor)
{
    uint8_t tx_data[8]={0};
    tx_data[0]=1;
    txMsg.ExtId = Communication_Type_SetPosZero<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    can1_txd();
}

/**
  * @brief          通信类型7 设置电机CANID  应答帧为通信类型0
  * @retval         None
  */
void set_CANID_cybergear(Cyber_Motor *Motor,uint8_t CAN_ID)
{
	uint8_t tx_data[8]={0};
	txMsg.ExtId = Communication_Type_CanID<<24|CAN_ID<<16|Master_CAN_ID<<8|Motor->CAN_ID;
    Motor->CAN_ID = CAN_ID;//将新的ID导入电机结构体
    can1_txd();	
}

/**
  * @brief          读取电机参数 通信类型17
  * @param[in]      Motor:对应控制电机结构体
  * @param[in]      Index:写入参数对应地址
  * @param[in]      Value:写入参数值
  * @param[in]      Value_type:写入参数数据类型
  * @retval         none
  */
void Read_Cyber_Parameter(Cyber_Motor *Motor,uint16_t Index)
{
	uint8_t tx_data[8]= {0};
	txMsg.ExtId = Communication_Type_GetSingleParameter<<24|Master_CAN_ID<<8|Motor->CAN_ID;
	tx_data[0]=Index;
	tx_data[1]=Index>>8;
	can1_txd();	
}

/**
  * @brief          通信类型18 写入电机参数 应答帧为通信类型2 
  * @param[in]      Motor:对应控制电机结构体
  * @param[in]      Index:写入参数对应地址
  * @param[in]      Value:写入参数值
  * @param[in]      Value_type:写入参数数据类型
  * @retval         none
  */
void Set_Cyber_Parameter(Cyber_Motor *Motor,uint16_t Index,float Value,char Value_type)
{
	uint8_t tx_data[8]= {0};
	txMsg.ExtId = Communication_Type_SetSingleParameter<<24|Master_CAN_ID<<8|Motor->CAN_ID;
    memcpy(&tx_data[0],&Index,2);
	if(Value_type == 'f')
    {
        memcpy(&tx_data[4], &Value, sizeof(Value));
	}
	else if(Value_type == 's')
        {
		tx_data[4]=(uint8_t)Value;
	}
	can1_txd();	
    HAL_Delay(1);
}
 
//**********************基于通信类型17 18 编写的函数**********************//
//**********************基于通信类型17 18 编写的函数**********************//
//**********************基于通信类型17 18 编写的函数**********************//

//@brief          设置电机模式(必须停止时调整！)
void Set_Cyber_Mode(Cyber_Motor *Motor,uint8_t Mode)
{	
	Set_Cyber_Parameter(Motor,RunMode_idx,Mode,'s');
}
 
// 位置模式下设置
//@brief          设置电机位置，单位弧度；  补充value为角度时的内容
void Set_Cyber_Pos(Cyber_Motor *Motor, float value) 
{
    float pos = value * pi / 180;
    Set_Cyber_Parameter(Motor, LocRef_idx, pos, 'f');
    Motor->des_pos = value;
}
//@brief          设置电机运动限速，单位弧度/s
void Set_Cyber_limitSp(Cyber_Motor *Motor, float value) {
    Set_Cyber_Parameter(Motor, LimitSpd_idx, value, 'f');
    Motor->max_vel = value;
}
//@brief          设置电机运动最大扭矩，单位Nm
void Set_Cyber_limitTor(Cyber_Motor *Motor, float value)
{
    Set_Cyber_Parameter(Motor, LimitTorque_idx, value, 'f');
    Motor->max_tor = value;
}
//@brief          设置电机当前圈数
void Set_Cyber_RotNum(Cyber_Motor *Motor, float value)
{
    Set_Cyber_Parameter(Motor, Rotation_idx, value, 'f');
    //Motor->max_tor = value;
}
//@brief          电流控制模式下设置电流
void Set_Cyber_Cur(Cyber_Motor *Motor,float Current)
{
	Set_Cyber_Parameter(Motor,IqRef_idx,Current,'f');
}
//@brief          读取电机当前位置模式指令角度
void Read_Cyber_Pos(Cyber_Motor *Motor)
{
    Read_Cyber_Parameter(Motor, LocRef_idx);
} 
 
//**********************应答帧编写的数据接受函数**********************//
//**********************应答帧编写的数据接受函数**********************//
//**********************应答帧编写的数据接受函数**********************//

/**
  * @brief          电机回复帧数据处理函数 通信类型2对应数据处理函数
  * @param[in]      Motor:对应控制电机结构体   
  * @param[in]      DataFrame:数据帧
  * @param[in]      IDFrame:扩展ID帧
  * @retval         None
  */
void Motor_Data_Handler(Cyber_Motor *Motor,uint8_t DataFrame[8],uint32_t IDFrame)
{	
    Motor->pre_pos=uint16_to_float(DataFrame[0]<<8|DataFrame[1],MIN_P,MAX_P,16);//DataFrame[0]<<8|DataFrame[1]低8位和高8位合并
    Motor->pre_vel=uint16_to_float(DataFrame[2]<<8|DataFrame[3],V_MIN,V_MAX,16);
    Motor->pre_tor=uint16_to_float(DataFrame[4]<<8|DataFrame[5],T_MIN,T_MAX,16);
    Motor->pre_temperature=(DataFrame[6]<<8|DataFrame[7])*Temp_Gain;
    Motor->error_code=(IDFrame&0x1F0000)>>16;
}

uint8_t pre_pos_ready(Cyber_Motor *Motor,float pos,float fompos)
{
    if(fabs(Motor->pre_pos)>=fabs(pos)-fompos) return 1;
    else return 0; 
}

uint8_t Start_ready(Cyber_Motor *Motor,Cyber_Motor *Motor2)
{
    if((Motor->pre_temperature>=10)&&(Motor2->pre_temperature>=10)) return 0;
    else return 1; 
}
/***************************************电机信息接受和发送**************************************/

/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
int16_t motor_id;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxMsg, rx_data);//接收数据  
    motor_id= (rxMsg.ExtId&0xFF00)>>8;
    if (motor_id==1) {Motor_Data_Handler(&Shift_Cyber,rx_data,rxMsg.ExtId);}
    else if (motor_id==2){Motor_Data_Handler(&Clutch_Cyber,rx_data,rxMsg.ExtId);}
//    if(rxMsg.StdId==1000&&rx_data[0]<=14 && rx_data[1]==0)//每帧前两位的状态符合标准则使用该帧
//        {   
//            CZCD_CANData_tran(rx_data);
//            
//        }
//    else  printf("%d\n",rxMsg.StdId);

}
extern uint16_t Motor_speed,Motor_dir;
extern uint8_t Eshift_up_flag;
void CANtest(Cyber_Motor *Motor1,Cyber_Motor *Motor2)
{
//printf("%f,%f,%f,%f,%d\n",Motor->pre_pos,Motor->pre_vel,Motor->pre_tor,Motor->pre_temperature,Motor->error_code);
//JustFloat_8(Motor->pre_pos,Motor->pre_vel,Motor->pre_tor,Motor->pre_temperature,RPM,GEAR,TPS,ECUvlot);
JustFloat_8(Motor1->pre_pos,Motor1->pre_vel,Motor1->pre_tor,Motor1->pre_temperature,Motor2->pre_pos,Motor2->pre_vel,Motor2->pre_tor,Motor2->pre_temperature);

}
 
/***************************************暂时不用***************************************/

/**
  * @brief          提取电机回复帧扩展ID中的电机CANID
  * @param[in]      CAN_ID_Frame:电机回复帧中的扩展CANID   
  * @retval         电机CANID
  */
uint32_t Get_Motor_ID(uint32_t CAN_ID_Frame)
{
	return (CAN_ID_Frame&0xFFFF)>>8;
}

//****************************************************************************//
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
        if (Motor->pre_vel < 0.01f)
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



/***************************************电机内部协议***************************************/
/**
  * @brief          浮点数转4字节函数
  * @param[in]      f:浮点数
  * @retval         4字节数组
  * @description  : IEEE 754 协议
  */
uint8_t* Float_to_Byte(float f)
{
	unsigned long longdata = 0;
	longdata = *(unsigned long*)&f;       
	byte[0] = (longdata & 0xFF000000) >> 24;
	byte[1] = (longdata & 0x00FF0000) >> 16;
	byte[2] = (longdata & 0x0000FF00) >> 8;
	byte[3] = (longdata & 0x000000FF);
	return byte;
}
 
/**
  * @brief          小米电机回文16位数据转浮点
  * @param[in]      x:16位回文
  * @param[in]      x_min:对应参数下限
  * @param[in]      x_max:对应参数上限
  * @param[in]      bits:参数位数
  * @retval         返回浮点值
  */
float uint16_to_float(uint16_t x,float x_min,float x_max,int bits)
{
    uint32_t span = (1 << bits) - 1;
    float offset = x_max - x_min;
    return offset * x / span + x_min;
}
 
/**
  * @brief          小米电机发送浮点转16位数据
  * @param[in]      x:浮点
  * @param[in]      x_min:对应参数下限
  * @param[in]      x_max:对应参数上限
  * @param[in]      bits:参数位数
  * @retval         返回浮点值
  */
int float_to_uint(float x, float x_min, float x_max, int bits)
{
  float span = x_max - x_min;
  float offset = x_min;
  if(x > x_max) x=x_max;
  else if(x < x_min) x= x_min;
  return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}
