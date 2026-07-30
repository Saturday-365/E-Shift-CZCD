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
#include "CyberGear_Control.h"

///*	CAN接收数据处理 */

float   RPM=1000;
float MAP=0;
float TPS=100;
float CLT=0;
float IAT=0;
float ECUvlot=0;
float GEAR=2;
float LAMDA1=0;
float OilPressure=0;
float APPS=0;
float IgnitionTiming=0;
uint8_t ReceFlage=0;
extern uint8_t RxBuffer_3[LENGTH];
//void CZCD_CAN_Runing(void)
//{		             
//        CZCD_CAN_Receive(CANRxData);	
//        if(CANRxData[0]<=14 && CANRxData[1]==0)//每帧前两位的状态符合标准则使用该帧
//        {   
//            ReceFlage=1;
//            CZCD_CANData_tran();
//        }
//        else ReceFlage=2;
//    memset(CANRxData,0,8);  //清空缓存数组       
//    
//}            

void CZCD_CANData_tran(uint8_t CANRxData[LENGTH])
{
 switch (CANRxData[0]) 
            {
                case 1:       
                        RPM=uint16_to_float(CANRxData[2]<<8|(CANRxData[3]),0,65535,16);
                        MAP=uint16_to_float(CANRxData[4]<<8|(CANRxData[5]),0,65535,16);break;
                case 2: 
                        TPS=uint16_to_float(CANRxData[4]<<8|(CANRxData[5]),0,65535,16)*0.1;break;
                case 3:  
                        CLT=uint16_to_float(CANRxData[6]<<8|(CANRxData[7]),0,65535,16)-50;break;
                case 4:
                        IAT=uint16_to_float(CANRxData[2]<<8|(CANRxData[3]),0,65535,16)-50;
                        ECUvlot=uint16_to_float(CANRxData[4]<<8|(CANRxData[5]),0,65535,16)*0.01;break;
                case 5:
                        GEAR=uint16_to_float(CANRxData[2]<<8|(CANRxData[3]),0,65535,16);
                        IgnitionTiming=uint16_to_float(CANRxData[6]<<8|(CANRxData[7]),0,65535,16)*0.1-100;break;
                case 6:break;
                case 7: 
                        LAMDA1=uint16_to_float(CANRxData[4]<<8|(CANRxData[5]),0,65535,16)*0.001;break;
                case 8:break;
                case 9:
                        OilPressure=uint16_to_float(CANRxData[4]<<8|(CANRxData[5]),0,65535,16);break;
                case 10:break;
                case 11:break;
                case 12:break;
                case 13:break;
                case 14:
                        APPS=uint16_to_float((CANRxData[2]<<8|CANRxData[3]),0,65536,16)*0.1;break;
                
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

