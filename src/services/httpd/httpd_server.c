/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Raw/Src/httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-April-2016
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "config.h"

#include "lwip/apps/httpd_opts.h"
#include "lwip/opt.h"

#if LWIP_HTTPD_SUPPORT_WEBSOCKET
#include "lwip/altcp.h" // must use for websockets
#else
#include "lwip/tcp.h"
#endif
#include "lwip/debug.h"

#include "lwip/apps/httpd.h"
#include "httpd_server.h"
//#include "cgi_functions.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "FreeRTOS.h"  // portTICK_RATE_MS
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "log.h"

/* SSI functions */
#define SSI_INDEX_LEDSTATE  0
#define SSI_INDEX_PWMSTATE  1
#define SSI_INDEX_PWMFREQ   2
#define SSI_INDEX_PWMDUTY   3
#define SSI_INDEX_FORMVARS  4
#define SSI_INDEX_TEST      5

#define SSI_INDEX_VERSION   6
#define SSI_INDEX_UPTIME    7
#define SSI_FREE_HEAP       8

/* Array of TAGS must be in same order as definitions above !! */
const char *SSI_TAGS[] =
{
    "LEDtext",        // SSI_INDEX_LEDSTATE
    "PWMtext",        // SSI_INDEX_PWMSTATE
    "PWMfreq",        // SSI_INDEX_PWMFREQ
    "PWMduty",        // SSI_INDEX_PWMDUTY
    "FormVars",       // SSI_INDEX_FORMVARS
    "test",           // SSI_INDEX_TEST

    "version",        // SSI_INDEX_VERSION
    "uptime",         // SSI_INDEX_UPTIME
    "heap",           // SSI_FREE_HEAP
};

#define NUM_CONFIG_SSI_TAGS     (sizeof(SSI_TAGS) / sizeof (char *))

/* SSI functions */
u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen);

/* CGI functions */
const char * control_GCI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const char *   debug_GCI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

static const tCGI ECMD_CGI[2]  =  {
  { "/ecmd.cgi",  control_GCI_Handler },
  { "/debug.cgi",   debug_GCI_Handler }
};

#define NUM_CONFIG_CGI_URIS     (sizeof(ECMD_CGI) / sizeof(tCGI))

#define DEFAULT_CGI_RESPONSE    "/index.html"



/* WEBSOCKET functions */
void websocket_cb(struct altcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode);
void websocket_open_cb(struct altcp_pcb *pcb, const char *uri);


#include "json_parser.c"



/**
  * @brief  Http webserver Init
  */
void http_server_init(void)
{
  /* Httpd Init */
  httpd_init();
  
  /* Configure SSI handlers (ADC page SSI) */
  //http_set_ssi_handler(ADC_Handler, (char const **)TAGS, 1);
  http_set_ssi_handler(SSIHandler, SSI_TAGS, NUM_CONFIG_SSI_TAGS);

  /* Configure CGI handlers (ECMD) */
  http_set_cgi_handlers(ECMD_CGI, NUM_CONFIG_CGI_URIS);
  
  /* Configure WEBSOCKET handlers */
  //httpd_websocket_register_callbacks((tWsOpenHandler) websocket_open_cb, (tWsHandler) websocket_cb);
  httpd_websocket_register_callbacks(websocket_open_cb, websocket_cb);
}




const char * control_GCI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  (void) iIndex;

  uint16_t i;

  /* Check cgi parameter : example GET /ecmd.cgi?param=value&set=off */
  for (i = 0; i < iNumParams; i++)
  {

    LOG_DEBUG((char *)  "%s = %s", pcParam[i], pcValue[i]);

#if 0
    /* check parameter "led" */
    if (strcmp(pcParam[i] , "led")==0)   
    {
      /* Switch LED1 ON if 1 */
      if(strcmp(pcValue[i], "1") ==0) 
        BSP_LED_On(LED1);
    }
#endif

  }

  return(DEFAULT_CGI_RESPONSE);
}


const char * debug_GCI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

  (void) iIndex;
  (void) iNumParams;
  (void) pcParam;
  (void) pcValue;
  
  //LOG_DEBUG((char *)  "CGI-debug" );
  
  return(DEFAULT_CGI_RESPONSE);
}


//static tSSIHandler SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
u16_t SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
  //unsigned long ulVal;

  //
  // Which SSI tag have we been passed?
  //
  switch(iIndex)
  {
      case SSI_INDEX_LEDSTATE:
          //io_get_ledstate(pcInsert, iInsertLen);
          snprintf(pcInsert, iInsertLen, "LED");
          break;

      case SSI_INDEX_PWMSTATE:
          //io_get_pwmstate(pcInsert, iInsertLen);
          snprintf(pcInsert, iInsertLen, "PWM");
          break;

      case SSI_INDEX_PWMFREQ:
          //ulVal = io_get_pwmfreq();
          //usnprintf(pcInsert, iInsertLen, "%d", ulVal);
          break;

      case SSI_INDEX_PWMDUTY:
          //ulVal = io_get_pwmdutycycle();
          //usnprintf(pcInsert, iInsertLen, "%d", ulVal);
          break;

      case SSI_INDEX_FORMVARS:
          //LOG_UART ((char *)  "FormVars" );
          //ulVal = io_get_pwmdutycycle();
          snprintf(pcInsert, iInsertLen, "INFO");
          //snprintf(pcInsert, iInsertLen, "%s", VERSION_STRING_LONG);
          break;

      case SSI_INDEX_TEST:
          //LOG_UART ((char *)  "FormVars" );
          //ulVal = io_get_pwmdutycycle();
          //snprintf(pcInsert, iInsertLen, "%s", VERSION_STRING_LONG);
          snprintf(pcInsert, iInsertLen, "TEST");
          break;

      case SSI_INDEX_VERSION:
          //LOG_UART ((char *)  "FormVars" );
          //ulVal = io_get_pwmdutycycle();
          snprintf(pcInsert, iInsertLen, "%s", VERSION_STRING_LONG);
          break;

      case SSI_INDEX_UPTIME:
          snprintf(pcInsert, iInsertLen, "%ld", xTaskGetTickCount() * portTICK_PERIOD_MS / 1000);
          break;

      case SSI_FREE_HEAP:
          snprintf(pcInsert, iInsertLen, "%d", (int) xPortGetFreeHeapSize());
          break;

      default:
          snprintf(pcInsert, iInsertLen, "?");
          break;
  }

  //LOG_UART ((char *)  "SSI" );
  //
  // Tell the server how many characters our insert string contains.
  //
  return(strlen(pcInsert));
}





void websocket_task(void *pvParameter)
{
  struct altcp_pcb *pcb = (struct altcp_pcb *) pvParameter;

  for (;;) {
    if (pcb == NULL || pcb->state != ESTABLISHED) {
      LWIP_DEBUGF(HTTPD_DEBUG, ("[ws_task] connection closed, deleting task\n"));
      break;
    }

    int uptime = xTaskGetTickCount() * portTICK_RATE_MS / 1000;
    int heap = (int) xPortGetFreeHeapSize();
    float temp = 18.75;

    /* Generate response in JSON format */
    char response[128];
    uint len = snprintf(response, sizeof (response),
    "{\"uptime\" : \"%d\","
    " \"heap\" : \"%d\","
    " \"temperature\" : \"%+6.2f\" }", uptime, heap, temp);
    if (len < sizeof (response))
      httpd_websocket_write(pcb, (unsigned char *) response, len, WS_TEXT_MODE);

    vTaskDelay(2000 / portTICK_RATE_MS);
  }

  vTaskDelete(NULL);
}

/**
 * This function is called when new websocket is open and
 * creates a new websocket_task if requested URI equals '/stream'.
 */
void websocket_open_cb(struct altcp_pcb *pcb, const char *uri)
{
  LWIP_DEBUGF(HTTPD_DEBUG, ("WS URI: %s\n", uri));
  if (!strcmp(uri, "/stream")) {
    LWIP_DEBUGF(HTTPD_DEBUG, ("[ws_opencb] request for streaming\n"));
    xTaskCreate(&websocket_task, "websocket_task", 512, (void *) pcb, 2, NULL);
  }
}

/**
 * This function is called when websocket frame is received.
 *
 * Note: this function is executed on TCP thread and should return as soon
 * as possible.
 */
void websocket_cb(struct altcp_pcb *pcb, uint8_t *data, u16_t data_len, uint8_t mode)
{

  (void) mode;
  
  JsonParser parser;


  if (json_parse((const char *)data, &parser) == 0) {
  
    if (json_get(&parser, "cmd")) {

      /* Parse command*/
      if (strcmp(      json_get(&parser, "cmd"), "gpio") == 0) {
        LWIP_DEBUGF(HTTPD_DEBUG, ("[json] gpio: %s = %s\n", json_get(&parser, "pin"), json_get(&parser, "action")));

      } else if (strcmp(json_get(&parser, "cmd"), "led") == 0) {
        LWIP_DEBUGF(HTTPD_DEBUG, ("[json] led: %s\n", json_get(&parser, "action")));
      }

    } else if (json_get(&parser, "values")) {

      char values[10][MAX_VAL_LEN];
      int count = json_get_array(&parser, "values", values);
      printf("values: [");
      for (int i = 0; i < count; i++) {
          printf("%s%s", values[i], i < count - 1 ? ", " : "");
      }
      printf("]\n");

    }
  
  }

  /* Data example {"uptime":"294","heap":"39496","temperature":"+18.75"} */
  static uint8_t echo_data[64] = {0}; // 64 bytes MAX!!
  memset(echo_data, 0, sizeof(echo_data)); // zero
  memcpy(echo_data, data, data_len);




  LWIP_DEBUGF(HTTPD_DEBUG, ("[ws_cb]: Send back %d bytes:\n\r%s\n", (int) data_len, (char*) echo_data));

  httpd_websocket_write(pcb, echo_data, data_len, WS_BIN_MODE);    //TODO: CHeck Format!
}


























#if 0

/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
static void ADC_Configuration(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /* ADC3 Configuration ------------------------------------------------------*/
  hadc.Instance = ADC3;
  hadc.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.ScanConvMode = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1; 
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.NbrOfConversion = 1;
  
  HAL_ADC_Init(&hadc);
  
  /* ADC3 Regular Channel Config */ 
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);

  /* Enable EOC interupt */
  HAL_ADC_Start(&hadc);

}

/**
  * @brief  ADC_Handler : SSI handler for ADC page 
  */
u16_t ADC_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
  /* We have only one SSI handler iIndex = 0 */
  if (iIndex ==0)
  {  
    char Digit1=0, Digit2=0, Digit3=0, Digit4=0; 
    uint32_t ADCVal = 0;        

     /* configure ADC if not yet configured */
     if (ADC_not_configured ==1)       
     {
        ADC_Configuration();
        ADC_not_configured=0;
     }
     
     HAL_ADC_PollForConversion(&hadc, 10);
     /* get ADC conversion value */
     ADCVal =  HAL_ADC_GetValue(&hadc);
     
     /* convert to Voltage,  step = 0.8 mV */
     ADCVal = (uint32_t)(ADCVal * 0.8);  
     
     /* get digits to display */
     
     Digit1= ADCVal/1000;
     Digit2= (ADCVal-(Digit1*1000))/100;
     Digit3= (ADCVal-((Digit1*1000)+(Digit2*100)))/10;
     Digit4= ADCVal -((Digit1*1000)+(Digit2*100)+ (Digit3*10));
        
     /* prepare data to be inserted in html */
     *pcInsert       = (char)(Digit1+0x30);
     *(pcInsert + 1) = (char)(Digit2+0x30);
     *(pcInsert + 2) = (char)(Digit3+0x30);
     *(pcInsert + 3) = (char)(Digit4+0x30);
    
    /* 4 characters need to be inserted in html*/
    return 4;
  }
  return 0;
}


/**
  * @brief  CGI handler for LEDs control 
  */
const char * ECMD_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

  int i;
  for (i=0; i<iNumParams; i++)
    {
      /* check parameter "led" */
      if (strcmp(pcParam[i] , "led") == 0)   
      {
                /* Switch LED1 ON if 1 */
        if (strcmp(pcValue[i], "1") == 0) 
          printf("Led ON\n");
        else if (strcmp(pcValue[i], "0") == 0)
          printf("Led OFF\n");

      }


    }
  /* uri to send after cgi call*/
  return "/index.html";  
}
#endif



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/