/**********************************************************************************
 * 工程名  ：串口1Printf实验
 * 描述    ：通过串口重定向，实现用C标准库的函数printf来发送数据       
 * 实验平台：芯物联STM32核心板
 * 库版本  ：ST_v3.5
 * 作者    ：深圳维特智能科技有限公司 
 * 
 * 淘宝    ：https://robotcontrol.taobao.com/?spm=2013.1.w12075176-17259889750.5.7649a65aoumS5G&scene=taobao_shop
 *********************************************************************
 *引脚连接说明：VCC --> 模块的3.3V或5V电源
 *						 GND --> 模块的GND
 *						
 *             PA9 --> 模块的RXD引脚
 *						 PA10--> 模块的TXD引脚
**********************************************************************************/

#include "stm32f10x.h"
#include "SysTick.h"
#include "usart.h"
#include "string.h"
#include "IO.h"
u8 CANSETUP[]={0X41,0X54,0x2b,0x43,0x47,0x0d,0x0a};//发送AT+CG进入设置模式
u8 CANModeCmdSet[] = "AT+CAN_MODE=0\r\n"; //将模块的工作模式设置为 正常工作模式
const char CANModeCmdSetAsk[] = "OK\r\n";
u8 CANEXIT[]={0X41,0X54,0x2b,0x45,0x54,0x0d,0x0a};//AT+ET退出设置模式
u8 CANModeCmdQuery[] = "AT+CAN_MODE=?\r\n"; //查询模块的工作模式
const char CANModeCmdQueryAsk[] = "+CAN_MODE:0\r\n";

extern char Cmd[];
extern unsigned char Cmd_Index;


#define CFG_Pin  GPIO_Pin_8
#define CFG_GPIO GPIOA
#define CFG_CLK	 RCC_APB2Periph_GPIOA


/*******************************************************************************
* 函数名  : CFGPinInit
* 描述    : 配置管脚
* 输入    : 无
* 输出    : 无
* 返回值  : 无// Header:
// File Name: // Header:
// File Name: |
// Author:
// Date:

// Author:
// Date:

* 说明    : 无
*******************************************************************************/
void CFGPinInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(CFG_CLK, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = CFG_Pin;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		
	GPIO_Init(CFG_GPIO, &GPIO_InitStructure);					
}




/*******************************************************************************
* 函数名  : main
* 描述    : 主函数，用户程序从main函数开始运行
* 输入    : 无
* 输出    : 无
* 返回值  : int:返回值为一个16位整形数
* 说明    : 无
*******************************************************************************/
int main(void)
{
	unsigned int i = 0x4ffff;
	USART1_Init_Config(9600);	//USART1初始化配置
	SysTick_Init_Config();		//初始化系统滴答时钟SysTick
	CFGPinInit();
	LED_Init();
	
	GPIO_ResetBits(CFG_GPIO,CFG_Pin); //配置前要拉低
	LED_OFF();
	ClearCmd();
	Delay_nMs(1000);
	USART1_SendData(CANSETUP, sizeof(CANSETUP));

	//ClearCmd();
	//USART1_SendData(CANModeCmdSet, sizeof(CANModeCmdSet));
	i = 0x4ffff;
		
	while(i--);

	if(!strcmp(Cmd,CANModeCmdSetAsk))
	{
				LED_ON();//设置成功
		
			Cmd_Index = 0;
			USART1_SendData(CANModeCmdQuery, sizeof(CANModeCmdQuery));
			i = 0x4ffff;
		
			while(i--);//延时
		
			//if(!strcmp(Cmd,CANModeCmdQueryAsk))
		//	{ 
		//USART1_SendData(CANEXIT, sizeof(CANEXIT));
		//			LED_ON();//查询成功
		//	}
	}
	else
	{
			//设置失败
			//可添加处理代码
	}
	
	
	GPIO_SetBits(CFG_GPIO,CFG_Pin); //配置完成要拉高
	
	//while (1)
	//{

	//}
}
