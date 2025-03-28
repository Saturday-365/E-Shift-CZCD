#include "SA_Usart.h"
#include "usart.h"
#include "dma.h"
#include "ATK_UART.h"
#include "stdio.h"
#include "main.h"
#include "SA_pid.h"
#include "SA_Flash.h"
#include "common.h"

uint8_t	RxBuffer_1[LENGTH];   //接受缓冲区 
uint8_t RxFlag_1 = 0;       //接收完成标志；0表示接受未完成，1表示接收完成
uint8_t	RxBuffer_3[LENGTH];   //接受缓冲区 
uint8_t RxFlag_3 = 0;       //接收完成标志；0表示接受未完成，1表示接收完成

//VOFA 用
uint8 RxBuffer[1];//串口接收缓冲
uint16 RxLine = 0;//指令长度
uint8_t DataBuff[200];//指令内容

//extern SpeedPID_Typedef PIDM1;
float ALLspeed;


/*======================printf重定义=====================*/
/*-----------------注意要Include"stdio.h"--------------*/
int fputc(int ch, FILE *f)
{
 uint8_t temp[1] = {ch};
 HAL_UART_Transmit(&huart1, temp, 1, 2);
 return ch;
}
/*-----------------注意打开USE MicroLIB-----------------*/

////使能DMA接收中断
void RS232_Uart_Init(void)
{
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer_1,LENGTH);
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)RxBuffer_3,LENGTH);
}


void VOFA_Uart_Init(void)
{
//    HAL_UART_Receive_DMA(&huart1, (uint8_t *)DataBuff,LENGTH);
//    PID_P_VOFA(&PIDM1,uint6_cov_float(Store_Data[1]));
//    PID_I_VOFA(&PIDM1,uint6_cov_float(Store_Data[2]));
//    PID_D_VOFA(&PIDM1,uint6_cov_float(Store_Data[3]));
//    ALLspeed=uint6_cov_float(Store_Data[4]);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //串口接收中断回调函数
{
	if(huart->Instance == USART1)   //判断发生接收中断的串口
	{
		RxFlag_1=1;   //置为接收完成标志

        HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer_1,LENGTH);//DMA使能接收中断  这个必须添加，否则不能再使用DMA进行发送接受
	}
    else RxFlag_1=0;
    
    if(huart->Instance == USART3)   //判断发生接收中断的串口
	{
		   
        RxFlag_3=1;    //置为接收完成标志    

        HAL_UART_Receive_DMA(&huart3, (uint8_t *)RxBuffer_3,LENGTH);//DMA使能接收中断  这个必须添加，否则不能再使用DMA进行发送接受
	}
    else RxFlag_3=0;
}

uint8_t Report_stage(void)
{
    return RxFlag_1;

}



float Get_Data(void)
{
    uint8 i;
	uint8 data_Start_Num = 0; // 记录数据位开始的地方
    uint8 data_End_Num = 0; // 记录数据位结束的地方
    uint8 data_Num = 0; // 记录数据位数
    uint8 minus_Flag = 0; // 判断是不是负数
    float data_return = 0; // 解析得到的数据
    
	for(i=0;i<15;i++) // 查找等号和感叹号的位置
    {
        if(DataBuff[i] == '=') data_Start_Num = i + 1; // +1是直接定位到数据起始位
        if(DataBuff[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(DataBuff[data_Start_Num] == '-') // 如果是负数
    {
        data_Start_Num += 1; // 后移一位到数据位
        minus_Flag = 1; // 负数flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) // 数据共4位   		0.00 小数点算一位
    {
        data_return = (DataBuff[data_Start_Num]-48)  + (DataBuff[data_Start_Num+2]-48)*0.1f +
                (DataBuff[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) // 数据共5位  10.00
    {
        data_return = (DataBuff[data_Start_Num]-48)*10 + (DataBuff[data_Start_Num+1]-48) + (DataBuff[data_Start_Num+3]-48)*0.1f +
                (DataBuff[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) // 数据共6位 100.00
    {
        data_return = (DataBuff[data_Start_Num]-48)*100 + (DataBuff[data_Start_Num+1]-48)*10 + (DataBuff[data_Start_Num+2]-48) +
                (DataBuff[data_Start_Num+4]-48)*0.1f + (DataBuff[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
    return data_return;
}

uint16_t dataship;
uint8_t flage=0;
void Vofa_PID_Adjust()
{
    float data_Get = Get_Data(); // 存放接收到的数据
//	dataship=float_cov_uint16(data_Get);
	if(DataBuff[0]=='A')
    {
//        if (flage==1){STOP_Motor();flage =0;}
//        else {EN_Motor();flage =1;} 
    } 
    if(DataBuff[0]=='P') 
	{
		Store_Data[1]=dataship;
//        PID_P_VOFA(&PIDM1,data_Get);
//		vofa_test_1=func_limit_ab(vofa_test_1,-100,100);
        }
	else if(DataBuff[0]=='I' ) 
	{
		Store_Data[2]=dataship;
//        PID_I_VOFA(&PIDM1,data_Get);
//		vofa_test_2=func_limit_ab(vofa_test_2,-100,100);
        }       
    else if(DataBuff[0]=='D' ) 
	{
		Store_Data[3]=dataship;
//        PID_D_VOFA(&PIDM1,data_Get);
//		vofa_test_3=func_limit_ab(vofa_test_3,-100,100);
        }
	else if(DataBuff[0]=='S' ) 
	{
		Store_Data[4]=dataship;
        //func_limit_ab(data_Get,-600,600);
        ALLspeed=data_Get;
    }	
    Store_Save();
}

unsigned char c0h[4],c1h[4],c2h[4],c3h[4],c4h[4],c5h[4],c6h[4],c7h[4],c8h[4],c9h[4],c10h[4],
                        c11h[4],c12h[4],c13h[4],c14h[4],c15h[4],c16h[4];
unsigned char tail[4]={0x00, 0x00, 0x80, 0x7f};	//vofa输出帧尾

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

void Float_to_Byte_usart(float f,unsigned char byte[]) /*将浮点数f转化为4个字节数据存放在byte[4]中*/
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

void JustFloat_4(float data1,float data2,float data3,float data4)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    HAL_UART_Transmit(&huart1,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}
void JustFloat_5(float data1,float data2,float data3,float data4,float data5)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    HAL_UART_Transmit(&huart1,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c4h,sizeof(c4h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}
void JustFloat_8(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    Float_to_Byte_usart(data6,c5h);
    Float_to_Byte_usart(data7,c6h);
    Float_to_Byte_usart(data8,c7h);
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c0h,sizeof(c0h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c1h,sizeof(c1h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c2h,sizeof(c2h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c3h,sizeof(c3h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c4h,sizeof(c4h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c5h,sizeof(c5h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c6h,sizeof(c6h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)c7h,sizeof(c7h));
    HAL_UART_Transmit_DMA(&huart3,(uint8_t *)tail,sizeof(tail));
}

void JustFloat_16_rs232(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10,float data11,float data12,float data13,float data14,float data15,float data16)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    Float_to_Byte_usart(data6,c5h);
    Float_to_Byte_usart(data7,c6h);
    Float_to_Byte_usart(data8,c7h);
    Float_to_Byte_usart(data9,c8h);
    Float_to_Byte_usart(data10,c9h);
    Float_to_Byte_usart(data11,c10h);
    Float_to_Byte_usart(data12,c11h);
    Float_to_Byte_usart(data13,c12h);
    Float_to_Byte_usart(data14,c13h);
    Float_to_Byte_usart(data15,c14h);
    Float_to_Byte_usart(data16,c15h);
    HAL_UART_Transmit(&huart2,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c4h,sizeof(c4h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c5h,sizeof(c5h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c6h,sizeof(c6h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c7h,sizeof(c7h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c8h,sizeof(c8h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c9h,sizeof(c9h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c10h,sizeof(c10h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c11h,sizeof(c11h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c12h,sizeof(c12h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c13h,sizeof(c13h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c14h,sizeof(c14h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c15h,sizeof(c15h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}
void JustFloat_16(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10,float data11,float data12,float data13,float data14,float data15,float data16)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    Float_to_Byte_usart(data6,c5h);
    Float_to_Byte_usart(data7,c6h);
    Float_to_Byte_usart(data8,c7h);
    Float_to_Byte_usart(data9,c8h);
    Float_to_Byte_usart(data10,c9h);
    Float_to_Byte_usart(data11,c10h);
    Float_to_Byte_usart(data12,c11h);
    Float_to_Byte_usart(data13,c12h);
    Float_to_Byte_usart(data14,c13h);
    Float_to_Byte_usart(data15,c14h);
    Float_to_Byte_usart(data16,c15h);
    HAL_UART_Transmit(&huart1,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c4h,sizeof(c4h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c5h,sizeof(c5h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c6h,sizeof(c6h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c7h,sizeof(c7h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c8h,sizeof(c8h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c9h,sizeof(c9h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c10h,sizeof(c10h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c11h,sizeof(c11h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c12h,sizeof(c12h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c13h,sizeof(c13h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c14h,sizeof(c14h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c15h,sizeof(c15h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}


void JustFloat_10_rs232(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    Float_to_Byte_usart(data6,c5h);
    Float_to_Byte_usart(data7,c6h);
    Float_to_Byte_usart(data8,c7h);
    Float_to_Byte_usart(data9,c8h);
    Float_to_Byte_usart(data10,c9h);
    HAL_UART_Transmit(&huart2,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c4h,sizeof(c4h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c5h,sizeof(c5h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c6h,sizeof(c6h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c7h,sizeof(c7h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c8h,sizeof(c8h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)c9h,sizeof(c9h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}
void JustFloat_10(float data1,float data2,float data3,float data4,float data5,float data6,float data7,float data8,
    float data9,float data10)
{
    Float_to_Byte_usart(data1,c0h);
    Float_to_Byte_usart(data2,c1h);
    Float_to_Byte_usart(data3,c2h);
    Float_to_Byte_usart(data4,c3h);
    Float_to_Byte_usart(data5,c4h);
    Float_to_Byte_usart(data6,c5h);
    Float_to_Byte_usart(data7,c6h);
    Float_to_Byte_usart(data8,c7h);
    Float_to_Byte_usart(data9,c8h);
    Float_to_Byte_usart(data10,c9h);
    HAL_UART_Transmit(&huart1,(uint8_t *)c0h,sizeof(c0h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c1h,sizeof(c1h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c2h,sizeof(c2h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c3h,sizeof(c3h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c4h,sizeof(c4h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c5h,sizeof(c5h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c6h,sizeof(c6h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c7h,sizeof(c7h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c8h,sizeof(c8h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)c9h,sizeof(c9h),HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1,(uint8_t *)tail,sizeof(tail),HAL_MAX_DELAY);
}

//https://blog.csdn.net/qq_45945548/article/details/121160290
//在main函数内部，while循环外部添加以下代码

////发送提示信息
//uint8_t message1[]="****UART commucition using IDLE IT DMA****\n";
//uint8_t message2[]="Please enter 8 characters: \n";
//HAL_UART_Transmit(&huart1,(uint8_t *)message1,sizeof(message1),HAL_MAX_DELAY);
//HAL_UART_Transmit(&huart1,(uint8_t *)message2,sizeof(message2),HAL_MAX_DELAY);
////使能DMA接收中断
//HAL_UART_Receive_DMA(&huart1, (uint8_t *)RxBuffer,LENGTH);

//在while循环内部添加一下代码
//while (1)
//  {
//    /* USER CODE END WHILE */
//   
//    /* USER CODE BEGIN 3 */
//     //添加下面代码
//   HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"hello windows!\n", 15 );
//	HAL_Delay(1000);  //延时1s
//	if(RxFlag == 1)  //如果接受完成，不在发送数据
//	{
//			HAL_UART_Transmit_DMA(&huart1,(uint8_t *)"Recevie Success!\n",17);  //提示接受成功
//			break;  //退出循环，不在发送数据
//	}
//  }

// HAL_UART_Transmit_DMA(&huart1, (uint8_t *)"hello windows!\n", 15 );
