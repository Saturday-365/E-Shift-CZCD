/**********************************************************************************
 * 文件名  ：led.c
 * 描述    ：led 应用函数库         
 * 实验平台：芯物联STM32核心板
 * 硬件连接：PC13 -> LED   
 * 库版本  ：ST_v3.5
**********************************************************************************/

#include "Led.h"
	   
/*******************************************************************************
* 函数名  : LED_GPIO_Config
* 描述    : LED IO配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : LED的IO口是:PC13
*******************************************************************************/
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;							//定义一个GPIO_InitTypeDef类型的GPIO初始化结构体
	
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);					//使能GPIOB的外设时钟	
	
	GPIO_InitStructure.GPIO_Pin = LED;				    		//选择要初始化的GPIOC引脚(PC13)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//设置引脚工作模式为通用推挽输出 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化GPIOC中的PBC13引脚

	LED_OFF();																				//关闭LED  					 
}

/*点亮LED.PC13*/
void LED_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED);
}

/*关闭LED.PC13*/
void LED_OFF(void)
{
	GPIO_ResetBits(LED_PORT,LED);
}

