




#ifndef __FLASH_DRIVE_H
#define __FLASH_DRIVE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include <string.h>
#include <stdio.h>


extern NAND_HandleTypeDef hNAND; 	/* NAND handle */


/* 
TODO:

CLE and ALE are inversed on the XCORE407I board, in original HAL library they are supposed to be fixed...

The defines for the HAL library code has been modified to match CXORE board. Here they are re-defined
for internal purpose and to give a little warning in the make process ...

Changed in file: stm32f4xx_hal_nand.h

*/
#ifdef CMD_AREA
#warning CMD_AREA redefined for XCORE, warning is okay !!
#undef CMD_AREA 
#define CMD_AREA                   (uint32_t)(1<<17)  /* A17 = CLE  high */
#endif

#ifdef ADDR_AREA
#warning ADDR_AREA redefined for XCORE, warning is okay !!
#undef ADDR_AREA
#define ADDR_AREA                  (uint32_t)(1<<16)  /* A16 = ALE high */
#endif


void flashdrive_init(void);





#endif /* __FLASH_DRIVE_H */
