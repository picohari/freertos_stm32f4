/**
  ******************************************************************************
  * @file    src/config/config.c 
  * @author  Harald W. Leschner (DK6YF) - H9 Laboratory Ltd.
  * @version V1.0.0
  * @date    15-July-2017
  * @brief   Main configuration file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


/**
* The STM32 factory-programmed UUID memory.
* Three values of 32 bits each starting at this address
* Use like this: STM32_UUID[0], STM32_UUID[1], STM32_UUID[2]
*/

#define STM32_UUID         ((uint32_t *)0x1FFF7A10)       // F4
//#define STM32_UUID 		     ((uint32_t *)0x1FF0F420)     // F7

#define STM32_UUID_FLASH   ((uint32_t *)0x1FFF7A22)       // F4
//#define STM32_UUID_FLASH	 ((uint32_t *)0x1FF0F442)     // F7

#define STM32_UUID_PACK		 ((uint32_t *)0x1FFF7BF0)       // chip case form


#define VERSION_STRING VERSION
#define VERSION_STRING_LONG VERSION " built on " __DATE__ " " __TIME__

/**
* The IP Adresses of the system 
*/

#define HOME_NET

#ifdef HOME_NET
  #define IP_ADDR_HOME_NET    1
  #define IP_ADDR_HOME_GW     1
#else /* my office network has different settings... */
  #define IP_ADDR_HOME_NET    200
  #define IP_ADDR_HOME_GW     254
#endif

/* Static IP Address */
#define IP_ADDR0        (uint8_t) 192
#define IP_ADDR1        (uint8_t) 168
#define IP_ADDR2        (uint8_t) IP_ADDR_HOME_NET
#define IP_ADDR3        (uint8_t) 164

/* Gateway IP Address */
#define GW_ADDR0        (uint8_t) 192
#define GW_ADDR1        (uint8_t) 168
#define GW_ADDR2        (uint8_t) IP_ADDR_HOME_NET
#define GW_ADDR3        (uint8_t) IP_ADDR_HOME_GW
   
/* Netmask */
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0






/**
* The default error handler 
*/
extern void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
