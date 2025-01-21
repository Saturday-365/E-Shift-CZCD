/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "headfile.h"    //头文

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
FOCmotor_Typedef FOCM1,FOCM2,FOCM3;
AnglePD_Typedef PDM1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t message1[]="****UART commucition using IDLE IT DMA****\n";
uint8_t message2[]="Please enter 8 characters: \n";

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
float a;
float b;
float set111;
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
  AnglePD_Init(&PDM1);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
    HAL_SETPWM_init();   //初始化pwm通道
    FOCType_Init(&FOCM1,7,1,1);
    FOCType_Init(&FOCM2,7,1,2);
    FOCType_Init(&FOCM3,7,1,3); //初始化电机结构体参数
    EN_Motor();//使能电机
////    Motor_alignSensor(&FOCM1,&FOCM2,&FOCM3);
    FOCType_setfirst(&FOCM1); //结构体参数定义
    Motor_setTorque(&FOCM1, 3);  //起劲
    HAL_Delay(2000);
    Get_zeroangle(&FOCM1);
    HAL_Delay(100);
    Motor_setTorque(&FOCM1, 0);  //起劲   
    HAL_Delay(500);

//    Get_ALL_zeroangleTEST(&FOCM1,&FOCM2,&FOCM3);
    HAL_TIM_Base_Start_IT(&htim2);//定时器5ms中断开启
    
    ATK_Uart_Init();//使能DMA接收中断
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {    
        set111=Get_IMUData (&IMUDATA,3);    
//    set111=i2c_mt6701_get_angle1();
//      set111=set111/PI*180;
//       Get_angle(&FOCM1);       
//      set111=100;
      testdata(&FOCM1,&FOCM2,&FOCM3,set111);
//    IMUuartData(&IMUDATA);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//   M1_HAL_SETPWM(1,1,1);
//   M2_HAL_SETPWM(0.8,0.8,0.8);
//   M3_HAL_SETPWM(0.5,0.5,0.5);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim2)  //判断中断是否来自于定时器2
   {
    Get_angle(&FOCM1);      
//  Motor_setTorque(&FOCM1,Angle_loop_PD(&PDM1,&FOCM1,set111));    
    Motor_setTorque(&FOCM1,2);    
   }
}
                        

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
