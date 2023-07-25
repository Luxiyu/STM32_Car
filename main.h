/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define PWMA_Pin GPIO_PIN_1
#define PWMA_GPIO_Port GPIOA
#define PWMB_Pin GPIO_PIN_2
#define PWMB_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_3
#define AIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_4
#define AIN2_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_5
#define BIN1_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_6
#define BIN2_GPIO_Port GPIOA
#define DO6_Pin GPIO_PIN_4
#define DO6_GPIO_Port GPIOB
#define DO6_EXTI_IRQn EXTI4_IRQn
#define DO1_Pin GPIO_PIN_5
#define DO1_GPIO_Port GPIOB
#define DO1_EXTI_IRQn EXTI9_5_IRQn
#define DO2_Pin GPIO_PIN_6
#define DO2_GPIO_Port GPIOB
#define DO2_EXTI_IRQn EXTI9_5_IRQn
#define DO3_Pin GPIO_PIN_7
#define DO3_GPIO_Port GPIOB
#define DO3_EXTI_IRQn EXTI9_5_IRQn
#define DO4_Pin GPIO_PIN_8
#define DO4_GPIO_Port GPIOB
#define DO4_EXTI_IRQn EXTI9_5_IRQn
#define DO5_Pin GPIO_PIN_9
#define DO5_GPIO_Port GPIOB
#define DO5_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
