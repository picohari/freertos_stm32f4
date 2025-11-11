/**
  ******************************************************************************
  * @file    stm324xg_eval.h
  * @author  MCD Application Team
  * @version V2.2.2
  * @date    22-April-2016
  * @brief   This file contains definitions for STM324xG_EVAL's LEDs, 
  *          push-buttons and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __XCORE407I_H
#define __XCORE407I_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   

/** @defgroup STM324xG_EVAL_LOW_LEVEL_Exported_Types STM324xG EVAL LOW LEVEL Exported Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
}Led_TypeDef;


/** @addtogroup STM324xG_EVAL_LOW_LEVEL_LED STM324xG EVAL LOW LEVEL LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_PIN_2
#define LED1_GPIO_PORT                   GPIOH
#define LED1_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          __GPIOH_CLK_DISABLE()
  
#define LED2_PIN                         GPIO_PIN_3
#define LED2_GPIO_PORT                   GPIOH
#define LED2_GPIO_CLK_ENABLE()           __GPIOH_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __GPIOH_CLK_DISABLE()
  
#define LED3_PIN                         GPIO_PIN_8
#define LED3_GPIO_PORT                   GPIOI
#define LED3_GPIO_CLK_ENABLE()           __GPIOI_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __GPIOI_CLK_DISABLE()
  
#define LED4_PIN                         GPIO_PIN_10
#define LED4_GPIO_PORT                   GPIOI
#define LED4_GPIO_CLK_ENABLE()           __GPIOI_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __GPIOI_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED1_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED2_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 2) LED3_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 3) LED4_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED1_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED2_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 2) LED3_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 3) LED4_GPIO_CLK_DISABLE(); \
                                            }while(0)


#define JOY_A_PIN                       GPIO_PIN_2
#define JOY_A_GPIO_PORT                 GPIOE
#define JOY_A_GPIO_CLK_ENABLE()         __GPIOE_CLK_ENABLE()
#define JOY_A_GPIO_CLK_DISABLE()        __GPIOE_CLK_DISABLE()

#define JOY_B_PIN                       GPIO_PIN_3
#define JOY_B_GPIO_PORT                 GPIOE
#define JOY_B_GPIO_CLK_ENABLE()         __GPIOE_CLK_ENABLE()
#define JOY_B_GPIO_CLK_DISABLE()        __GPIOE_CLK_DISABLE()

#define JOY_C_PIN                       GPIO_PIN_4
#define JOY_C_GPIO_PORT                 GPIOE
#define JOY_C_GPIO_CLK_ENABLE()         __GPIOE_CLK_ENABLE()
#define JOY_C_GPIO_CLK_DISABLE()        __GPIOE_CLK_DISABLE()

#define JOY_D_PIN                       GPIO_PIN_5
#define JOY_D_GPIO_PORT                 GPIOE
#define JOY_D_GPIO_CLK_ENABLE()         __GPIOE_CLK_ENABLE()
#define JOY_D_GPIO_CLK_DISABLE()        __GPIOE_CLK_DISABLE()

#define JOY_OK_PIN                       GPIO_PIN_6
#define JOY_OK_GPIO_PORT                 GPIOE
#define JOY_OK_GPIO_CLK_ENABLE()         __GPIOE_CLK_ENABLE()
#define JOY_OK_GPIO_CLK_DISABLE()        __GPIOE_CLK_DISABLE()

#define JOY_GPIO_PINS                   JOY_A_PIN | JOY_B_PIN | JOY_C_PIN | JOY_D_PIN | JOY_OK_PIN
#define JOY_GPIO_PORT                   GPIOE
#define JOY_GPIO_CLK_ENABLE()           __GPIOE_CLK_ENABLE()
#define JOY_GPIO_CLK_DISABLE()          __GPIOE_CLK_DISABLE()


/** @defgroup STM324xG_EVAL_LOW_LEVEL_Exported_Functions STM324xG EVAL LOW LEVEL Exported Functions
  * @{
  */
void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

void BSP_JoyStick_Init(void);

void BSP_LCD_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM324xG_EVAL_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
