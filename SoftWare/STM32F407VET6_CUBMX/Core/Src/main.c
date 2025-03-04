/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "headfile.h"    //头文
#include "SA_E_shift.h"    //头文

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//float a;
//float b;
uint16_t Motor_speed=0,Motor_dir=0;
uint8_t Eshift_up_flag=0,Eshift_dw_flag=0;
uint8_t key_num,gus_gear=0;

float set111,angle=0;
#define Clutch_pos -75
#define UPshift_pos 38
#define DOWNshift_pos  -38
 //5-6 37
 //4-5
#define Clutch_speed 100
#define Clutch_tor 11.9
#define Shift_speed 100
#define Shift_tor 11.9

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_Delay(2000);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
//    HAL_TIM_Base_Start_IT(&htim2);//定时器5ms中断开启
//   while (Start_ready(&Clutch_Cyber,&Shift_Cyber))
//  {       
    Init_Cyber(&Clutch_Cyber, 0x02);   //初始化电机参数
    Init_Cyber(&Shift_Cyber, 0x01);
    Init_MOTO_CAN();    //初始化can滤波器
    Stop_Cyber(&Clutch_Cyber, 1);
    HAL_Delay(20);
    Stop_Cyber(&Shift_Cyber, 1);     //停止电机
    HAL_Delay(20);
    Set_Cyber_Mode(&Clutch_Cyber,1);    //设置电机模式
    HAL_Delay(20);
    Set_Cyber_Mode(&Shift_Cyber,1);
    HAL_Delay(20); 
    Set_Cyber_ZeroPos(&Clutch_Cyber);
    HAL_Delay(20);
    Set_Cyber_ZeroPos(&Shift_Cyber);    //设置当前位置为0位
    HAL_Delay(20);
    Start_Cyber(&Shift_Cyber);     
    HAL_Delay(20);
    Start_Cyber(&Clutch_Cyber);
    HAL_Delay(20);     
    Set_Cyber_Pos(&Clutch_Cyber,0) ;   //设置电机位置
    HAL_Delay(20);
    Set_Cyber_Pos(&Shift_Cyber,0) ;
    HAL_Delay(20);
    Set_Cyber_limitSp(&Clutch_Cyber,Clutch_speed) ;
    HAL_Delay(20);
    Set_Cyber_limitSp(&Shift_Cyber,Shift_speed) ;   
    Set_Cyber_limitTor(&Clutch_Cyber,Clutch_tor) ;
    HAL_Delay(20);
    Set_Cyber_limitTor(&Shift_Cyber,Shift_tor) ;
    
//    }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      CANtest(&Clutch_Cyber,&Shift_Cyber);       
      Set_Cyber_Pos(&Clutch_Cyber,0) ;
      Set_Cyber_Pos(&Shift_Cyber,0) ;
      key_num=get_key_num();
      if (key_num==4) {Stop_Cyber(&Clutch_Cyber, 1);}
      if (key_num==3) {Start_Cyber(&Clutch_Cyber);}
      if (key_num==1) //upshiftsign
          {
              Eshift_up_flag=1;
              while(Eshift_up_flag)
                {
                     CANtest(&Clutch_Cyber,&Shift_Cyber);       
                    Set_Cyber_Pos(&Clutch_Cyber,Clutch_pos);
                    UPSHIFT_flag(1);
                    while(pre_pos_ready(&Clutch_Cyber,Clutch_pos,30))//等待离合拉到指定位置-提前量  
                    {
                        CANtest(&Clutch_Cyber,&Shift_Cyber); 
                        Set_Cyber_Pos(&Shift_Cyber,UPshift_pos);
                            while(pre_pos_ready(&Shift_Cyber,UPshift_pos,0))//等待离合拉到指定位置-提前量  
                        {
                            CANtest(&Clutch_Cyber,&Shift_Cyber); 
                            Set_Cyber_Pos(&Shift_Cyber,0);
                            UPSHIFT_flag(0);
                            Eshift_up_flag=0;
                            break;
                        }
                        break;
                    }
                }
//              Eshift_up_flag=1;
//              while(Eshift_up_flag)
//                {
//                        CANtest(&Clutch_Cyber,&Shift_Cyber); 
//                        Set_Cyber_Pos(&Shift_Cyber,UPshift_pos);
//                            while(pre_pos_ready(&Shift_Cyber,UPshift_pos,0))//等待离合拉到指定位置-提前量  
//                        {
//                            CANtest(&Clutch_Cyber,&Shift_Cyber); 
//                            Set_Cyber_Pos(&Shift_Cyber,0);
//                            UPSHIFT_flag(0);
//                            Eshift_up_flag=0;
//                            break;
//                        }
//                        break;
//                }

           }
      if (key_num==2) 
            {
              Eshift_dw_flag=1;
              while(Eshift_dw_flag)
                {
                    Set_Cyber_Pos(&Clutch_Cyber,Clutch_pos);
                    DOWNSHIFT_flag(1);
                    while(pre_pos_ready(&Clutch_Cyber,Clutch_pos,30))//等待离合拉到指定位置-提前量  
                    {
                        Set_Cyber_Pos(&Shift_Cyber,DOWNshift_pos);
                            while(pre_pos_ready(&Shift_Cyber,DOWNshift_pos,1))//等待离合拉到指定位置-提前量  
                        {
                            Set_Cyber_Pos(&Shift_Cyber,0);
                            DOWNSHIFT_flag(0);
                            Eshift_dw_flag=0;
                            break;
                        }
                        break;
                    }
                }
                      
  }

    HAL_GPIO_WritePin((GPIO_TypeDef *)LED_GPIO_Port, (uint16_t)LED_Pin, (GPIO_PinState)0);


//    printf("samples: %f \n",angle);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//    if(htim == &htim2)  //判断中断是否来自于定时器2
//   {      
//   }
//}
                        

//单片机测试代码
//    HAL_GPIO_WritePin((GPIO_TypeDef *)LED1_GPIO_Port, (uint16_t)LED1_Pin, (GPIO_PinState)0); 
//    HAL_Delay(200);
//    HAL_GPIO_WritePin((GPIO_TypeDef *)LED1_GPIO_Port, (uint16_t)LED1_Pin, (GPIO_PinState)1);
//    HAL_Delay(200);
//    printf("samples: %d \n",a );
//    a++;
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
