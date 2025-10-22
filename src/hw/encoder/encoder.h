/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    encoder.h
  * @brief   This file contains all the function prototypes for
  *          the encoder.c file
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
#ifndef __ENCODER_H__
#define __ENCODER_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void Encoder_Init(void);
void Encoder_Update(void);
float Encoder_GetPosition(void);
float Encoder_GetDelta(void);
void Encoder_Reset(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ENCODER_H__ */

