/**********************************************************************************
 * ������  ������1Printfʵ��
 * ����    ��ͨ�������ض���ʵ����C��׼��ĺ���printf����������       
 * ʵ��ƽ̨��о����STM32���İ�
 * ��汾  ��ST_v3.5
 * ����    ������ά�����ܿƼ����޹�˾ 
 * 
 * �Ա�    ��https://robotcontrol.taobao.com/?spm=2013.1.w12075176-17259889750.5.7649a65aoumS5G&scene=taobao_shop
 *********************************************************************
 *��������˵����VCC --> ģ���3.3V��5V��Դ
 *						 GND --> ģ���GND
 *						
 *             PA9 --> ģ���RXD����
 *						 PA10--> ģ���TXD����
**********************************************************************************/

#include "stm32f10x.h"
#include "SysTick.h"
#include "usart.h"
#include "string.h"
#include "IO.h"
u8 CANSETUP[]={0X41,0X54,0x2b,0x43,0x47,0x0d,0x0a};//����AT+CG��������ģʽ
u8 CANModeCmdSet[] = "AT+CAN_MODE=0\r\n"; //��ģ��Ĺ���ģʽ����Ϊ ��������ģʽ
const char CANModeCmdSetAsk[] = "OK\r\n";
u8 CANEXIT[]={0X41,0X54,0x2b,0x45,0x54,0x0d,0x0a};//AT+ET�˳�����ģʽ
u8 CANModeCmdQuery[] = "AT+CAN_MODE=?\r\n"; //��ѯģ��Ĺ���ģʽ
const char CANModeCmdQueryAsk[] = "+CAN_MODE:0\r\n";

extern char Cmd[];
extern unsigned char Cmd_Index;


#define CFG_Pin  GPIO_Pin_8
#define CFG_GPIO GPIOA
#define CFG_CLK	 RCC_APB2Periph_GPIOA


/*******************************************************************************
* ������  : CFGPinInit
* ����    : ���ùܽ�
* ����    : ��
* ���    : ��
* ����ֵ  : ��// Header:
// File Name: // Header:
// File Name: |
// Author:
// Date:

// Author:
// Date:

* ˵��    : ��
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
* ������  : main
* ����    : ���������û������main������ʼ����
* ����    : ��
* ���    : ��
* ����ֵ  : int:����ֵΪһ��16λ������
* ˵��    : ��
*******************************************************************************/
int main(void)
{
	unsigned int i = 0x4ffff;
	USART1_Init_Config(9600);	//USART1��ʼ������
	SysTick_Init_Config();		//��ʼ��ϵͳ�δ�ʱ��SysTick
	CFGPinInit();
	LED_Init();
	
	GPIO_ResetBits(CFG_GPIO,CFG_Pin); //����ǰҪ����
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
				LED_ON();//���óɹ�
		
			Cmd_Index = 0;
			USART1_SendData(CANModeCmdQuery, sizeof(CANModeCmdQuery));
			i = 0x4ffff;
		
			while(i--);//��ʱ
		
			//if(!strcmp(Cmd,CANModeCmdQueryAsk))
		//	{ 
		//USART1_SendData(CANEXIT, sizeof(CANEXIT));
		//			LED_ON();//��ѯ�ɹ�
		//	}
	}
	else
	{
			//����ʧ��
			//����Ӵ������
	}
	
	
	GPIO_SetBits(CFG_GPIO,CFG_Pin); //�������Ҫ����
	
	//while (1)
	//{

	//}
}
