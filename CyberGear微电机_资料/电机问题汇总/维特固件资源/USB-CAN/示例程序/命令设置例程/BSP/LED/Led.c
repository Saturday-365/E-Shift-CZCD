/**********************************************************************************
 * �ļ���  ��led.c
 * ����    ��led Ӧ�ú�����         
 * ʵ��ƽ̨��о����STM32���İ�
 * Ӳ�����ӣ�PC13 -> LED   
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "Led.h"
	   
/*******************************************************************************
* ������  : LED_GPIO_Config
* ����    : LED IO����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : LED��IO����:PC13
*******************************************************************************/
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;							//����һ��GPIO_InitTypeDef���͵�GPIO��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);					//ʹ��GPIOB������ʱ��	
	
	GPIO_InitStructure.GPIO_Pin = LED;				    		//ѡ��Ҫ��ʼ����GPIOC����(PC13)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//�������Ź���ģʽΪͨ��������� 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz
	GPIO_Init(LED_PORT, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��GPIOC�е�PBC13����

	LED_OFF();																				//�ر�LED  					 
}

/*����LED.PC13*/
void LED_ON(void) 
{
	GPIO_SetBits(LED_PORT,LED);
}

/*�ر�LED.PC13*/
void LED_OFF(void)
{
	GPIO_ResetBits(LED_PORT,LED);
}

