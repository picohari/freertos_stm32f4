/**
  ******************************************************************************
  * @file    ONEWIRE/UART_Printf/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    17-February-2017
  * @brief   Header for onewire.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
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
#ifndef __DS18B20_H
#define __DS18B20_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdbool.h>

#define DS18B20_MAX_SENSORS 		5

#define DS18B20_GPIO						GPIOI
#define DS18B20_PIN							GPIO_PIN_9

#define DS18B20_UPDATE_INTERVAL_MS	1000

#define DS18B20_CONVERT_TIMEOUT_MS	5000

//###################################################################################

typedef struct
{
	uint8_t  Address[8];
	float 	 Temperature;
	bool		 DataIsValid;	
	
}Ds18b20Sensor_t;

//###################################################################################

extern Ds18b20Sensor_t	ds18b20[DS18B20_MAX_SENSORS];

//###################################################################################
/* Every onewire chip has different ROM code, but all the same chips has same family code */
/* in case of DS18B20 this is 0x28 and this is first byte of ROM address */
#define DS18B20_FAMILY_CODE					0x28
#define DS18B20_CMD_ALARMSEARCH			0xEC

/* DS18B20 read temperature command */
#define DS18B20_CMD_CONVERTTEMP				  0x44 	/* Convert temperature */

#define DS18B20_DECIMAL_STEPS_12BIT			0.0625
#define DS18B20_DECIMAL_STEPS_11BIT			0.125
#define DS18B20_DECIMAL_STEPS_10BIT			0.25
#define DS18B20_DECIMAL_STEPS_9BIT			0.5

/* Bits locations for resolution */
#define DS18B20_RESOLUTION_R1				6
#define DS18B20_RESOLUTION_R0				5

/* CRC enabled */
#ifdef DS18B20_USE_CRC	
#define DS18B20_DATA_LEN					9
#else
#define DS18B20_DATA_LEN					2
#endif

//###################################################################################
typedef enum {
	DS18B20_Resolution_9bits  =  9, /*!< DS18B20  9 bits resolution */
	DS18B20_Resolution_10bits = 10, /*!< DS18B20 10 bits resolution */
	DS18B20_Resolution_11bits = 11, /*!< DS18B20 11 bits resolution */
	DS18B20_Resolution_12bits = 12  /*!< DS18B20 12 bits resolution */
} DS18B20_Resolution_t;

//###################################################################################
//void		Ds18b20_Init(osPriority Priority);

bool		Ds18b20_ManualConvert(void);

void 		OW_thread(void const * argument);

void 		OW_thread_test(void const * argument);

//###################################################################################
uint8_t DS18B20_Start(OneWire_t* OneWireStruct, uint8_t* ROM);
void 		DS18B20_StartAll(OneWire_t* OneWireStruct);
bool		DS18B20_Read(OneWire_t* OneWireStruct, uint8_t* ROM, float* destination);
uint8_t DS18B20_GetResolution(OneWire_t* OneWireStruct, uint8_t* ROM);
uint8_t DS18B20_SetResolution(OneWire_t* OneWireStruct, uint8_t* ROM, DS18B20_Resolution_t resolution);
uint8_t DS18B20_Is(uint8_t* ROM);
uint8_t DS18B20_SetAlarmHighTemperature(OneWire_t* OneWireStruct, uint8_t* ROM, int8_t temp);
uint8_t DS18B20_SetAlarmLowTemperature(OneWire_t* OneWireStruct, uint8_t* ROM, int8_t temp);
uint8_t DS18B20_DisableAlarmTemperature(OneWire_t* OneWireStruct, uint8_t* ROM);
uint8_t DS18B20_AlarmSearch(OneWire_t* OneWireStruct);
uint8_t DS18B20_AllDone(OneWire_t* OneWireStruct);
//###################################################################################

#endif /* _DS18B20_H */