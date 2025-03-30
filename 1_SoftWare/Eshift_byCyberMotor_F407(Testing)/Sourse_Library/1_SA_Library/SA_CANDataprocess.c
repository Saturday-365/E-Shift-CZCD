/****************************************************************************************************
 * @author     		sjc
 * @version    		0.1
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STM32F103C8T6
 * @date       		2024-07-5
 * @note		
参考文献 https://blog.csdn.net/qq_43400642/article/details/102758938
         江协科技 can总线入门教程

HQU14第一版 pcb中 
rx口为a10 tx口为a9
canrxd 为 b8 txd为 b9

通用仪表中有设置参数id和 波特率为20hz
7.设置CAN总线ID匹配仪表盘预期的ID号。
8.设置比特率为20Hz或仪表盘预期数值。
9.确认没有其它的CAN总线通道被配置为与通用型仪表盘设备通道使用相同的CAN总线ID。

can波特率配置
波特率kbit/s	    CAN_SJW	        CAN_BS1	    CAN_BS2	        CAN_Prescaler                      
原文链接：https://blog.csdn.net/occasionally/article/details/123844179


 ********************************************************************************************************************/
#include "headfile.h"
#include "SA_CANDataprocess.h"
#include "can.h"
#include "CyberGear_Control.h"

CAN_RxHeaderTypeDef ECU_rxMsg;//发送接收结构体

uint8_t rx_ECUdata[8];       //接收数据
uint8_t ECUbyte[4];          //转换临时数据
 

///*	CAN接收数据处理 */
Data_Radio ECUDATA;

uint8_t ReceFlage=0;
extern uint8_t RxBuffer_3[LENGTH];

void Init_DATA_CAN()
{
/* 配置CAN过滤器 */
	CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 14;                             /* 过滤器14 */
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;                      /* 32位ID */
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;                  /* 32位MASK */
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1; // 给邮箱1配置的过滤器
    sFilterConfig.FilterActivation = CAN_FILTER_ENABLE;       /* 激活滤波器0 */
    sFilterConfig.SlaveStartFilterBank = 13;				//设置CAN2的起始过滤器组（对于单CAN的CPU或从CAN此参数无效；对于双CAN的CPU此参数为从CAN的起始过滤器组编号）
    HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig);             /* 过滤器配置 */
                        
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);    
    HAL_CAN_Start(&hcan2);//此语句不打开，则不能发送也不能接收数据
}
        
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &ECU_rxMsg, rx_ECUdata);//接收数据  
    if(rx_ECUdata[0]<=14 && rx_ECUdata[1]==0) CZCD_CANData_tran(&ECUDATA,rx_ECUdata);
    HAL_GPIO_WritePin((GPIO_TypeDef *)LED_GPIO_Port, (uint16_t)LED_Pin, (GPIO_PinState)0);

}
void CZCD_CANData_tran(Data_Radio *DATA,uint8_t CANRxData[LENGTH])
{
 switch (CANRxData[0]) 
            {//data[1] << 8) | data[0]
                case 0:       
                        DATA->RPM=((CANRxData[2]<<8)|CANRxData[3])/256;
                        DATA->MAP=((CANRxData[4]<<8)|CANRxData[5])/256;break;
                case 1: 
                        DATA->TPS=((CANRxData[4]<<8|(CANRxData[5]))*0.1)/256;break;
                case 2:  
                        DATA->CLT=((CANRxData[6]<<8|(CANRxData[7]))/256)-50;break;
                case 3:
                        DATA->IAT=((CANRxData[2]<<8|(CANRxData[3]))-50)/256;
                        DATA->ECUvlot=((CANRxData[4]<<8)|CANRxData[5])/256*0.01;break;
                case 4:
                        DATA->GEAR=((CANRxData[2]<<8)|CANRxData[3])/256;
                        DATA->RealGEAR=stabilize_gear(DATA->GEAR);        
                        DATA->IgnitionTiming=((CANRxData[6]<<8|(CANRxData[7]))*0.1-100)/256;break;
                case 5:break;
                case 6: 
                        DATA->LAMDA1=((CANRxData[4]<<8|(CANRxData[5]))*0.001)/256;break;
                case 7:break;
                case 8:
                        DATA->OilPressure=(CANRxData[4]<<8|(CANRxData[5]))/256;break;
                case 9:break;
                case 10:break;
                case 11:break;
                case 12:break;
                case 13:
                        DATA->APPS=((CANRxData[2]<<8|CANRxData[3])*0.1)/256;break;
                
                default:break;                    
            }
            
}  
//union Fp32
//{
//    uint32_t u;
//    float f;
//};


//float uint6_cov_float(uint16_t value)
//{
//    const Fp32 magic = { (254U - 15U) << 23 };
//    const Fp32 was_infnan = { (127U + 16U) << 23 };
//    Fp32 out;

//    out.u = (value & 0x7FFFU) << 13;   /* exponent/mantissa bits */
//    out.f *= magic.f;                  /* exponent adjust */
//    if (out.f >= was_infnan.f)         /* make sure Inf/NaN survive */
//    {
//        out.u |= 255U << 23;
//    }
//    out.u |= (value & 0x8000U) << 16;  /* sign bit */

//    return out.f;
//}

//  
//void CZCD_CANData_Send(void)
//{
//    if(ReceFlage==1)
//    {
////        Serial_Printf( "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",RPM,MAP,TPS,CLT,IAT,ECUvlot,GEAR,LAMDA1,OilPressure,APPS,IgnitionTiming); 
////        Serial_Printf( "%d,%f\n",RPM,MAP); 
////        Serial_Printf( "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",RPM,MAP,TPS,CLT,IAT,ECUvlot,GEAR,LAMDA1,OilPressure,APPS,IgnitionTiming); 
////        Serial_Printf( "RPM=%d,MAP=%d,TPS=%d,CLT=%d,IAT=%d,ECUvlot=%d,GEAR=%d,LAMDA1=%d,OilPressure=%d,APPS=%d,IgnitionTiming=%d\n",
////                        RPM,   MAP,TPS,CLT,IAT,ECUvlot,GEAR,LAMDA1,OilPressure,APPS,IgnitionTiming);

//        //        sent_data(RPM,MAP,TPS);        
//    }
////    else if(ReceFlage==2)  Serial_Printf( "Error!\n"); 
////    Delay_ms(30);  
//}

