/**********************************************************************************
 * �ļ���  ��usart.c
 * ����    ��usart1 �ض���Ӧ�ú�����          
 * ʵ��ƽ̨��о����STM32���İ�
 * Ӳ�����ӣ�TXD(PA9)  -> �ⲿ����RXD     
 *           RXD(PA10) -> �ⲿ����TXD      
 *           GND	   -> �ⲿ����GND 
 * ��汾  ��ST_v3.5
**********************************************************************************/

#include "usart.h"	

char Cmd[30];
unsigned char Cmd_Index = 0;


void ClearCmd(void)
{
	unsigned char i = 0;
	for(i=0;i<30;i++)
		Cmd[i] = 0;
	
	Cmd_Index = 0;
}
/*******************************************************************************
* ������  : USART1_Init_Config
* ����    : USART1��ʼ������
* ����    : bound��������(���ã�2400��4800��9600��19200��38400��115200��)
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART1_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/*ʹ��USART1��GPIOA����ʱ��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*��λ����1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO��ʼ������*/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//USART1_TXD(PA.9)     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//������������������Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9)  


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  

	/*Usart1 NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 	
	

   /*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate = bound;										//���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//����ģʽ����Ϊ�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

	//USART_Cmd(USART1, ENABLE);                    			//ʹ�ܴ��� 
	USART_ClearFlag(USART1, USART_FLAG_TC);					//���������ɱ�־
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(USART1, ENABLE);
}


/*******************************************************************************
* ������  : USART_SendData
* ����    : USART1�������ݻ���������
* ����    : *buff�����ݻ�����ָ�룬len���������ݳ���
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void USART1_SendData(u8* buff, u16 len)
{    
	u16 i;

	for(i=0; i<len; i++)  
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
	    USART_SendData(USART1 ,buff[i]);   
	}
}



/*******************************************************************************
* ������  : USART1_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 1)��ֻ������USART1�жϽ��գ�δ����USART1�жϷ��͡�
*           2)�����յ�0x0d 0x0a(�س���"\r\n")����֡���ݽ������
*******************************************************************************/
void USART1_IRQHandler(void)                	
{
	
	u8 Res=0;
//	static unsigned char r_cnt = 0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1); //��ȡ���յ�������(USART1->DR)
		Cmd[Cmd_Index++] = Res;

	} 
}

/***********************************************************************/
/*************************----- �����ض��� -----************************/
/***********************************************************************/
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 

/*******************************************************************************
* ������  : fputc
* ����    : �ض���putc����
* ����    : ��
* ����    : �� 
* ˵��    : �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*******************************************************************************/
int fputc(int ch, FILE *f)
{
 /* e.g. ��USARTдһ���ַ� */
 USART_SendData(USART1, (uint8_t) ch);

 /* ѭ��ֱ��������� */
 while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

 return ch;
}

/*******************************************************************************
* ������  : fgetc
* ����    : �ض���getc����
* ����    : ��
* ����    : �� 
* ˵��    : �ض���getc��������������ʹ��scanff�����Ӵ���1��������
*******************************************************************************/
int fgetc(FILE *f)
{
 /* �ȴ�����1�������� */
 while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

 return (int)USART_ReceiveData(USART1);
}	

