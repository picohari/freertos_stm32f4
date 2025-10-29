#ifndef _AXIS_HELPERS_H_
#define _AXIS_HELPERS_H_

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <linux/input.h>
#else
	#include "config.h"
	#include "task.h"
	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

	#include "eth_if.h"
	#include "lwip/netif.h"

#endif

#include "style.h"
#include "gui_menu.h"
#include "gui_logger.h"


uint32_t remote_ipv4_addr = 0xc0a80053;     /* 192.168.0.83  - the ip of remote LinuxCNC machine */
uint32_t local_ipv4_addr  = 0xc0a800a4;     /* 192.168.0.164 - the ip of local HandWheel controller */

uint32_t netmask = 0xffffff00;              /* 255.255.255.0 - netmask */
uint32_t gateway = 0xc0a80001;              /* 192.168.0.1   - gateway */

#ifndef UGFXSIMULATOR
static SemaphoreHandle_t settings_mutex;
#endif


void axis_helper_setRemoteIP(uint32_t *new_address)
{
#ifndef UGFXSIMULATOR    
if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
    remote_ipv4_addr = *new_address;
    xSemaphoreGive(settings_mutex);
}
#else
    remote_ipv4_addr = *new_address;
#endif
}

void axis_helper_getRemoteIP(uint32_t *out_addr)
{
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        *out_addr = remote_ipv4_addr;
        xSemaphoreGive(settings_mutex);
    }
#else
    *out_addr = remote_ipv4_addr;
#endif
}

void axis_helper_setLocalIP(uint32_t *new_address)
{
#ifndef UGFXSIMULATOR    
if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
    local_ipv4_addr = *new_address;
    xSemaphoreGive(settings_mutex);
}
#else
    local_ipv4_addr = *new_address;
#endif
}

void axis_helper_getLocalIP(uint32_t *out_addr)
{
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        *out_addr = local_ipv4_addr;
        xSemaphoreGive(settings_mutex);
    }
#else
    *out_addr = local_ipv4_addr;
#endif
}



void axis_helper_setNetmask(uint32_t *new_address) {}
void axis_helper_getNetmask(uint32_t *out_addr) {}

void axis_helper_setGateway(uint32_t *new_address) {}
void axis_helper_getGateway(uint32_t *out_addr) {}

#endif /* _AXIS_HELPERS_H_ */