/**
  ******************************************************************************
  * @file    src/main.c 
  * @author  Harald W. Leschner (DK6YF) - H9 Laboratory Ltd.
  * @version V1.0.0
  * @date    15-July-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
#include <string.h>

#include "config.h"
#include "log.h"

/* XCore drivers */
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "XCore407I.h"

/* Hardware peripherals */
#include "uart.h"
#include "rtc_clock.h"
#include "eth_if.h"
#include "flashdrive.h"
#include "adc.h"

/* External Hardware */
#include "onewire.h"
#include "ds18b20.h"

#include "encoder.h"
#include "timer.h"

/* GUI and interfaces */
#include "gfx.h"
#include "gui.h"
//#include "vt100.h"
//#include "test.h"

/* LWIP stack */
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/dns.h"

/* NTP, DNS and DHCP */
#include "lwip/apps/sntp.h"
#include "dhcp_eth.h"

/* HTTP server */
#include "httpd_server.h"
//#include "test_server.h"
#include "udp_connect.h"

/* Clients */
#include "httplog.h"
//#include "lwip/apps/mqtt.h"
#include "MQTTlwip.h"
#include "MQTTClient.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio.h"

/* LinuxCNC application */
#include "linuxcnc_ctrl.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED1_UPDATE_DELAY   250
#define LED2_UPDATE_DELAY   500



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private function prototypes -----------------------------------------------*/
/*
 ____               _          _                            
|  _ \  _ __  ___  | |_  ___  | |_  _   _  _ __    ___  ___ 
| |_) || '__|/ _ \ | __|/ _ \ | __|| | | || '_ \  / _ \/ __|
|  __/ | |  | (_) || |_| (_) || |_ | |_| || |_) ||  __/\__ \
|_|    |_|   \___/  \__|\___/  \__| \__, || .__/  \___||___/
                                    |___/ |_|               
*/

static void os_init(void);              /* Hardware setup */
static void os_tasks(void);             /* Tasks declarations */
static void SystemClock_Config(void);   /* STM32 clock setup */

static void Register_printout(void);    /* Some hardware register printout on serial console */


/* Threads and startup tasks */
static void LED1_thread (void const * arg);   /* Blink LED 1 */
//static void LED2_thread (void const * arg);   /* Blink LED 2 */

static void GUI_start   (void const * arg);   /* Initiate LCD and uGFX */
static void GUI_thread  (void const * arg);   /* Operate GUI */

static void NET_start   (void const * arg);   /* ETH Networking */
static void HTTP_start  (void const * arg);   /* HTTP Server */
static void UDP_start   (void const * arg);   /* UDP Service */

static void LinuxCNC_start (void const * arg); /* LinuxCNC Application */  

//static void MQTT_start  (void const * arg);   /* MQTT Server + Client */
//static void MQTT_connect(mqtt_client_t *client);

//static void TTY_thread  (void const * arg);   /* TTY Console command interpreter */
//static void RTC_thread  (void const * arg);   /* Real Time Clock */
//static void ADC_thread  (void const * arg);   /* Read ADC input */
//static void OW_thread   (void const * arg);   /* Read ONEWIRE Temperature Sensor */

/* USB Tasks */
//static void USB_thread  (void const * arg);
//static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
//static void MSC_Application(void);
//static void DISK_thread (void const * arg);



/* Private functions ---------------------------------------------------------*/

/*
 ___         _  _    _         _  _             _    _               
|_ _| _ __  (_)| |_ (_)  __ _ | |(_) ____ __ _ | |_ (_)  ___   _ __  
 | | | '_ \ | || __|| | / _` || || ||_  // _` || __|| | / _ \ | '_ \ 
 | | | | | || || |_ | || (_| || || | / /| (_| || |_ | || (_) || | | |
|___||_| |_||_| \__||_| \__,_||_||_|/___|\__,_| \__||_| \___/ |_| |_|
                                                                     
*/

/**
  * @brief  Initialize hardware and HAL subsystems.
  * @param  None
  * @retval None
  */
static void os_init(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();

  /* Configure interrupt for system clock with maximum priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0x0, 0x0);

  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure UART */
  uart_init();

  /* Configure RTC */
  rtc_init();
  RTC_CalendarConfig(17, 1, 1, 0, 0, 0);

  Register_printout();

  /* Configure ADC Temperature Sensor */
  //adc_temp_init();

  /* Configure rotary encoder */
  Encoder_Init();
}



/*
 _____            _         
|_   _|__ _  ___ | | __ ___ 
  | | / _` |/ __|| |/ // __|
  | || (_| |\__ \|   < \__ \
  |_| \__,_||___/|_|\_\|___/
                            
*/

/**
  * @brief  FreeRTOS task definitions and initialization.
  * @param  None
  * @retval None
  */
static void os_tasks(void)
{
  /* LED 1 */
  osThreadDef(led1, LED1_thread,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
  osThreadCreate( osThread(led1),  NULL);

  /* LED 2 */
  //osThreadDef(led2, LED2_thread,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
  //LEDThread2Handle = osThreadCreate( osThread(led2),  NULL);

  /* GUI */
  osThreadDef(sgui, GUI_start,     osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(sgui),  NULL);

  /* NETWORK */
  osThreadDef(snet, NET_start,     osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(snet),  NULL);

  /* MAIN LinuxCNC APPLICATION */
  osThreadDef(lcnc, LinuxCNC_start, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(lcnc),  NULL);


  /* RTC */
  //osThreadDef(rtc0, RTC_thread,    osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
  //osThreadCreate( osThread(rtc0),  NULL);

  /* ADC */
  //osThreadDef(adc0, ADC_thread,    osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
  //osThreadCreate( osThread(adc0),  NULL);

  /* ONEWIRE */
  //osThreadDef(ow0, OW_thread,      osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  //osThreadCreate( osThread(ow0),   NULL);


#if 0
  /*##-1- Start task #########################################################*/
  osThreadDef(USB_drv, USB_thread,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 8);
  osThreadCreate(osThread(USB_drv), NULL);
  
  osThreadDef(USB_fat, DISK_thread,  osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 8);
  osThreadCreate (osThread(USB_fat), NULL);  

  /*##-2- Create Application Queue ###########################################*/
  osMessageQDef(usb_queue,  1, uint16_t);
  AppliEvent = osMessageCreate(osMessageQ(usb_queue), NULL);

  /*##-3- Create Disk Queue ##################################################*/
  osMessageQDef(disk_queue, 1, uint16_t);
  DiskEvent  = osMessageCreate (osMessageQ(disk_queue), NULL);
#endif

}

/* Total amount of STACK_SIZE allocated: 34 * 128 = 4352 Bytes */


/*
 __  __         _        
|  \/  |  __ _ (_) _ __  
| |\/| | / _` || || '_ \ 
| |  | || (_| || || | | |
|_|  |_| \__,_||_||_| |_|
                         
*/

/**
  * @brief  Most minimalistic main function: Initalize hardware and start os.
  * @param  None
  * @retval None
  */
int main(void)
{

  /* Initialize modules and hardware */
	os_init();

  /* Create all tasks */
  os_tasks();

  /* Start scheduler - should never leave this ;) */
  osKernelStart();

	/* Infinite loop - forever */
  for(;;)

  /* Should never ever get here ..*/
	return 0;
}





/*
 _   _        _                          _    
| \ | |  ___ | |_ __      __ ___   _ __ | | __
|  \| | / _ \| __|\ \ /\ / // _ \ | '__|| |/ /
| |\  ||  __/| |_  \ V  V /| (_) || |   |   < 
|_| \_| \___| \__|  \_/\_/  \___/ |_|   |_|\_\
                                              
*/

/**
  * @brief  Create and start all networking threads.
  * @param  arg
  * @retval None
  */
static void NET_start (void const * arg)
{

  (void) arg;

  static ip_addr_t ipaddr;
  static ip_addr_t netmask;
  static ip_addr_t gw;
  static ip_addr_t dns;

#ifdef USE_DHCP
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#else
  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif

  //lwip_init(); /* TODO: Check this */ 
  tcpip_init(NULL, NULL);

  /* Add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);


  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
  
  /* create a binary semaphore used for informing ethernetif of frame reception */
  osSemaphoreDef(Netif_SEM);
  Netif_LinkSemaphore = osSemaphoreCreate(osSemaphore(Netif_SEM) , 1);

  /* Assign interface and semaphore to link status structure */
  link_arg.netif = &gnetif;
  link_arg.semaphore = Netif_LinkSemaphore;   // Useless, if no hardware interrupt line to PHY


  /* Start LINK STATUS task - poll PHY for link status as we do not have a dedicated interrupt line */
  osThreadDef(link_status, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(link_status), &link_arg);



  /* Start DNS service - set to ip of gateway (most routers do offer dns) */
  dns_init();
  IP4_ADDR(&dns, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  dns_setserver(0, &dns);

#if 0
  /* Start NTP service */
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, "de.pool.ntp.org");
  sntp_init();
#endif

  /* Test httplog */
  //int numberone = 123;
  //httplog("led1=%d", numberone);

  /* Start HTTP service */
  osThreadDef(http, HTTP_start, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(http),  NULL);

  /* Start UDP service */
  osThreadDef(udp, UDP_start,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(udp),  NULL);

  /* Start MQTT service */
  //osThreadDef(mqtt, MQTT_start,    osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 4);
  //osThreadCreate( osThread(mqtt),  NULL);

  while (1) {
    osThreadTerminate( NULL );  /* important to stop task here !! */
  }

}

static void HTTP_start (void const * arg)
{

  (void) arg;

  /* Starts the httpd server */
  http_server_init();
  //http_server_socket_init();

  while(1) { }

}

static void UDP_start (void const * arg)
{

  (void) arg;

  /* Starts the UDP jogwheel server */
  udp_server_init();

  while(1) { }

}



/*
         ____  _____ __  __   ____  _   _  ___ 
 _   _  / ___||  ___|\ \/ /  / ___|| | | ||_ _|
| | | || |  _ | |_    \  /  | |  _ | | | | | | 
| |_| || |_| ||  _|   /  \  | |_| || |_| | | | 
| ._,_| \____||_|    /_/\_\  \____| \___/ |___|
|_|                                            

*/

 /**
  * @brief  Create and start GUI thread
  * @param  arg
  * @retval None
  */
static void GUI_start (void const * arg)
{

  (void) arg;

  /* Start µGFX */
  gfxInit();

  /* Start TIM11 on PD7 for backlight control */
  #if 0
  Backlight_Init();

  if(HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1) != HAL_OK)
  {
    /* PWM Generation Error */
    Error_Handler();
  }
  #endif

  /* Setup initial display configuration */
  gdispSetBacklight(100);
  gdispSetContrast(100);
  gdispClear(HTML2COLOR(0x000000));

  /* Create all GUI windows and widgets */
  guiCreate();

  /* Create GUI thread */
  osThreadDef(gui, GUI_thread,  osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate( osThread(gui), NULL);

  osDelay(100);

  //osThreadDef(tty0, TTY_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
  //osThreadCreate( osThread(tty0), NULL);


  while (1) {
    osThreadTerminate( NULL );  /* important to stop task here !! */
  }

}

static void GUI_thread (void const * arg)
{

  (void) arg;

  while (1) {
    guiEventLoop();
    osDelay(1);

    //LCD_Backlight_Set((uint32_t) Encoder_GetPosition() );
  }
}


/*
 _      _                       ____  _   _   ____ 
| |    (_) _ __   _   _ __  __ / ___|| \ | | / ___|
| |    | || '_ \ | | | |\ \/ /| |    |  \| || |    
| |___ | || | | || |_| | >  < | |___ | |\  || |___ 
|_____||_||_| |_| \__,_|/_/\_\ \____||_| \_| \____|
                                                   
*/

/**
  * @brief  Create and start all LinuxCNC threads.
  * @param  arg
  * @retval None
  */
static void LinuxCNC_start (void const * arg)
{

  (void) arg;

  LinuxCNC_Init();

  
  while (1) {
    
    /* Read rotary encoder */
    Encoder_Update();
    
    static float last_rotary = 0.0f;
    float current_rotary = Encoder_GetPosition();

    JogState_t pkt;

    pkt.axis_select   = 1;
    pkt.encoder_value = current_rotary;

    if (current_rotary != last_rotary) {
      last_rotary = current_rotary;

      udp_send_jogstate(&pkt);
    }

    osDelay(1);
  }

}








/*
 ____               _                      ____  _               _    
/ ___|  _   _  ___ | |_  ___  _ __ ___    / ___|| |  ___    ___ | | __
\___ \ | | | |/ __|| __|/ _ \| '_ ` _ \  | |    | | / _ \  / __|| |/ /
 ___) || |_| |\__ \| |_|  __/| | | | | | | |___ | || (_) || (__ |   < 
|____/  \__, ||___/ \__|\___||_| |_| |_|  \____||_| \___/  \___||_|\_\
        |___/                                                         
*/

/**
* @brief  System Clock Configuration
*         The system Clock is configured as follow : 
*            System Clock source            = PLL (HSE)
*            SYSCLK(Hz)                     = 168000000
*            HCLK(Hz)                       = 168000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 4
*            APB2 Prescaler                 = 2
*            HSE Frequency(Hz)              = 25000000
*            PLL_M                          = 8
*            PLL_N                          = 336
*            PLL_P                          = 2
*            PLL_Q                          = 7
*            VDD(V)                         = 3.3
*            Main regulator output voltage  = Scale1 mode
*            Flash Latency(WS)              = 5
* @param  None
* @retval None
*/
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
      clocked below the maximum system frequency, to update the voltage scaling value 
      regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState        = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM        = 8;
  RCC_OscInitStruct.PLL.PLLN        = 336;
  RCC_OscInitStruct.PLL.PLLP        = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ        = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
      clocks dividers */
  RCC_ClkInitStruct.ClockType       = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider  = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
  
  
/**
  * @brief  FreeRTOS hook function for configUSE_TICK_HOOK. Required for HAL in blocking functions.
  * @param  None
  * @retval None
  */
void vApplicationTickHook(void)
{
    HAL_IncTick();  // keeps HAL_GetTick() alive under FreeRTOS

    /* Test with LED4 
    static int count = 0;
    if (++count % 100 == 0)
      BSP_LED_Toggle(LED4);
    */
}
  


/*
 ____         _                     ____   _            __   __ 
|  _ \   ___ | |__   _   _   __ _  / ___| | |_  _   _  / _| / _|
| | | | / _ \| '_ \ | | | | / _` | \___ \ | __|| | | || |_ | |_ 
| |_| ||  __/| |_) || |_| || (_| |  ___) || |_ | |_| ||  _||  _|
|____/  \___||_.__/  \__,_| \__, | |____/  \__| \__,_||_|  |_|  
                            |___/                               
*/

/**
  * @brief  Debugging LEDs
  * @param  None
  * @retval None
  */
static void LED1_thread (void const * arg)
{

  (void) arg;

  while(1) {
    BSP_LED_On(LED1);
    osDelay(LED1_UPDATE_DELAY);
    BSP_LED_Off(LED1);
    osDelay(LED1_UPDATE_DELAY);
  }
}


/*
static void LED2_thread (void const * arg)
{
  
(void) arg;

while(1) {
  BSP_LED_On(LED2);
  osDelay(LED2_UPDATE_DELAY);
  BSP_LED_Off(LED2);
  osDelay(LED2_UPDATE_DELAY);
}
}
*/


/*
Print something like this:

  Firmware fc8f-dirty built on Oct 22 2025 02:52:04
  STM32F407i - CMSIS - FreeRTOS - LwIP - BSP - µGFX
  XCore: 0x0413, Rev. 0x1007
  UUID:  0035003c-32355118-34333432
  Flash: 1024 kB
  Pack:  7
  CLK:   168 MHz
  NAND Flash: K9F1G08U0B
  TFT Driver: ILI9325
  ETH PHY:    DP83848 (Rev 0)
  Network disconnected
  Static IP address: 192.168.100.164

*/

/**
  * @brief  Print hardware register infos to serial debug console.
  * @param  None
  * @retval None
  */
static void Register_printout(void)
{

  /* Clear and put some information on the starting screen */
  LOG_PRINTF("\033[2J");  // Clear screen
  LOG_PRINTF("\r\n");

  /* We run our code as ... */
  LOG_DEBUG("Firmware %s", VERSION_STRING_LONG);

  /* Welcome to our guests ;) */
  LOG_DEBUG("STM32F407i - CMSIS - FreeRTOS - LwIP - BSP - µGFX");

  /* Read STM32F4 hardware registers for information and fun ;) */
  char rev_id[42];
  sprintf((char*)rev_id, "XCore: 0x%04lx, Rev. 0x%04lx", (DBGMCU->IDCODE & 0x00000FFF), ((DBGMCU->IDCODE >> 16) & 0x0000FFFF));
  LOG_DEBUG("%s", rev_id);

  /*
     * Prettify those outputs one day...
     *
     * \par Revisions possible:
     *  - 0x1000: Revision A
     *  - 0x1001: Revision Z
     *  - 0x1003: Revision Y
     *  - 0x1007: Revision 1
     *  - 0x2001: Revision 3
     *
     * \par Device signatures:
     *  - 0x0413: STM32F405xx/07xx and STM32F415xx/17xx)
     *  - 0x0419: STM32F42xxx and STM32F43xxx
     *  - 0x0423: STM32F401xB/C
     *  - 0x0433: STM32F401xD/E
     *  - 0x0431: STM32F411xC/E
     *  - 0x0421: STM32F446xx
     *  - 0x0449: STM32F7x6xx
     *  - 0x0444: STM32F03xxx
     *  - 0x0445: STM32F04xxx
     *  - 0x0440: STM32F05xxx
     *  - 0x0448: STM32F07xxx
     *  - 0x0442: STM32F09xxx
  */

  /* Read UUID */
  uint32_t idPart1 = STM32_UUID[0];
  uint32_t idPart2 = STM32_UUID[1];
  uint32_t idPart3 = STM32_UUID[2];
  
  char uuid0_tmp[38];
  sprintf((char*)uuid0_tmp,  "UUID:  %08lx-%08lx-%08lx", idPart1, idPart2, idPart3);
  LOG_DEBUG("%s", uuid0_tmp);

  /* Print flash size */
  uint32_t flashSize = STM32_UUID_FLASH[0];
  char flash_tmp[32];
  sprintf((char*)flash_tmp, "%lx", flashSize);

  char flash[6];
  sprintf((char*)flash, &(flash_tmp[strlen(flash_tmp) - 4]));

  uint16_t size = strtol((char*)flash, NULL, 16);
  sprintf((char*)flash_tmp, "Flash: %d kB", size);
  LOG_DEBUG("%s", flash_tmp);

  /* Print package code (hardware chip case form) */
  uint32_t flashPack = (((*(__IO uint16_t *) (STM32_UUID_PACK)) & 0x0700) >> 8);

  /*
   * This also needs some makeup for better looking output
   *
   *  - 0b01xx: LQFP208 and TFBGA216 package
   *  - 0b0011: LQFP176 and UFBGA176 package
   *  - 0b0010: WLCSP143 and LQFP144 package
   *  - 0b0001: LQFP100 package
   */

  char pack_tmp[10];
  sprintf((char*)pack_tmp, "Pack:  %lx", flashPack);
  LOG_DEBUG("%s", pack_tmp);

  /* System clock runnning at ... */
  LOG_DEBUG("CLK:   %d MHz", SystemCoreClock / 1000000UL);

  /* NAND flash drive */
  flashdrive_init();

  static NAND_IDTypeDef flash_id;

  HAL_NAND_Read_ID(&hNAND, &flash_id);

  //LOG_DEBUG("NAND Flash ID: 0x%02x, 0x%02x, 0x%02x, 0x%02x", flash_id.Maker_Id, flash_id.Device_Id,
  //                                                           flash_id.Third_Id, flash_id.Fourth_Id );

  LOG_PRINTF("NAND Flash: ");
  if ((flash_id.Maker_Id == 0xEC) && (flash_id.Device_Id == 0xF1)
    && (flash_id.Third_Id == 0x80) && (flash_id.Fourth_Id == 0x15))
  {
    LOG_DEBUG("K9F1G08U0A");
  }
  else if ((flash_id.Maker_Id == 0xEC) && (flash_id.Device_Id == 0xF1)
    && (flash_id.Third_Id == 0x00) && (flash_id.Fourth_Id == 0x95))
  {
    LOG_DEBUG("K9F1G08U0B");   
  }
  else if ((flash_id.Maker_Id == 0xAD) && (flash_id.Device_Id == 0xF1)
    && (flash_id.Third_Id == 0x80) && (flash_id.Fourth_Id == 0x1D))
  {
    LOG_DEBUG("HY27UF081G2A");   
  }
  else
  {
    LOG_DEBUG("Unknow type");
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1) { }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) { }
}
#endif

