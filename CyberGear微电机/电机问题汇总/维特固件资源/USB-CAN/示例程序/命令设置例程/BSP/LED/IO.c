#include "IO.h"
#include "stm32f10x_rcc.h"
struct SPin LED = {GPIOA, GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_2MHz};

void DIO_Initial(struct SPin stcPin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);  
	//GPIO_Remap_SWJ_JTAGDisable: PA15-JTDI PB3-JTDO PB4-JNTRST 
	//GPIO_Remap_SWJ_Disable: PA13-SWDIO PA14-SWCLK
	if (stcPin.GPIOx == GPIOB)
	{
		if ((stcPin.GPIO_Pin == GPIO_Pin_3)|(stcPin.GPIO_Pin == GPIO_Pin_4))
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}
	if (stcPin.GPIOx == GPIOA)
	{
		if (stcPin.GPIO_Pin == GPIO_Pin_15)
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	}	
	if (stcPin.GPIOx == GPIOA)
	{
		if ((stcPin.GPIO_Pin == GPIO_Pin_13)|(stcPin.GPIO_Pin == GPIO_Pin_14))
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	}	
	
	GPIO_StructInit(&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Mode = stcPin.GPIO_Mode;
	GPIO_InitStructure.GPIO_Speed = stcPin.GPIO_Speed;
	GPIO_InitStructure.GPIO_Pin =  stcPin.GPIO_Pin;
	GPIO_Init(stcPin.GPIOx, &GPIO_InitStructure);	
}	

void IOSleep()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);  
	
	GPIO_StructInit(&GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_All;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
}	

void WriteDO(struct SPin stcPin,unsigned char ucData)
{
	if (ucData>0)
		stcPin.GPIOx->BSRR = stcPin.GPIO_Pin;
	else
		stcPin.GPIOx->BRR = stcPin.GPIO_Pin;
}
unsigned char ReadDI(struct SPin stcPin)
{
	return (stcPin.GPIOx->IDR&stcPin.GPIO_Pin)==stcPin.GPIO_Pin;
}

/****************************************************
函数功能：LED初始化
输入参数：无
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
unsigned char ucLEDInitial=0;
void LED_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	DIO_Initial(LED);
	ucLEDInitial=1;
}

/****************************************************
函数功能：LED开
输入参数：无
输出参数：无
****************************************************/
void LED_ON(void)
{
	if (ucLEDInitial==0) LED_Init();
	WriteDO(LED,1);	
}

/****************************************************
函数功能：LED关
输入参数：无
输出参数：无
****************************************************/
void LED_OFF(void)
{
	if (ucLEDInitial==0) LED_Init();
	WriteDO(LED,0);	
}

void LED_REVERSE(void)
{
	if (ucLEDInitial==0) LED_Init();
	if (ReadDI(LED))	
		WriteDO(LED,0);
	else
		WriteDO(LED,1);
}

void LED_STATE(unsigned char State)
{
	if(State==1) LED_ON();
	else LED_OFF();
	
}