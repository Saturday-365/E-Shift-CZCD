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

///*	CAN接收数据处理 */

int   RPM=0;
int MAP=0;
int TPS=0;
int CLT=0;
int IAT=0;
int ECUvlot=0;
int GEAR=0;
int LAMDA1=0;
int OilPressure=0;
int APPS=0;
int IgnitionTiming=0;
uint8_t ReceFlage=0;
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

//void CZCD_CANData_tran(void)
//{
// switch (CANRxData[0]) 
//            {
//                case 1:       
//                        RPM=(CANRxData[2]+(CANRxData[3]<<8));
//                        MAP=(CANRxData[4]+(CANRxData[5]<<8));break;
//                case 2: 
//                        TPS=(CANRxData[4]+(CANRxData[5]<<8))*0.1;break;
//                case 3:  
//                        CLT=(CANRxData[6]+(CANRxData[7]<<8))-50;break;
//                case 4:
//                        IAT=(CANRxData[2]+(CANRxData[3]<<8))-50;
//                        ECUvlot=(CANRxData[4]+(CANRxData[5]<<8))*0.01;break;
//                case 5:
//                        GEAR=(CANRxData[2]+(CANRxData[3]<<8));
//                        IgnitionTiming=(CANRxData[6]+(CANRxData[7]<<8))*0.1-100;break;
//                case 6:break;
//                case 7: 
//                        LAMDA1=(CANRxData[4]+(CANRxData[5]<<8))*0.001;break;
//                case 8:break;
//                case 9:
//                        OilPressure=(CANRxData[4]+(CANRxData[5]<<8));break;
//                case 10:break;
//                case 11:break;
//                case 12:break;
//                case 13:break;
//                case 14:
//                        APPS=(CANRxData[2]+(CANRxData[3]<<8))*0.1;break;
//                
//                default:break;                    
//            }
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

