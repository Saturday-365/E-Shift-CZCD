#ifndef __LED_H__
#define __LED_H__
	 
#include "stm32f10x.h"

//LED¶Ë¿Ú¶¨Òå
#define LED_RCC     RCC_APB2Periph_GPIOC                            
#define LED_PORT	  GPIOC    
#define LED         GPIO_Pin_13    

void LED_GPIO_Config(void);
void LED_ON(void);
void LED_OFF(void);		 				    
#endif
