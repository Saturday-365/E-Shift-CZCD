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

////uint32_t RxID;
////uint8_t RxLength;
////extern uint8_t CANRxData[8];

///*CAN GPIO 和时钟配置 */
// void CAN_GPIO_Config(void)
//{
//  GPIO_InitTypeDef GPIO_InitStructure; 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能外设时钟
//	
//  /* 复用功能和GPIOB端口时钟使能*/	 
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);	                        											 
// 
//  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
// 
//  /* Configure CAN pin: RX */	 // PB8
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
//  /* Configure CAN pin: TX */   // PB9
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  
///* CAN1 模块时钟使能 */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
//}
// 

/////*CAN RX1 中断优先级配置  */
// void CAN_NVIC_Configuration(void)
//{
//        //定义结构体变量
//    NVIC_InitTypeDef NVIC_InitStructure; 
//  	/* Configure the NVIC Preemption Priority Bits */  
//  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//	/* enabling interrupt */
//    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);						
// 	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;//选择配置NVIC的canT1线
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//指定NVIC线路的抢占优先级为0
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//指定NVIC线路的响应优先级为0
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//指定NVIC线路使能
//  	NVIC_Init(&NVIC_InitStructure);//将结构体变量交给NVIC_Init，配置NVIC外设
//    
//}


///*	CAN初始化 */
// void CZCD_CAN_Init(void) 
//{
//	CAN_GPIO_Config();
//    /*CAN初始化*/
//	CAN_InitTypeDef CAN_InitStructure;              //定义结构体变量
//	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //在我们的使用过程中只需要读取ecu与仪表之间的通讯 故设置为静默模式    
//    
//    //设置波特率
//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;    //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;    //Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
//	CAN_InitStructure.CAN_Prescaler = 12;       //指定时间量程的长度 //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq//分频系数(Fdiv)为brp+1

//    CAN_InitStructure.CAN_TTCM = DISABLE;   //非时间触发通信模式 
//	CAN_InitStructure.CAN_ABOM = DISABLE;   //软件自动离线管理	
//	CAN_InitStructure.CAN_AWUM = DISABLE;   //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
//	CAN_InitStructure.CAN_NART = DISABLE;   //禁止报文自动传送 
//	CAN_InitStructure.CAN_RFLM = DISABLE;   //报文不锁定,新的覆盖旧的 
//	CAN_InitStructure.CAN_TXFP = DISABLE;   //优先级由报文标识符决定
//	CAN_Init(CAN1, &CAN_InitStructure); 
//	
//    //过滤器0
//	CAN_FilterInitTypeDef CAN_FilterInitStructure;
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
//    CAN_NVIC_Configuration(); //CAN中断初始化         
//}

///*	CAN发送代码 */
// void CZCD_CAN_Transmit(uint32_t ID, uint8_t Length, uint8_t *Data)
//{
//	CanTxMsg TxMessage;
//	TxMessage.StdId = ID;   // 标准标识符
//	TxMessage.ExtId = ID;   // 设置扩展标示符
//	TxMessage.IDE = CAN_Id_Standard;   // 标准帧
//	TxMessage.RTR = CAN_RTR_Data;    // 数据帧
//	TxMessage.DLC = Length;      // 要发送的数据长度
//	for (uint8_t i = 0; i < Length; i ++)
//	{
//		TxMessage.Data[i] = Data[i];
//	}
//	uint8_t TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
//	while (CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok);
//}


///*	CAN接收标志位 */
//uint8_t CZCD_CAN_ReceiveFlag(void)
//{   
//    if (CAN_MessagePending(CAN1, CAN_FIFO0) > 0)
//	{
//        return 1;
//	} 
//	return 0;
//}
///*	CAN接收代码 */
//void CZCD_CAN_Receive(uint8_t *Data)
//{
//	CanRxMsg RxMessage;
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
//		for (uint8_t i = 0; i < 8; i ++)
//		{
//			Data[i] = RxMessage.Data[i];
//		}
//    memset(&RxMessage,0,8);
//}

///* USB中断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。 */
//uint8_t CANRxData[8]={0};
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
//    CZCD_CAN_Runing();
//    CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
//}
//    

///*	CAN接收数据处理 */

//int   RPM=0;
//int MAP=0;
//int TPS=0;
//int CLT=0;
//int IAT=0;
//int ECUvlot=0;
//int GEAR=0;
//int LAMDA1=0;
//int OilPressure=0;
//int APPS=0;
//int IgnitionTiming=0;
//uint8_t ReceFlage=0;
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
////                case 6:break;
//                case 7: 
//                        LAMDA1=(CANRxData[4]+(CANRxData[5]<<8))*0.001;break;
////                case 8:break;
//                case 9:
//                        OilPressure=(CANRxData[4]+(CANRxData[5]<<8));break;
////                case 10:break;
////                case 11:break;
////                case 12:break;
////                case 13:break;
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
//        Serial_Printf( "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",RPM,MAP,TPS,CLT,IAT,ECUvlot,GEAR,LAMDA1,OilPressure,APPS,IgnitionTiming); 
////        Serial_Printf( "RPM=%d,MAP=%d,TPS=%d,CLT=%d,IAT=%d,ECUvlot=%d,GEAR=%d,LAMDA1=%d,OilPressure=%d,APPS=%d,IgnitionTiming=%d\n",
////                        RPM,   MAP,TPS,CLT,IAT,ECUvlot,GEAR,LAMDA1,OilPressure,APPS,IgnitionTiming);

//        //        sent_data(RPM,MAP,TPS);        
//    }
//    else if(ReceFlage==2)  Serial_Printf( "Error!\n"); 
////    Delay_ms(30);  
//}

