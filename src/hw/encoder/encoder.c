/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    encoder.c
  * @brief   This file provides code for the configuration
  *          of the TIM3 instances for rotary encoder.
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
#include "encoder.h"
#include "config.h"

#include "uart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

// Encoder constants
#define ENCODER_MAX_COUNT 65535
#define ENCODER_MID_COUNT (ENCODER_MAX_COUNT / 2)
#define ENCODER_SCALE_DIV 2.0f   // adjust for 2 counts per detent (use 4.0f for x4 mode)

// Static state variables
static uint16_t last_count = ENCODER_MID_COUNT;
static float jog_position = 0.0f;
static float jog_delta = 0.0f;  // last movement since last update


/* TIM3 init function */
void Encoder_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance                = TIM3;
  htim3.Init.Prescaler          = 0;
  htim3.Init.CounterMode        = TIM_COUNTERMODE_UP;
  htim3.Init.Period             = 65535;
  htim3.Init.ClockDivision      = TIM_CLOCKDIVISION_DIV1;
  //htim3.Init.AutoReloadPreload  = TIM_AUTORELOAD_PRELOAD_DISABLE;

  sConfig.EncoderMode           = TIM_ENCODERMODE_TI1;

  sConfig.IC1Polarity           = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection          = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler          = TIM_ICPSC_DIV1;
  sConfig.IC1Filter             = 10;
  
  sConfig.IC2Polarity           = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection          = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler          = TIM_ICPSC_DIV1;
  sConfig.IC2Filter             = 10;

  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* USER CODE BEGIN TIM3_Init 2 */
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
  last_count = ENCODER_MID_COUNT;
  jog_position = -16383.50;

  /* USER CODE END TIM3_Init 2 */

}

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(tim_encoderHandle->Instance == TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PC7     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin         = GPIO_PIN_6;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate   = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin         = GPIO_PIN_7;
    GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull        = GPIO_NOPULL;
    GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate   = GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* tim_encoderHandle)
{

  if(tim_encoderHandle->Instance == TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PC7     ------> TIM3_CH2
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);

  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}



/* USER CODE BEGIN 1 */

void Encoder_Update(void)
{
    uint16_t current_count = __HAL_TIM_GET_COUNTER(&htim3);

    // Compute signed delta with wraparound
    int16_t raw_delta = (int16_t)(current_count - last_count);
    last_count = current_count;

    // Scale to physical steps
    jog_delta = -((float)raw_delta / ENCODER_SCALE_DIV);

    // Integrate position
    jog_position += jog_delta;

    #if 0
    // Optional: debug print only when moved
    if (raw_delta != 0)
    {
        writef("Δ=%+d  JogPos=%.2f\n\r", raw_delta, jog_position);
    }
    #endif
}

float Encoder_GetPosition(void)
{
    return jog_position;
}

float Encoder_GetDelta(void)
{
    return jog_delta;
}

void Encoder_Reset(void)
{
    jog_position = 0.0f;
    jog_delta = 0.0f;
    __HAL_TIM_SET_COUNTER(&htim3, ENCODER_MID_COUNT);
    last_count = ENCODER_MID_COUNT;
}

/* USER CODE END 1 */
