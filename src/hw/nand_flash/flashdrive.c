

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "flashdrive.h"



#define DEBUG_NANDFLASH     
#include "uart.h"
  
#ifdef DEBUG_NANDFLASH
#define NANDF_DEBUG(a...)  debug("nandf: " a)
#else
#define NANDF_DEBUG(a...)
#endif


NAND_HandleTypeDef hNAND; 	/* NAND handle */

void flashdrive_init(void) 
{

	GPIO_InitTypeDef GPIO_Init_Structure; 

	/* Enable FSMC clock */
	__FSMC_CLK_ENABLE();

	/* Enable GPIOs clock */
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();


	/*-- GPIO Configuration ------------------------------------------------------*/

	GPIO_Init_Structure.Mode       = GPIO_MODE_AF_PP;
	GPIO_Init_Structure.Pull 	   = GPIO_NOPULL;
	GPIO_Init_Structure.Speed      = GPIO_SPEED_FREQ_LOW;
  	GPIO_Init_Structure.Alternate  = GPIO_AF12_FSMC;

	/* D0->D3 NAND pin configuration  */
  	GPIO_Init_Structure.Pin   = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_14 | GPIO_PIN_15;

	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

	/* D4->D7 NAND pin configuration  */
  	GPIO_Init_Structure.Pin   = GPIO_PIN_7  | GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10;

	HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);

	/* RE/WE/CE/CLE/ALE NAND pin configuration  */
  	GPIO_Init_Structure.Pin   = GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12;

	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);


	/* R/B NAND pin configuration */
	GPIO_Init_Structure.Pin 	= GPIO_PIN_6;
	GPIO_Init_Structure.Speed   = GPIO_SPEED_FREQ_LOW;
	GPIO_Init_Structure.Pull 	= GPIO_PULLDOWN;
	GPIO_Init_Structure.Mode 	= GPIO_MODE_INPUT;

	HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);


  	

	FMC_NAND_PCC_TimingTypeDef 		NAND_ComTiming; 		/* Common memory space timing */
	FMC_NAND_PCC_TimingTypeDef 		NAND_AttTiming; 		/* Attribute memory space timing */

	hNAND.Instance = FSMC_NAND_DEVICE;

	/* NAND device configuration */
	NAND_ComTiming.SetupTime 		= 0xf5;
	NAND_ComTiming.WaitSetupTime 	= 0xf3;
	NAND_ComTiming.HoldSetupTime 	= 0xf2;
	NAND_ComTiming.HiZSetupTime 	= 0xf5;

	NAND_AttTiming.SetupTime 		= 1;
	NAND_AttTiming.WaitSetupTime 	= 3;
	NAND_AttTiming.HoldSetupTime 	= 1;
	NAND_AttTiming.HiZSetupTime 	= 2;

	hNAND.Init.NandBank 		= FSMC_NAND_BANK2;
	hNAND.Init.Waitfeature 		= FSMC_NAND_PCC_WAIT_FEATURE_DISABLE;
	hNAND.Init.MemoryDataWidth 	= FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
	hNAND.Init.EccComputation 	= FSMC_NAND_ECC_ENABLE;
	hNAND.Init.ECCPageSize 		= FSMC_NAND_ECC_PAGE_SIZE_512BYTE;

	hNAND.Init.TCLRSetupTime 	= 0xa1;
	hNAND.Init.TARSetupTime 	= 0x15;
/*
	hNAND.Info.BlockNbr 		= 0x04;
	hNAND.Info.BlockSize 		= 0x20;
	hNAND.Info.ZoneSize 		= 0x400;
	hNAND.Info.PageSize 		= 0x200;
	hNAND.Info.SpareAreaSize 	= 0x10;
*/
	/* NAND controller initialization */
	HAL_NAND_Init(&hNAND, &NAND_ComTiming, &NAND_AttTiming);

}

