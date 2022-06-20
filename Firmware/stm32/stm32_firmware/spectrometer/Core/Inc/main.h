/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define ONBOARD_LED_Pin GPIO_PIN_13
#define ONBOARD_LED_GPIO_Port GPIOC
#define SPEC_VIDEO_Pin GPIO_PIN_1
#define SPEC_VIDEO_GPIO_Port GPIOA
#define SPEC_CLK_Pin GPIO_PIN_4
#define SPEC_CLK_GPIO_Port GPIOA
#define SPEC_LED_Pin GPIO_PIN_5
#define SPEC_LED_GPIO_Port GPIOA
#define SPEC_START_Pin GPIO_PIN_7
#define SPEC_START_GPIO_Port GPIOA
#define OK_BTN_Pin GPIO_PIN_1
#define OK_BTN_GPIO_Port GPIOB
#define OK_BTN_EXTI_IRQn EXTI1_IRQn
#define DOWN_BTN_Pin GPIO_PIN_12
#define DOWN_BTN_GPIO_Port GPIOB
#define DOWN_BTN_EXTI_IRQn EXTI15_10_IRQn
#define UP_BTN_Pin GPIO_PIN_13
#define UP_BTN_GPIO_Port GPIOB
#define UP_BTN_EXTI_IRQn EXTI15_10_IRQn
#define TFT_DC_Pin GPIO_PIN_8
#define TFT_DC_GPIO_Port GPIOA
#define TFT_RST_Pin GPIO_PIN_9
#define TFT_RST_GPIO_Port GPIOA
#define SPI2_CS_Pin GPIO_PIN_10
#define SPI2_CS_GPIO_Port GPIOA
#define MENU_BTN_Pin GPIO_PIN_8
#define MENU_BTN_GPIO_Port GPIOB
#define MENU_BTN_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
