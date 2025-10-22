#pragma once

#include <stm32f4xx_hal.h>
#include <string.h>

// The command byte to put on the front of each page line
#define SSD1312_PAGE_PREFIX	0x40		// Co = 0, D/C = 1

static I2C_HandleTypeDef i2cHandle;

static GFXINLINE void init_board(GDisplay *g)
{
    (void) g;

    // GPIO
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};

        __HAL_RCC_GPIOB_CLK_ENABLE();

        // I2C SCL
        GPIO_InitStruct.Pin         = GPIO_PIN_8;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_PULLUP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate   = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        // I2C SDA
        GPIO_InitStruct.Pin         = GPIO_PIN_9;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull        = GPIO_PULLUP;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate   = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

    // I2C1
    {
        __HAL_RCC_I2C1_CLK_ENABLE();

        i2cHandle.Instance             = I2C1;
        i2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
        i2cHandle.Init.ClockSpeed      = 400000;
        i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
        i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        i2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&i2cHandle) != HAL_OK)
            gfxHalt("I2C HAL init error");
    }
}

static GFXINLINE void post_init_board(GDisplay *g)
{
    (void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, gBool state)
{
    (void) g;
    (void) state;
}

static GFXINLINE void acquire_bus(GDisplay *g)
{
    (void) g;
}

static GFXINLINE void release_bus(GDisplay *g)
{
    (void) g;
}

static GFXINLINE void write_cmd(GDisplay *g, gU8 *data, gU16 length)
{
    (void) g;

    gU8 buf[4];		// length is always <= 3
    buf[0] = 0x00;
    memcpy(buf+1, data, length);

    HAL_I2C_Master_Transmit(&i2cHandle, (0x3c << 1), buf, length+1, 10000);
}

static GFXINLINE void write_data(GDisplay *g, gU8 *data, gU16 length)
{
    (void) g;

    HAL_I2C_Master_Transmit(&i2cHandle, (0x3c << 1), data, length, 10000);
}

