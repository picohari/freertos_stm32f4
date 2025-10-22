/*
 * This board file is used for the following setup:
 * An ILI9325 display controller connected to an STM32F407IGT micro controller through a 16-bit
 * parallel interface (Intel 8080 compatible). The FMC interface of the STM32 micro controller is used.
 * LCD_CS is hooked up to NE4 (4. SRAM bank) and LCD_RS is hooked up to A15.
 * Schematic of display can be found here: https://www.waveshare.com/wiki/File:3.2inch-320-240-Touch-LCD-SCH.pdf
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#include "stm32f4xx_hal.h"

/* Backlight can be PWM controlled - must modify hardware and remove resistor R9 */
//#define BL_CONTROL_ON
//#include "timer.h"  // for backlight PWM timer control

#define GDISP_REG              (*((volatile gU16 *) 0x6C000000)) /* FSMC Bank4, RS = 0 */
#define GDISP_RAM              (*((volatile gU16 *) 0x6C010000)) /* FSMC Bank4, RS = 1 (A15) */

static GFXINLINE void init_board(GDisplay* g)
{
  GPIO_InitTypeDef GPIO_Init_Structure;
  SRAM_HandleTypeDef hsram;
  FSMC_NORSRAM_TimingTypeDef SRAM_Timing;

  (void)g;

  /* Enable FSMC clock */
  __FSMC_CLK_ENABLE();

  /* Peripheral clocks */
  __HAL_RCC_GPIOD_CLK_ENABLE();   // LCD
  __HAL_RCC_GPIOE_CLK_ENABLE();   // LCD
  __HAL_RCC_GPIOG_CLK_ENABLE();   // LCD
  
  #ifndef BL_CONTROL_ON
  __HAL_RCC_GPIOF_CLK_ENABLE();   // Backlight
  
  /* BACKLIGHT */
  // Display has BL_PWM on PF7 (TIM11_CH1) for brightness control but it is always on.
  // as long as R8 is kept in place on display pcb. Jumper out resistor R8 and enable
  // PWM control in TIM11
  // BUT: As ratio of R8/R9 is about 0.3V when PF7 is low, voltage is still too high to drive CE low.
  // Reading datasheet of XC6209 page 6 says, threshold must be LOWER than 0.25V to drive CE off.
  GPIO_Init_Structure.Pin   = GPIO_PIN_7;
  GPIO_Init_Structure.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_Init_Structure.Pull  = GPIO_PULLUP;
  GPIO_Init_Structure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);
  #endif

  /* LCD_RESET */
  // Display has no dedicated reset line.


  /* FSMC */
  {
    /* Common GPIO configuration */
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init_Structure.Alternate = GPIO_AF12_FSMC;
    
    /* GPIOD configuration */
    GPIO_Init_Structure.Pin   = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | \
                                GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

    /* GPIOE configuration */  
    GPIO_Init_Structure.Pin   = GPIO_PIN_7 | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 | \
                                GPIO_PIN_11| GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
    
    /* GPIOG configuration */  
    GPIO_Init_Structure.Pin   = GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);


    /* SRAM device configuration */
    hsram.Instance = FMC_NORSRAM_DEVICE;
    hsram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;

    // hsram.Init
    hsram.Init.NSBank             = FSMC_NORSRAM_BANK4;
    hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
    hsram.Init.PageSize           = FSMC_PAGE_SIZE_NONE;
    //hsram.Init.ContinuousClock    = FSMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    //hsram.Init.WriteFifo          = FSMC_WRITE_FIFO_ENABLE;

    // Timings
    SRAM_Timing.DataSetupTime         = 8;
    SRAM_Timing.AddressSetupTime      = 8;
    SRAM_Timing.AddressHoldTime       = 0;
    SRAM_Timing.BusTurnAroundDuration = 0;
    SRAM_Timing.CLKDivision           = 0;
    SRAM_Timing.DataLatency           = 0;
    SRAM_Timing.AccessMode            = FSMC_ACCESS_MODE_A;

    /* Initialize the SRAM controller */
    HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing);
  }
}

static GFXINLINE void post_init_board(GDisplay* g)
{
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay* g, gBool state)
{
	(void) g;
  (void) state;
}

static GFXINLINE void set_backlight(GDisplay* g, gU8 percent)
{
	(void) g;

#ifdef BL_CONTROL_ON
  /* Backlight PWM control */
  if (percent > 100) percent = 100;
  uint32_t value = (htim11.Init.Period * percent) / 100;
  __HAL_TIM_SET_COMPARE(&htim11, TIM_CHANNEL_1, value);
#else
  /* This is useless, until R8 is kept in place - tested. Turn it on anyway. */
	if (percent > 0) {
	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET);
	} else {
	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET);
	}
#endif
}

static GFXINLINE void acquire_bus(GDisplay* g)
{
	(void) g;
}

static GFXINLINE void release_bus(GDisplay* g)
{
	(void) g;
}

static GFXINLINE void write_index(GDisplay* g, gU16 index)
{
	(void) g;

  GDISP_REG = index;
}

static GFXINLINE void write_data(GDisplay* g, gU16 data)
{
	(void) g;

  GDISP_RAM = data;
}

static GFXINLINE void setreadmode(GDisplay* g)
{
	(void) g;
}

static GFXINLINE void setwritemode(GDisplay* g)
{
	(void) g;
}

static GFXINLINE gU16 read_data(GDisplay* g)
{
	(void) g;

	return (gU16)GDISP_RAM;
}

#endif /* _GDISP_LLD_BOARD_H */
