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


/* Predefined settings variables
 *
 * These are the variables to access settings through the menu.
 * As the settings values can be modified by ugfx pages, these variables are
 * encapsulated here. 
 * include axis_menu.h in app code to load/update the settings on startup
 * or during program running.
 * Expose variables globally by adding extern in axix_menu.h.
 */

/* NETWORK settings */
uint32_t remote_ipv4_addr = 0xc0a80053;     /* 192.168.0.83  - the ip of remote LinuxCNC machine */
uint32_t local_ipv4_addr  = 0xc0a800a4;     /* 192.168.0.164 - the ip of local HandWheel controller */
uint32_t netmask = 0xffffff00;              /* 255.255.255.0 - netmask */
uint32_t gateway = 0xc0a80001;              /* 192.168.0.1   - gateway */




#ifndef UGFXSIMULATOR
static SemaphoreHandle_t settings_mutex;

void axis_helper_Init(void) {
    settings_mutex = xSemaphoreCreateMutex();
}
#endif


/*
 * NETWORK Settings
 */
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

void axis_helper_setNetmask(uint32_t *new_netmask)
{
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        netmask = *new_netmask;
        xSemaphoreGive(settings_mutex);
    }
#else
    netmask = *new_netmask;
#endif
}

void axis_helper_getNetmask(uint32_t *out_netmask)
{
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        *out_netmask = netmask;
        xSemaphoreGive(settings_mutex);
    }
#else
    *out_netmask = netmask;
#endif    
}

void axis_helper_setGateway(uint32_t *new_gateway)
{
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        gateway = *new_gateway;
        xSemaphoreGive(settings_mutex);
    }
#else
    gateway = *new_gateway;
#endif   
}

void axis_helper_getGateway(uint32_t *out_gateway){
#ifndef UGFXSIMULATOR    
    if (xSemaphoreTake(settings_mutex, portMAX_DELAY) == pdTRUE) {
        *out_gateway = gateway;
        xSemaphoreGive(settings_mutex);
    }
#else
    *out_gateway = gateway;
#endif    
}

#endif /* _AXIS_HELPERS_H_ */