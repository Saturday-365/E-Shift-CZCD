#include "SA_Delay.h"
 
uint32_t fac_us = 72;
/*==================延时标定函数====================*/
/*			参数uint8_t SYSCLK是系统时钟				*/
/*==================================================*/
void My_Delay_us_init(uint8_t SYSCLK)	
{
    fac_us=SYSCLK; 
}
 
/*==================纳秒级别的Delay==================*/
void My_Delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;
    ticks=nus*fac_us; 
    told=SysTick->VAL; 
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break; 
        }
    };
}
 
/*==================毫秒级别的Delay==================*/
void My_Delay_ms(uint32_t nms)
{
	while(nms--)
	{
		My_Delay_us(1000);
	}
}
