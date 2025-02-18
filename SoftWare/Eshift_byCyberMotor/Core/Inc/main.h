/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define SWITCH1_Pin GPIO_PIN_0
#define SWITCH1_GPIO_Port GPIOC
#define BREAK_ADC_Pin GPIO_PIN_2
#define BREAK_ADC_GPIO_Port GPIOC
#define UPSHIFTSIG_OUT_Pin GPIO_PIN_3
#define UPSHIFTSIG_OUT_GPIO_Port GPIOC
#define IOOUT1_Pin GPIO_PIN_6
#define IOOUT1_GPIO_Port GPIOA
#define IOOUT2_Pin GPIO_PIN_7
#define IOOUT2_GPIO_Port GPIOA
#define SWITCH2_Pin GPIO_PIN_4
#define SWITCH2_GPIO_Port GPIOC
#define DOWNSHIFTSIG_OUT_Pin GPIO_PIN_5
#define DOWNSHIFTSIG_OUT_GPIO_Port GPIOC
#define My_I2C_SCL2_Pin GPIO_PIN_10
#define My_I2C_SCL2_GPIO_Port GPIOB
#define My_I2C_SDA2_Pin GPIO_PIN_11
#define My_I2C_SDA2_GPIO_Port GPIOB
#define MOTOR_PH_Pin GPIO_PIN_8
#define MOTOR_PH_GPIO_Port GPIOA
#define UPSHIFTSIG_IN_Pin GPIO_PIN_12
#define UPSHIFTSIG_IN_GPIO_Port GPIOC
#define DOWNSHIFTSIG_IN_Pin GPIO_PIN_6
#define DOWNSHIFTSIG_IN_GPIO_Port GPIOB
#define My_I2C_SCL_Pin GPIO_PIN_8
#define My_I2C_SCL_GPIO_Port GPIOB
#define My_I2C_SDA_Pin GPIO_PIN_9
#define My_I2C_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
