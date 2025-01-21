#include "SA_Usart.h"
#include "usart.h"
#include "dma.h"
#include "ATK_UART.h"
#include "stdio.h"
#include "main.h"


uint8_t	RxBuffer_1[LENGTH];   //接受缓冲区 
uint8_t RxFlag_1 = 0;       //接收完成标志；0表示接受未完成，1表示接收完成

/*======================printf重定义=====================*/
/*-----------------注意要Include"stdio.h"--------------*/
int fputc(int ch, FILE *f)
{
 uint8_t temp[1] = {ch};
 HAL_UART_Transmit(&huart1, temp, 1, 2);
 return ch;
}
/*-----------------注意打开USE MicroLIB-----------------*/


void ATK_Uart_Init(void)
{
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)RxBuffer_1,LENGTH);
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  //串口接收中断回调函数
{
	if(huart->Instance == USART2)   //判断发生接收中断的串口
	{
		   //置为接收完成标志
        if (IMU_uart_callback_BYTE(RxBuffer_1)) RxFlag_1=1;        
       else RxFlag_1=0;
//test code
//        if (IMU_uart_callback_BYTE(RxBuffer_1)) printf("ok \n");        
//       else printf("no ");
//        printf("%s \n",RxBuffer_1);
//test code
        HAL_UART_Receive_DMA(&huart2, (uint8_t *)RxBuffer_1,LENGTH);//DMA使能接收中断  这个必须添加，否则不能再使用DMA进行发送接受
	}
//    else RxFlag_1=0;
}

uint8_t Report_stage(void)
{
    return RxFlag_1;

}
