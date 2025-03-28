/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint16_t overtime_tick;

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SWITCH5_Pin GPIO_PIN_4
#define SWITCH5_GPIO_Port GPIOA
#define SWITCH3_Pin GPIO_PIN_4
#define SWITCH3_GPIO_Port GPIOC
#define SWITCH2_Pin GPIO_PIN_5
#define SWITCH2_GPIO_Port GPIOC
#define SWITCH1_Pin GPIO_PIN_0
#define SWITCH1_GPIO_Port GPIOB
#define DOWNSHIFTSIG_IN_Pin GPIO_PIN_1
#define DOWNSHIFTSIG_IN_GPIO_Port GPIOB
#define UPSHIFTSIG_IN_Pin GPIO_PIN_9
#define UPSHIFTSIG_IN_GPIO_Port GPIOE
#define DATA_CAN2_RX_Pin GPIO_PIN_12
#define DATA_CAN2_RX_GPIO_Port GPIOB
#define DATA_CAN2_TX_Pin GPIO_PIN_13
#define DATA_CAN2_TX_GPIO_Port GPIOB
#define IOOUT1_Pin GPIO_PIN_11
#define IOOUT1_GPIO_Port GPIOD
#define IOOUT2_Pin GPIO_PIN_12
#define IOOUT2_GPIO_Port GPIOD
#define DIO1_Pin GPIO_PIN_6
#define DIO1_GPIO_Port GPIOC
#define DIO2_Pin GPIO_PIN_7
#define DIO2_GPIO_Port GPIOC
#define MOTO_CAN1_RX_Pin GPIO_PIN_11
#define MOTO_CAN1_RX_GPIO_Port GPIOA
#define MOTO_CAN1_TX_Pin GPIO_PIN_12
#define MOTO_CAN1_TX_GPIO_Port GPIOA
#define UPSHIFTSIG_OUT_Pin GPIO_PIN_4
#define UPSHIFTSIG_OUT_GPIO_Port GPIOB
#define DOWNSHIFTSIG_OUT_Pin GPIO_PIN_5
#define DOWNSHIFTSIG_OUT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
