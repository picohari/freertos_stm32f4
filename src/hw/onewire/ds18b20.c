/**
  ******************************************************************************
  * @file    UART/UART_Printf/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    17-February-2017
  * @brief   HAL MSP module.    
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

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "XCore407I.h"

#include "stm32f4xx_it.h"

#include "onewire.h"
#include "ds18b20.h"

#include "uart.h"

#include <stdbool.h>
#include <stdarg.h>
#include <string.h>




//###################################################################################
Ds18b20Sensor_t	ds18b20[DS18B20_MAX_SENSORS];

OneWire_t OneWire;

uint8_t	  OneWireDevices;
uint8_t   TempSensorCount = 0; 
uint8_t	  Ds18b20StartConvert = 0;
uint16_t  Ds18b20Timeout = 0;

osThreadId 	Ds18b20Handle;



//###########################################################################################

/*
void Ds18b20_Init(osPriority Priority)
{
	osThreadDef(myTask_Ds18b20, Task_Ds18b20, Priority, 0, 128);

	Ds18b20Handle = osThreadCreate(osThread(myTask_Ds18b20), NULL);	
}
*/

bool Ds18b20_ManualConvert(void)
{
	Ds18b20StartConvert = 1;

	while (Ds18b20StartConvert == 1)
		osDelay(10);
	
	if (Ds18b20Timeout == 0)
		return false;
	else
		return true;	
}

void OW_thread_test(void const * argument)
{
	(void) argument;

	//ow_timer3_init();
	//ow_gpio_init();

	OneWire_Init(&OneWire, DS18B20_GPIO , DS18B20_PIN);
	ONEWIRE_OUTPUT(&OneWire);

	osDelay(500);

	while (1) {
		
		//HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_9);
		//ow_delay_us(100);

		ONEWIRE_HIGH(&OneWire);
		ow_delay_us(100);
		
		ONEWIRE_LOW(&OneWire);
		ow_delay_us(40);

	}

}


void OW_thread(void const * argument)
{
	(void) argument;

	uint8_t	Ds18b20TryToFind = DS18B20_MAX_SENSORS;
	
	osDelay(100);

	writef("OW: init\r\n");

	osDelay(10);

	do {
		OneWire_Init(&OneWire, DS18B20_GPIO , DS18B20_PIN);

		TempSensorCount = 0;	
		OneWireDevices = OneWire_First(&OneWire);

		while (OneWireDevices) {
			osDelay(100);
			TempSensorCount++;

			OneWire_GetFullROM(&OneWire, ds18b20[TempSensorCount-1].Address);
			OneWireDevices = OneWire_Next(&OneWire);
		}

		if (TempSensorCount > 0)
			break;

		Ds18b20TryToFind--;

	} while (Ds18b20TryToFind > 0);

	if (Ds18b20TryToFind == 0) {
		writef("OW: no devices found!\r\n");
		vTaskDelete(Ds18b20Handle);
	}

	writef("OW: %d device(s) found!\r\n", TempSensorCount);

	uint8_t i = 0;

	for (i = 0; i < TempSensorCount; i++) {
		
		osDelay(10);

    	DS18B20_SetResolution(&OneWire, ds18b20[i].Address, DS18B20_Resolution_12bits);

		osDelay(10);
    	
    	DS18B20_DisableAlarmTemperature(&OneWire,  ds18b20[i].Address);
  	}

	for(;;) {

		while (DS18B20_UPDATE_INTERVAL_MS == 0) {

			if (Ds18b20StartConvert == 1)
				break;
			
			osDelay(10);	
		}

		Ds18b20Timeout = DS18B20_CONVERT_TIMEOUT_MS / 10;
		
		DS18B20_StartAll(&OneWire);

		osDelay(10);
    
    	while (!DS18B20_AllDone(&OneWire)) {

			osDelay(10);

			Ds18b20Timeout -= 1;

			if (Ds18b20Timeout == 0)
				break;
		}	
		
		if (Ds18b20Timeout > 0) {

			uint8_t i = 0;

			for (i = 0; i < TempSensorCount; i++) {

				osDelay(10);
				
				ds18b20[i].DataIsValid = DS18B20_Read(&OneWire, ds18b20[i].Address, &ds18b20[i].Temperature);

				if (ds18b20[i].DataIsValid) {
					writef("OW: Sensor %d = %2.2f\r\n", TempSensorCount, ds18b20[i].Temperature);
				}

			}

		} else {

			uint8_t i = 0;

			for (i = 0; i < TempSensorCount; i++)
				ds18b20[i].DataIsValid = false;
		}

		Ds18b20StartConvert = 0;

    	osDelay(DS18B20_UPDATE_INTERVAL_MS);
	}
}


//###########################################################################################

uint8_t DS18B20_Start(OneWire_t* OneWire, uint8_t *ROM)
{
	/* Check if device is DS18B20 */
	if (!DS18B20_Is(ROM))
		return 0;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Start temperature conversion */
	OneWire_WriteByte(OneWire, DS18B20_CMD_CONVERTTEMP);
	
	return 1;
}


void DS18B20_StartAll(OneWire_t* OneWire)
{
	/* Reset pulse */
	OneWire_Reset(OneWire);

	/* Skip rom */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_SKIPROM);

	/* Start conversion on all connected devices */
	OneWire_WriteByte(OneWire, DS18B20_CMD_CONVERTTEMP);
}


bool DS18B20_Read(OneWire_t* OneWire, uint8_t *ROM, float *destination) 
{
	uint16_t temperature;
	uint8_t resolution;
	int8_t digit, minus = 0;
	float decimal;
	uint8_t i = 0;
	uint8_t data[9];
	uint8_t crc;
	
	/* Check if device is DS18B20 */
	if (!DS18B20_Is(ROM)) {
		return false;
	}
	
	/* Check if line is released, if it is, then conversion is complete */
	if (!OneWire_ReadBit(OneWire)) {
		/* Conversion is not finished yet */
		return false; 
	}

	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Get data */
	for (i = 0; i < 9; i++) {
		/* Read byte by byte */
		data[i] = OneWire_ReadByte(OneWire);
	}
	
	/* Calculate CRC */
	crc = OneWire_CRC8(data, 8);
	
	/* Check if CRC is ok */
	if (crc != data[8]) {
		/* CRC invalid */
		return 0;
	}
	
	/* First two bytes of scratchpad are temperature values */
	temperature = data[0] | (data[1] << 8);

	/* Reset line */
	OneWire_Reset(OneWire);
	
	/* Check if temperature is negative */
	if (temperature & 0x8000) {
		/* Two's complement, temperature is negative */
		temperature = ~temperature + 1;
		minus = 1;
	}
	
	/* Get sensor resolution */
	resolution = ((data[4] & 0x60) >> 5) + 9;
	
	/* Store temperature integer digits and decimal digits */
	digit = temperature >> 4;
	digit |= ((temperature >> 8) & 0x7) << 4;
	
	/* Store decimal digits */
	switch (resolution) {
		case 9:
			decimal = (temperature >> 3) & 0x01;
			decimal *= (float)DS18B20_DECIMAL_STEPS_9BIT;
		break;
		case 10:
			decimal = (temperature >> 2) & 0x03;
			decimal *= (float)DS18B20_DECIMAL_STEPS_10BIT;
		 break;
		case 11: 
			decimal = (temperature >> 1) & 0x07;
			decimal *= (float)DS18B20_DECIMAL_STEPS_11BIT;
		break;
		case 12: 
			decimal = temperature & 0x0F;
			decimal *= (float)DS18B20_DECIMAL_STEPS_12BIT;
		 break;
		default: 
			decimal = 0xFF;
			digit = 0;
	}
	
	/* Check for negative part */
	decimal = digit + decimal;
	if (minus) 
		decimal = 0 - decimal;
	
	/* Set to pointer */
	*destination = decimal;
	
	/* Return 1, temperature valid */
	return true;
}


uint8_t DS18B20_GetResolution(OneWire_t* OneWire, uint8_t *ROM)
{
	uint8_t conf;
	
	if (!DS18B20_Is(ROM)) 
		return 0;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Ignore first 4 bytes */
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	
	/* 5th byte of scratchpad is configuration register */
	conf = OneWire_ReadByte(OneWire);
	
	/* Return 9 - 12 value according to number of bits */
	return ((conf & 0x60) >> 5) + 9;
}

uint8_t DS18B20_SetResolution(OneWire_t* OneWire, uint8_t *ROM, DS18B20_Resolution_t resolution) 
{
	uint8_t th, tl, conf;

	if (!DS18B20_Is(ROM)) 
		return 0;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Ignore first 2 bytes */
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	
	th = OneWire_ReadByte(OneWire);
	tl = OneWire_ReadByte(OneWire);
	conf = OneWire_ReadByte(OneWire);
	
	if (resolution == DS18B20_Resolution_9bits) 
	{
		conf &= ~(1 << DS18B20_RESOLUTION_R1);
		conf &= ~(1 << DS18B20_RESOLUTION_R0);
	}
	else if (resolution == DS18B20_Resolution_10bits) 
	{
		conf &= ~(1 << DS18B20_RESOLUTION_R1);
		conf |= 1 << DS18B20_RESOLUTION_R0;
	}
	else if (resolution == DS18B20_Resolution_11bits)
	{
		conf |= 1 << DS18B20_RESOLUTION_R1;
		conf &= ~(1 << DS18B20_RESOLUTION_R0);
	}
	else if (resolution == DS18B20_Resolution_12bits)
	{
		conf |= 1 << DS18B20_RESOLUTION_R1;
		conf |= 1 << DS18B20_RESOLUTION_R0;
	}
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Write scratchpad command by onewire protocol, only th, tl and conf register can be written */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_WSCRATCHPAD);
	
	/* Write bytes */
	OneWire_WriteByte(OneWire, th);
	OneWire_WriteByte(OneWire, tl);
	OneWire_WriteByte(OneWire, conf);
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Copy scratchpad to EEPROM of DS18B20 */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}


uint8_t DS18B20_Is(uint8_t *ROM) 
{
	/* Checks if first byte is equal to DS18B20's family code */
	if (*ROM == DS18B20_FAMILY_CODE) 
		return 1;
	
	return 0;
}


uint8_t DS18B20_SetAlarmLowTemperature(OneWire_t* OneWire, uint8_t *ROM, int8_t temp) 
{
	uint8_t tl, th, conf;

	if (!DS18B20_Is(ROM)) 
		return 0;
	
	if (temp > 125) 
		temp = 125;
	 
	if (temp < -55) 
		temp = -55;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Ignore first 2 bytes */
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	
	th = OneWire_ReadByte(OneWire);
	tl = OneWire_ReadByte(OneWire);
	conf = OneWire_ReadByte(OneWire);
	
	tl = (uint8_t)temp; 

	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Write scratchpad command by onewire protocol, only th, tl and conf register can be written */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_WSCRATCHPAD);
	
	/* Write bytes */
	OneWire_WriteByte(OneWire, th);
	OneWire_WriteByte(OneWire, tl);
	OneWire_WriteByte(OneWire, conf);
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Copy scratchpad to EEPROM of DS18B20 */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}


uint8_t DS18B20_SetAlarmHighTemperature(OneWire_t* OneWire, uint8_t *ROM, int8_t temp) 
{
	uint8_t tl, th, conf;

	if (!DS18B20_Is(ROM)) 
		return 0;
	
	if (temp > 125) 
		temp = 125;
	
	if (temp < -55) 
		temp = -55;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Ignore first 2 bytes */
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	
	th = OneWire_ReadByte(OneWire);
	tl = OneWire_ReadByte(OneWire);
	conf = OneWire_ReadByte(OneWire);
	
	th = (uint8_t)temp; 

	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Write scratchpad command by onewire protocol, only th, tl and conf register can be written */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_WSCRATCHPAD);
	
	/* Write bytes */
	OneWire_WriteByte(OneWire, th);
	OneWire_WriteByte(OneWire, tl);
	OneWire_WriteByte(OneWire, conf);
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Copy scratchpad to EEPROM of DS18B20 */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}


uint8_t DS18B20_DisableAlarmTemperature(OneWire_t* OneWire, uint8_t *ROM) 
{
	uint8_t tl, th, conf;

	if (!DS18B20_Is(ROM)) 
		return 0;
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Read scratchpad command by onewire protocol */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_RSCRATCHPAD);
	
	/* Ignore first 2 bytes */
	OneWire_ReadByte(OneWire);
	OneWire_ReadByte(OneWire);
	
	th = OneWire_ReadByte(OneWire);
	tl = OneWire_ReadByte(OneWire);
	conf = OneWire_ReadByte(OneWire);
	
	th = 125;
	tl = (uint8_t)-55;

	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Write scratchpad command by onewire protocol, only th, tl and conf register can be written */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_WSCRATCHPAD);
	
	/* Write bytes */
	OneWire_WriteByte(OneWire, th);
	OneWire_WriteByte(OneWire, tl);
	OneWire_WriteByte(OneWire, conf);
	
	/* Reset line */
	OneWire_Reset(OneWire);

	/* Select ROM number */
	OneWire_SelectWithPointer(OneWire, ROM);

	/* Copy scratchpad to EEPROM of DS18B20 */
	OneWire_WriteByte(OneWire, ONEWIRE_CMD_CPYSCRATCHPAD);
	
	return 1;
}


uint8_t DS18B20_AlarmSearch(OneWire_t* OneWire)
{
	/* Start alarm search */
	return OneWire_Search(OneWire, DS18B20_CMD_ALARMSEARCH);
}


uint8_t DS18B20_AllDone(OneWire_t* OneWire)
{
	/* If read bit is low, then device is not finished yet with calculation temperature */
	return OneWire_ReadBit(OneWire);
}

