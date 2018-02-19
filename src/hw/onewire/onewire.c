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

#include <stdarg.h>
#include <string.h>

#include "uart.h"


TIM_HandleTypeDef       TimHandle;

void ow_timer3_init(void)
{

    /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();

  uint32_t uwPrescalerValue = 0;

  /* Compute the prescaler value to have TIM3 counter clock equal to 1 MHz or 1 microsecond */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock / 1000000UL) / 2) - 1;

  /* Set TIMx instance */
  TimHandle.Instance = TIM3;
   
  /* Initialize TIM3 peripheral as follow:
       + Period = 10000 - 1
       + Prescaler = ((SystemCoreClock/2)/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = 0xffff - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start Timer 3 */
  HAL_TIM_Base_Start(&TimHandle);

}

void ow_delay_us(uint16_t us)
{

  taskENTER_CRITICAL();

  //TIM_SetCounter(TIM3, 0x0000);
  __HAL_TIM_SET_COUNTER(&TimHandle, 0x0000);

  //uint16_t start = TIM_GetCounter(TIM3);
  uint16_t start = __HAL_TIM_GET_COUNTER(&TimHandle); 

  //while((uint16_t)(TIM_GetCounter(TIM3) - start) < us)
  while((uint16_t)(__HAL_TIM_GET_COUNTER(&TimHandle) - start) < us)
      ;

  taskEXIT_CRITICAL();
}


#if 1

static GPIO_InitTypeDef GPIO_InitStruct;

void ow_gpio_init(void)
{
  /* Enable clock on PORT I */
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /* Configure PI.9 IO in output push-pull mode */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;

  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct); 
  
}

#endif







/*
void ONEWIRE_DELAY(uint16_t time_us)
{
  _DS18B20_TIMER.Instance->CNT = 0;
  while(_DS18B20_TIMER.Instance->CNT <= time_us);
}
*/

void ONEWIRE_DELAY(uint16_t time_us)
{
  ow_delay_us(time_us);
}

void ONEWIRE_LOW(OneWire_t *gp)
{
  gp->GPIOx->BSRR = gp->GPIO_Pin << 16;
} 

void ONEWIRE_HIGH(OneWire_t *gp)
{
  gp->GPIOx->BSRR = gp->GPIO_Pin;
} 

void ONEWIRE_INPUT(OneWire_t *gp)
{
  GPIO_InitTypeDef  gpinit;
  gpinit.Mode = GPIO_MODE_INPUT;
  gpinit.Pull = GPIO_NOPULL;
  gpinit.Speed = GPIO_SPEED_FREQ_MEDIUM;
  gpinit.Pin = gp->GPIO_Pin;

  HAL_GPIO_Init(gp->GPIOx, &gpinit);
} 

void ONEWIRE_OUTPUT(OneWire_t *gp)
{
  GPIO_InitTypeDef  gpinit;
  gpinit.Mode = GPIO_MODE_OUTPUT_OD;
  gpinit.Pull = GPIO_NOPULL;
  gpinit.Speed = GPIO_SPEED_FREQ_MEDIUM;
  gpinit.Pin = gp->GPIO_Pin;

  HAL_GPIO_Init(gp->GPIOx, &gpinit);
}





void OneWire_Init(OneWire_t* OneWireStruct, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{ 
  //HAL_TIM_Base_Start(&_DS18B20_TIMER);
  ow_timer3_init();

  __HAL_RCC_GPIOI_CLK_ENABLE();

  OneWireStruct->GPIOx = GPIOx;
  OneWireStruct->GPIO_Pin = GPIO_Pin;

  taskENTER_CRITICAL();

  ONEWIRE_OUTPUT(OneWireStruct);

  ONEWIRE_HIGH(OneWireStruct);
  osDelay(1000);

  ONEWIRE_LOW(OneWireStruct);
  osDelay(1000);

  ONEWIRE_HIGH(OneWireStruct);
  osDelay(2000);

  taskEXIT_CRITICAL();

}

uint8_t OneWire_Reset(OneWire_t* OneWireStruct)
{
  uint8_t i;
  
  taskENTER_CRITICAL();

  /* Line low, and wait 480us */
  ONEWIRE_LOW(OneWireStruct);

  ONEWIRE_OUTPUT(OneWireStruct);

  ONEWIRE_DELAY(480);
  ONEWIRE_DELAY(20);

  /* Release line and wait for 70us */
  ONEWIRE_INPUT(OneWireStruct);
  ONEWIRE_DELAY(70);

  /* Check bit value */
  i = HAL_GPIO_ReadPin(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin);
  
  /* Delay for 410 us */
  ONEWIRE_DELAY(410);

  taskEXIT_CRITICAL();

  /* Return value of presence pulse, 0 = OK, 1 = ERROR */
  return i;
}

void OneWire_WriteBit(OneWire_t* OneWireStruct, uint8_t bit)
{
  
  taskENTER_CRITICAL();

  if (bit) {
    /* Set line low */
    ONEWIRE_LOW(OneWireStruct);
    ONEWIRE_OUTPUT(OneWireStruct);
    ONEWIRE_DELAY(10);
    
    /* Bit high */
    ONEWIRE_INPUT(OneWireStruct);
    
    /* Wait for 55 us and release the line */
    ONEWIRE_DELAY(55);
    ONEWIRE_INPUT(OneWireStruct);
  } else {
    /* Set line low */
    ONEWIRE_LOW(OneWireStruct);
    ONEWIRE_OUTPUT(OneWireStruct);
    ONEWIRE_DELAY(65);
    
    /* Bit high */
    ONEWIRE_INPUT(OneWireStruct);
    
    /* Wait for 5 us and release the line */
    ONEWIRE_DELAY(5);
    ONEWIRE_INPUT(OneWireStruct);
  }

  taskEXIT_CRITICAL();

}

uint8_t OneWire_ReadBit(OneWire_t* OneWireStruct) 
{
  uint8_t bit = 0;
  
  taskENTER_CRITICAL();

  /* Line low */
  ONEWIRE_LOW(OneWireStruct);
  ONEWIRE_OUTPUT(OneWireStruct);
  ONEWIRE_DELAY(2);
  
  /* Release line */
  ONEWIRE_INPUT(OneWireStruct);
  ONEWIRE_DELAY(10);
  
  /* Read line value */
  if (HAL_GPIO_ReadPin(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin)) {
    /* Bit is HIGH */
    bit = 1;
  }
  
  /* Wait 50us to complete 60us period */
  ONEWIRE_DELAY(50);
  
  taskEXIT_CRITICAL();

  /* Return bit value */
  return bit;
}


void OneWire_WriteByte(OneWire_t* OneWireStruct, uint8_t byte)
{
  uint8_t i = 8;

  /* Write 8 bits */
  while (i--) {
    /* LSB bit is first */
    OneWire_WriteBit(OneWireStruct, byte & 0x01);
    byte >>= 1;
  }
}

uint8_t OneWire_ReadByte(OneWire_t* OneWireStruct)
{
  uint8_t i = 8, byte = 0;
  while (i--) {
    byte >>= 1;
    byte |= (OneWire_ReadBit(OneWireStruct) << 7);
  }
  
  return byte;
}








uint8_t OneWire_First(OneWire_t* OneWireStruct)
{
  /* Reset search values */
  OneWire_ResetSearch(OneWireStruct);

  /* Start with searching */
  return OneWire_Search(OneWireStruct, ONEWIRE_CMD_SEARCHROM);
}

uint8_t OneWire_Next(OneWire_t* OneWireStruct)
{
   /* Leave the search state alone */
   return OneWire_Search(OneWireStruct, ONEWIRE_CMD_SEARCHROM);
}

void OneWire_ResetSearch(OneWire_t* OneWireStruct)
{
  /* Reset the search state */
  OneWireStruct->LastDiscrepancy = 0;
  OneWireStruct->LastDeviceFlag = 0;
  OneWireStruct->LastFamilyDiscrepancy = 0;
}

uint8_t OneWire_Search(OneWire_t* OneWireStruct, uint8_t command)
{
  uint8_t id_bit_number;
  uint8_t last_zero, rom_byte_number, search_result;
  uint8_t id_bit, cmp_id_bit;
  uint8_t rom_byte_mask, search_direction;

  /* Initialize for search */
  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;

  /* if the last call was not the last one */
  if (!OneWireStruct->LastDeviceFlag) {
    /* 1-Wire reset */
    if (OneWire_Reset(OneWireStruct)) {
      /* Reset the search */
      OneWireStruct->LastDiscrepancy = 0;
      OneWireStruct->LastDeviceFlag = 0;
      OneWireStruct->LastFamilyDiscrepancy = 0;
      return 0;
    }

    /* issue the search command */
    OneWire_WriteByte(OneWireStruct, command);

    writef("OW: searching...\r\n");  

    /* loop to do the search */
    do {

      /* read a bit and its complement */
      id_bit = OneWire_ReadBit(OneWireStruct);
      cmp_id_bit = OneWire_ReadBit(OneWireStruct);

      /* check for no devices on 1-wire */
      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
      } else {
        /* all devices coupled have 0 or 1 */
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;  // bit write value for search
        } else {
          /* if this discrepancy if before the Last Discrepancy */
          /* on a previous next then pick the same as last time */
          if (id_bit_number < OneWireStruct->LastDiscrepancy) {
            search_direction = ((OneWireStruct->ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
          } else {
            /* if equal to last pick 1, if not then pick 0 */
            search_direction = (id_bit_number == OneWireStruct->LastDiscrepancy);
          }
          
          /* if 0 was picked then record its position in LastZero */
          if (search_direction == 0) {
            last_zero = id_bit_number;

            /* check for Last discrepancy in family */
            if (last_zero < 9) {
              OneWireStruct->LastFamilyDiscrepancy = last_zero;
            }
          }

        }

        /* set or clear the bit in the ROM byte rom_byte_number */
        /* with mask rom_byte_mask */
        if (search_direction == 1) {
          OneWireStruct->ROM_NO[rom_byte_number] |= rom_byte_mask;
        } else {
          OneWireStruct->ROM_NO[rom_byte_number] &= ~rom_byte_mask;
        }
        
        // serial number search direction write bit */
        OneWire_WriteBit(OneWireStruct, search_direction);

        /* increment the byte counter id_bit_number */
        /* and shift the mask rom_byte_mask */
        id_bit_number++;
        rom_byte_mask <<= 1;

        /* if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask */
        if (rom_byte_mask == 0) {
          //docrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC

          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);  // loop until through all ROM bytes 0-7

    /* if the search was successful then */
    if (!(id_bit_number < 65)) {
      // search successful so set LastDiscrepancy,LastDeviceFlag,search_result 
      OneWireStruct->LastDiscrepancy = last_zero;

      /* check for last device */
      if (OneWireStruct->LastDiscrepancy == 0) {
        OneWireStruct->LastDeviceFlag = 1;
      }

      writef("OW: found %02x %02x %02x %02x %02x %02x %02x %02x\r\n", 
            OneWireStruct->ROM_NO[0],
            OneWireStruct->ROM_NO[1],
            OneWireStruct->ROM_NO[2],
            OneWireStruct->ROM_NO[3],
            OneWireStruct->ROM_NO[4],
            OneWireStruct->ROM_NO[5],
            OneWireStruct->ROM_NO[6],
            OneWireStruct->ROM_NO[7]);

      search_result = 1;
    }
  }

  /* if no device found then reset counters so next 'search' will be like a first */
  if (!search_result || !OneWireStruct->ROM_NO[0]) {
    OneWireStruct->LastDiscrepancy = 0;
    OneWireStruct->LastDeviceFlag = 0;
    OneWireStruct->LastFamilyDiscrepancy = 0;
    search_result = 0;
  }

  return search_result;
}





#if 0

int OneWire_Verify(OneWire_t* OneWireStruct)
{
  unsigned char rom_backup[8];
  int i,rslt,ld_backup,ldf_backup,lfd_backup;

  /* keep a backup copy of the current state */
  for (i = 0; i < 8; i++)
  rom_backup[i] = OneWireStruct->ROM_NO[i];
  ld_backup = OneWireStruct->LastDiscrepancy;
  ldf_backup = OneWireStruct->LastDeviceFlag;
  lfd_backup = OneWireStruct->LastFamilyDiscrepancy;

  // set search to find the same device
  OneWireStruct->LastDiscrepancy = 64;
  OneWireStruct->LastDeviceFlag = 0;

  if (OneWire_Search(OneWireStruct, ONEWIRE_CMD_SEARCHROM)) {
    // check if same device found
    rslt = 1;
    for (i = 0; i < 8; i++) {
      if (rom_backup[i] != OneWireStruct->ROM_NO[i]) {
        rslt = 1;
        break;
      }
    }
  } else {
    rslt = 0;
  }

  // restore the search state 
  for (i = 0; i < 8; i++) {
    OneWireStruct->ROM_NO[i] = rom_backup[i];
  }
  OneWireStruct->LastDiscrepancy = ld_backup;
  OneWireStruct->LastDeviceFlag = ldf_backup;
  OneWireStruct->LastFamilyDiscrepancy = lfd_backup;

  // return the result of the verify
  return rslt;
}


void OneWire_TargetSetup(OneWire_t* OneWireStruct, uint8_t family_code)
{
   uint8_t i;

  // set the search state to find SearchFamily type devices
  OneWireStruct->ROM_NO[0] = family_code;
  for (i = 1; i < 8; i++) {
    OneWireStruct->ROM_NO[i] = 0;
  }
  
  OneWireStruct->LastDiscrepancy = 64;
  OneWireStruct->LastFamilyDiscrepancy = 0;
  OneWireStruct->LastDeviceFlag = 0;
}


void OneWire_FamilySkipSetup(OneWire_t* OneWireStruct)
{
  // set the Last discrepancy to last family discrepancy
  OneWireStruct->LastDiscrepancy = OneWireStruct->LastFamilyDiscrepancy;
  OneWireStruct->LastFamilyDiscrepancy = 0;

  // check for end of list
  if (OneWireStruct->LastDiscrepancy == 0) {
    OneWireStruct->LastDeviceFlag = 1;
  }
}


uint8_t OneWire_GetROM(OneWire_t* OneWireStruct, uint8_t index)
{
  return OneWireStruct->ROM_NO[index];
}


void OneWire_Select(OneWire_t* OneWireStruct, uint8_t* addr)
{
  uint8_t i;
  OneWire_WriteByte(OneWireStruct, ONEWIRE_CMD_MATCHROM);
  
  for (i = 0; i < 8; i++) {
    OneWire_WriteByte(OneWireStruct, *(addr + i));
  }
}

#endif

void OneWire_SelectWithPointer(OneWire_t* OneWireStruct, uint8_t *ROM)
{
  uint8_t i;
  OneWire_WriteByte(OneWireStruct, ONEWIRE_CMD_MATCHROM);
  
  for (i = 0; i < 8; i++) {
    OneWire_WriteByte(OneWireStruct, *(ROM + i));
  } 
}






void OneWire_GetFullROM(OneWire_t* OneWireStruct, uint8_t *firstIndex)
{
  uint8_t i;

  for (i = 0; i < 8; i++) {
    *(firstIndex + i) = OneWireStruct->ROM_NO[i];
  }
}


uint8_t OneWire_CRC8(uint8_t *addr, uint8_t len)
{

  uint8_t crc = 0, inbyte, i, mix;

  while (len--) {
    inbyte = *addr++;

    for (i = 8; i; i--) {
      
      mix = (crc ^ inbyte) & 0x01;
      
      crc >>= 1;

      if (mix) {
        crc ^= 0x8C;
      }

      inbyte >>= 1;
    }
  }

  /* Return calculated CRC */
  return crc;
}
