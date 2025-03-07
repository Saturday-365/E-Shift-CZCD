#include "math.h"

unsigned char c0h[4],c1h[4],c2h[4],c3h[4],c4h[4],c5h[4],c6h[4],c7h[4],c8h[4],c9h[4],c10h[4],
                        c11h[4],c12h[4],c13h[4],c14h[4],c15h[4],c16h[4];
unsigned char tail[4]={0x00, 0x00, 0x80, 0x7f};	//vofa输出帧尾

//////////////////////////////vofa调试参数定义//////////////////////////////
	typedef union
{
    float fdata;
    unsigned long ldata;
}FloatLongType;

typedef union {
	float numeric;
	unsigned char ascii[4];
}CharFloat;

float charTofloat(unsigned char *str) {
	CharFloat value;
	value.ascii[0] = str[0];
	value.ascii[1] = str[1];
	value.ascii[2] = str[2];
	value.ascii[3] = str[3];
	return value.numeric;
}

void Float_to_Byte(float f,unsigned char byte[]) /*将浮点数f转化为4个字节数据存放在byte[4]中*/
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}
float vofa_test_1=0,vofa_test_2=0,vofa_test_3=0,vofa_test_4=0,vofa_test_5=0;


void Vofa_PID_Adjust()
{
    float data_Get = Get_Data(); // 存放接收到的数据
	
	if(DataBuff[0]=='P') 
	{
		vofa_test_1= data_Get;
//		vofa_test_1=func_limit_ab(vofa_test_1,-100,100);
        }
	else if(DataBuff[0]=='I' ) 
	{
		vofa_test_2 = data_Get;
//		vofa_test_2=func_limit_ab(vofa_test_2,-100,100);
        }       
    else if(DataBuff[0]=='D' ) 
	{
		vofa_test_3 = data_Get;
//		vofa_test_3=func_limit_ab(vofa_test_3,-100,100);
        }
	else if(DataBuff[0]=='S' ) 
	{
		vofa_test_4 = data_Get;
//		vofa_test_4=func_limit_ab(vofa_test_4,-600,600);
    }

//	else if(DataBuff[0]=='I' ) 
//		vofa_test_4 = data_Get;
//	else if(DataBuff[0]=='I' ) 
//		vofa_test_5 = data_Get;
	
}


//	void vofatest()
//{
//	if(EA!=0){
//		
//#if vofachanal==1    //速度环 
//        Float_to_Byte(voltK,c1h);	
//		Float_to_Byte(test1,c2h);		
//		Float_to_Byte(test4,c3h);	
//		Float_to_Byte(SpeedR_filt,c4h);	
//		Float_to_Byte(SpeedRset,c5h);	
//		Float_to_Byte(SpeedL_filt,c6h);
//		Float_to_Byte(SpeedLset,c7h);	
//		Float_to_Byte(voltR,c8h);
//		Float_to_Byte(voltL,c9h);	
//		wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c8h,4);
//		wireless_uart_send_buff(c9h,4);
//#endif
//#if vofachanal==4    //速度环 
//        Float_to_Byte(voltK,c1h);	
//		Float_to_Byte(vofa_test_1,c2h);		
//		Float_to_Byte(vofa_test_2,c3h);	
//		Float_to_Byte(vofa_test_3,c4h);	
//		Float_to_Byte(vofa_test_4,c5h);	
//		Float_to_Byte(SpeedR_filt,c6h);
//		Float_to_Byte(SpeedRset,c7h);	
//		Float_to_Byte(SpeedL_filt,c8h);
//		Float_to_Byte(SpeedLset,c9h);	
//		Float_to_Byte(voltR,c10h);
//		Float_to_Byte(voltL,c11h);	
//		wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c8h,4);
//		wireless_uart_send_buff(c9h,4);       
//		wireless_uart_send_buff(c10h,4);
//		wireless_uart_send_buff(c11h,4);
//#endif
//#if vofachanal==2    //角度环 
//		Float_to_Byte(voltK,c1h);	
//		Float_to_Byte(Dir_error,c2h);		
////		Float_to_Byte(deviation_mor,c3h);	
//		Float_to_Byte(pidd_err_dert,c4h);	
//        Float_to_Byte(pidd_Kp,c5h);	
//		Float_to_Byte(pidd_Kdacc,c6h);	    
//        Float_to_Byte(SpeedR_filt,c7h);	
//		Float_to_Byte(SpeedRset,c8h);	
//		Float_to_Byte(SpeedL_filt,c9h);
//		Float_to_Byte(SpeedLset,c10h);	
//		Float_to_Byte(acc_err_real,c11h);
//		Float_to_Byte(voltR,c12h);
//		Float_to_Byte(voltL,c13h);	
//        wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
////		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c8h,4);
//		wireless_uart_send_buff(c9h,4);
//		wireless_uart_send_buff(c10h,4);
//		wireless_uart_send_buff(c11h,4);
// 		wireless_uart_send_buff(c12h,4);
//        wireless_uart_send_buff(c13h,4);
//#endif
//#if vofachanal==10  //电感值
//        Float_to_Byte(Left_Adc,c1h);	
//		Float_to_Byte(Left_Adc_flit,c2h);	
//		Float_to_Byte(LeftM_Adc,c3h);
//		Float_to_Byte(LeftM_Adc_flit,c4h);
//		Float_to_Byte(Left_M_Adc,c5h);	
//		Float_to_Byte(Left_M_Adc_flit,c6h);		
//		Float_to_Byte(Middle_Adc,c7h);	
//		Float_to_Byte(Middle_Adc_flit,c8h);	
//		Float_to_Byte(Right_M_Adc,c9h);	
//		Float_to_Byte(Right_M_Adc_flit,c10h);	
//		Float_to_Byte(RightM_Adc,c11h);	
//		Float_to_Byte(RightM_Adc_flit,c12h);	
//		Float_to_Byte(Right_Adc,c13h);
//		Float_to_Byte(Right_Adc_flit,c14h);
//		Float_to_Byte(Dir_error,c15h);
//        wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c8h,4);
//		wireless_uart_send_buff(c9h,4);
//		wireless_uart_send_buff(c10h,4);
//		wireless_uart_send_buff(c11h,4);
//		wireless_uart_send_buff(c12h,4);
//		wireless_uart_send_buff(c13h,4);
//		wireless_uart_send_buff(c14h,4);
// 		wireless_uart_send_buff(c15h,4);
// #endif  
//#if vofachanal==11  //电感值原值
//		Float_to_Byte(Left_Adc,c1h);	
//		Float_to_Byte(LeftM_Adc,c3h);
//		Float_to_Byte(Left_M_Adc,c5h);	
//		Float_to_Byte(Middle_Adc,c7h);	
//		Float_to_Byte(Right_M_Adc,c9h);	
//		Float_to_Byte(RightM_Adc,c11h);	
//		Float_to_Byte(Right_Adc,c13h);
//		Float_to_Byte(Dir_error,c14h);
//        wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c9h,4);
//		wireless_uart_send_buff(c11h,4);
//		wireless_uart_send_buff(c13h,4);
// 		wireless_uart_send_buff(c14h,4); 
//#endif

//#if vofachanal==12  //电感值归一化
//		Float_to_Byte(Left_Adc_1,c1h);	
//		Float_to_Byte(LeftM_Adc_1,c3h);
//		Float_to_Byte(Left_M_Adc_1,c5h);	
//		Float_to_Byte(Middle_Adc_1,c7h);	
//		Float_to_Byte(Right_M_Adc_1,c9h);	
//		Float_to_Byte(RightM_Adc_1,c11h);	
//		Float_to_Byte(Right_Adc_1,c13h);
//		Float_to_Byte(Dir_error,c14h);
//        wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c9h,4);
//		wireless_uart_send_buff(c11h,4);
//		wireless_uart_send_buff(c13h,4);
// 		wireless_uart_send_buff(c14h,4); 
//        
//        #endif    
//#if vofachanal==3   // 直立环
//        Float_to_Byte(voltK,c1h);	
////		Float_to_Byte(vofa_test_1,c2h);		
////		Float_to_Byte(vofa_test_2,c3h);	
////		Float_to_Byte(vofa_test_3,c4h);	
////		Float_to_Byte(vofa_test_4,c5h);	
//		Float_to_Byte(pullup_value,c6h);
//		Float_to_Byte(std_err ,c7h);	
//		Float_to_Byte(SpeedRset,c8h);
//		Float_to_Byte(SpeedLset,c9h);	
//		Float_to_Byte(voltR,c10h);
//		Float_to_Byte(voltL,c11h);	
//		wireless_uart_send_buff(c1h,4);
////		wireless_uart_send_buff(c2h,4);
////		wireless_uart_send_buff(c3h,4);
////		wireless_uart_send_buff(c4h,4);
////		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
////		wireless_uart_send_buff(c8h,4);
//		wireless_uart_send_buff(c9h,4);       
//		wireless_uart_send_buff(c10h,4);
//		wireless_uart_send_buff(c11h,4);       
//    #endif 
//#if vofachanal==5   // 元素角度和距离积分调试
//        Float_to_Byte(voltK,c1h);	
//		Float_to_Byte(distance,c2h);		
//		Float_to_Byte(Angle,c3h);	
//		wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
////		wireless_uart_send_buff(c4h,4);
////		wireless_uart_send_buff(c5h,4);
////		wireless_uart_send_buff(c6h,4);
////		wireless_uart_send_buff(c7h,4);
////		wireless_uart_send_buff(c8h,4);
////		wireless_uart_send_buff(c9h,4);       
////		wireless_uart_send_buff(c10h,4);
////		wireless_uart_send_buff(c11h,4);       
//    #endif           
//      
//#if vofachanal==6   // 避障调试
//        Float_to_Byte(flage,c1h);	
//		Float_to_Byte(brdiff,c2h);		
//		Float_to_Byte(dl1b_distance_mm,c3h);	
//        Float_to_Byte(pullup_value,c4h);	
//		Float_to_Byte(std_angle,c5h);		
//		Float_to_Byte(distance,c6h);	
//		Float_to_Byte(ramp_process_flage,c7h);		
//		Float_to_Byte(Middle_Adc,c8h);
//        
//		wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
//		wireless_uart_send_buff(c8h,4);
//        
//    #endif     

//#if vofachanal==7   // 圆环调试
//        Float_to_Byte(flage,c1h);	
//		Float_to_Byte(Left_M_Adc_1,c2h);		
//		Float_to_Byte(Right_M_Adc_1,c3h);	
//        Float_to_Byte(round_process_flage_0,c4h);	
//		Float_to_Byte(round_process_flage_1,c5h);		
//		Float_to_Byte(Middle_Adc,c6h);	

//		wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
// #endif
//#if vofachanal==99   // 圆环调试

//        Float_to_Byte(SpeedR_filt,c1h);	
//		Float_to_Byte(SpeedL_filt,c2h);		

//		Float_to_Byte(Left_Adc,c3h);	
//		Float_to_Byte(Left_M_Adc,c4h);
//		Float_to_Byte(LeftM_Adc,c5h);	
//		Float_to_Byte(Middle_Adc,c6h);	
//		Float_to_Byte(RightM_Adc,c7h);	
//		Float_to_Byte(Right_M_Adc,c8h);	
//		Float_to_Byte(Right_Adc,c9h);
//		Float_to_Byte(Eight_Adc,c10h);
//        Float_to_Byte(dl1b_distance_mm,c11h);
//        wireless_uart_send_buff(c1h,4);
//		wireless_uart_send_buff(c2h,4);
//		wireless_uart_send_buff(c3h,4);
//		wireless_uart_send_buff(c4h,4);
//		wireless_uart_send_buff(c5h,4);
//		wireless_uart_send_buff(c6h,4);
//		wireless_uart_send_buff(c7h,4);
// 		wireless_uart_send_buff(c8h,4); 
//		wireless_uart_send_buff(c9h,4);
//		wireless_uart_send_buff(c10h,4);
//		wireless_uart_send_buff(c11h,4);

//#endif   



//wireless_uart_send_buff(tail,4);
//	
//    
//    
//    
//    }
//}

//void vofaprint()
//{
//		//printf("samples:%d, %f, %f, %d, %d, %f, %f\r\n",test3,SpeedR_out,SpeedL_out,dutyR,dutyL,SpeedR*1.57,SpeedL*1.57);
//}


////逐飞库文件修改内容 防丢
////uint8 RxBuffer[1];//串口接收缓冲
////uint16 RxLine = 0;//指令长度
////uint8 DataBuff[200];//指令内容

////void wireless_uart_callback(void)
////{
////    //接到一个字节后单片机将会进入串口中断，通过在此处读取wireless_uart_data可以取走数据
////	wireless_uart_data = WIRELESS_DATA_BUF;
////    //fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data, 1);       // 存入 FIFO
////	RxLine++;       //每接收到一个数据，进入回调数据长度加1
////    DataBuff[RxLine-1]=wireless_uart_data;  //把每次接收到的数据保存到缓存数组
////    if(wireless_uart_data==0x21)            //接收结束标志位，这个数据可以自定义，根据实际需求，这里只做示例使用，不一定是0x21
////      {
////        Vofa_PID_Adjust();//数据解析和参数赋值函数
////        memset(DataBuff,0,sizeof(DataBuff));  //清空缓存数组
////        RxLine=0;  //清空接收长度
////      }
////        wireless_uart_data=0;
////}

//	

////float Get_Data(void)
////{
////    uint8 i;
////	uint8 data_Start_Num = 0; // 记录数据位开始的地方
////    uint8 data_End_Num = 0; // 记录数据位结束的地方
////    uint8 data_Num = 0; // 记录数据位数
////    uint8 minus_Flag = 0; // 判断是不是负数
////    float data_return = 0; // 解析得到的数据
////    
////	for(i=0;i<200;i++) // 查找等号和感叹号的位置
////    {
////        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1是直接定位到数据起始位
////        if(DataBuff[i] == '!')
////        {
////            data_End_Num = i - 1;
////            break;
////        }
////    }
////    if(DataBuff[data_Start_Num] == '-') // 如果是负数
////    {
////        data_Start_Num += 1; // 后移一位到数据位
////        minus_Flag = 1; // 负数flag
////    }
////    data_Num = data_End_Num - data_Start_Num + 1;
////    if(data_Num == 4) // 数据共4位   		0.00 小数点算一位
////    {
////        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
////                (DataBuff[data_Start_Num+3]-48)*0.01f;
////    }
////    else if(data_Num == 5) // 数据共5位  10.00
////    {
////        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
////                (DataBuff[data_Start_Num+4]-48)*0.01f;
////    }
////    else if(data_Num == 6) // 数据共6位 100.00
////    {
////        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
////                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
////    }
////    if(minus_Flag == 1)  data_return = -data_return;
////    return data_return;
////}

//	
//uint8 RxBuffer[1];//串口接收缓冲
//uint16 RxLine = 0;//指令长度
//uint8 DataBuff[200];//指令内容

////-------------------------------------------------------------------------------------------------------------------
////  @brief      无线转串口模块回调函数
////  @param      NULL
////  @return     void					
////  @since      v1.0
////  Sample usage:	
////  @note       
////-------------------------------------------------------------------------------------------------------------------
//void wireless_uart_callback(void)
//{
//    //接到一个字节后单片机将会进入串口中断，通过在此处读取wireless_uart_data可以取走数据
//	wireless_uart_data = WIRELESS_DATA_BUF;
//    //fifo_write_buffer(&wireless_uart_fifo, &wireless_uart_data, 1);       // 存入 FIFO
//	RxLine++;       //每接收到一个数据，进入回调数据长度加1
//    DataBuff[RxLine-1]=wireless_uart_data;  //把每次接收到的数据保存到缓存数组
//    if(wireless_uart_data==0x21)            //接收结束标志位，这个数据可以自定义，根据实际需求，这里只做示例使用，不一定是0x21
//      {
//        Vofa_PID_Adjust();//数据解析和参数赋值函数
//        memset(DataBuff,0,sizeof(DataBuff));  //清空缓存数组
//        RxLine=0;  //清空接收长度
//      }
//        wireless_uart_data=0;
//}


//float Get_Data(void)
//{
//    uint8 i;
//	uint8 data_Start_Num = 0; // 记录数据位开始的地方
//    uint8 data_End_Num = 0; // 记录数据位结束的地方
//    uint8 data_Num = 0; // 记录数据位数
//    uint8 minus_Flag = 0; // 判断是不是负数
//    float data_return = 0; // 解析得到的数据
//    
//	for(i=0;i<200;i++) // 查找等号和感叹号的位置
//    {
//        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1是直接定位到数据起始位
//        if(DataBuff[i] == '!')
//        {
//            data_End_Num = i - 1;
//            break;
//        }
//    }
//    if(DataBuff[data_Start_Num] == '-') // 如果是负数
//    {
//        data_Start_Num += 1; // 后移一位到数据位
//        minus_Flag = 1; // 负数flag
//    }
//    data_Num = data_End_Num - data_Start_Num + 1;
//    if(data_Num == 4) // 数据共4位   		0.00 小数点算一位
//    {
//        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
//                (DataBuff[data_Start_Num+3]-48)*0.01f;
//    }
//    else if(data_Num == 5) // 数据共5位  10.00
//    {
//        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
//                (DataBuff[data_Start_Num+4]-48)*0.01f;
//    }
//    else if(data_Num == 6) // 数据共6位 100.00
//    {
//        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
//                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
//    }
//    if(minus_Flag == 1)  data_return = -data_return;
//    return data_return;
//}


