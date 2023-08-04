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
#define key_pause_Pin GPIO_PIN_7
#define key_pause_GPIO_Port GPIOA
#define key_switch_Pin GPIO_PIN_4
#define key_switch_GPIO_Port GPIOC
#define key_OK_Pin GPIO_PIN_5
#define key_OK_GPIO_Port GPIOC
#define key_reset_Pin GPIO_PIN_10
#define key_reset_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define adjust_down_Pin GPIO_PIN_11
#define adjust_down_GPIO_Port GPIOA
#define adjust_up_Pin GPIO_PIN_12
#define adjust_up_GPIO_Port GPIOA
#define adjust_right_Pin GPIO_PIN_10
#define adjust_right_GPIO_Port GPIOC
#define adjust_left_Pin GPIO_PIN_11
#define adjust_left_GPIO_Port GPIOC
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* 定时器及其通道马甲及定时时间间隔说明 BEGIN */
// motor 用 timer3
#define htim_motor htim3
#define TIM_CHANNEL_MOTOR_X TIM_CHANNEL_3
#define TIM_CHANNEL_MOTOR_Y TIM_CHANNEL_4
#define TIM_MOTOR_INTERVAL 20u // 单位：ms

// task 用 timer6
#define htim_task htim6
#define TIM_TASK_INTERVAL 25u // 单位：ms

// reset 用 timer7
#define htim_reset htim7
#define TIM_RESET_INTERVAL 36u // 单位：ms
/* 定时器及其通道马甲及定时时间间隔说明 END */

/* 串口马甲 BEGIN */
#define huart_with_k210 huart2
/* 串口马甲 END */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
