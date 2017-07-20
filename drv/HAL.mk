
LIB_HAL = ./drv/$(MCU_FAMILY)_HAL_Driver

CSRC += $(LIB_HAL)/Src/stm32f4xx_hal.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dma2d.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dma.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dma_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_eth.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_flash.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_flash_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_flash_ramfunc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_sram.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_gpio.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_i2c.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_i2c_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_pwr.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_pwr_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_rcc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_rcc_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_spi.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_tim.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_tim_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_uart.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_cortex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_usart.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_rng.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_rtc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_adc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_adc_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_can.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_cec.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_crc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_cryp.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_cryp_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dac.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dac_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dcmi.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dcmi_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dfsdm.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_dsi.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_fmpi2c.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_fmpi2c_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_hash.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_hash_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_hcd.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_i2s.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_i2s_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_irda.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_iwdg.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_lptim.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_ltdc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_ltdc_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_mmc.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_nand.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_nor.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_pccard.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_pcd.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_pcd_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_qspi.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_rtc_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_sai.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_sai_ex.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_sd.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_sdram.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_smartcard.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_spdifrx.c \
		$(LIB_HAL)/Src/stm32f4xx_hal_wwdg.c \


# Low Level routines
CSRC +=	$(LIB_HAL)/Src/stm32f4xx_ll_fsmc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_usart.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_fmc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_sdmmc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_usb.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_adc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_crc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_dac.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_dma2d.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_dma.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_exti.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_gpio.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_i2c.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_lptim.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_pwr.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_rcc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_rng.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_rtc.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_spi.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_tim.c \
		$(LIB_HAL)/Src/stm32f4xx_ll_utils.c \
		
		#$(LIB_HAL)/Src/stm32f4xx_hal_timebase_rtc_alarm_template.c \
		#$(LIB_HAL)/Src/stm32f4xx_hal_timebase_rtc_wakeup_template.c \
		#$(LIB_HAL)/Src/stm32f4xx_hal_timebase_tim_template.c \
		#$(LIB_HAL)/Src/stm32f4xx_hal_msp_template.c \

INCDIR += $(LIB_HAL)/Inc \




