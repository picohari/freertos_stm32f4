/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    06-May-2016
  * @brief   This file implements Ethernet network interface drivers for lwIP
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright © 2016 STMicroelectronics International N.V. 
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
#include "stm32f4xx_hal.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
  
#include "eth_if.h"
  
#include <string.h>

//#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* The time to block waiting for input. */
#define TIME_WAITING_FOR_INPUT                 ( 100 )
/* Stack size of the interface thread */
#define INTERFACE_THREAD_STACK_SIZE            ( 350 )

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END;/* Ethernet Rx MA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END;/* Ethernet Tx DMA Descriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

/* Semaphore to signal incoming packets */
osSemaphoreId s_xSemaphore = NULL;

/* Global Ethernet handle*/
ETH_HandleTypeDef hETH;

//static struct netif *s_pxNetIf = NULL;

/* Private function prototypes -----------------------------------------------*/
static void ethernetif_input( void const * argument );

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOs clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

/* Ethernet pins configuration ************************************************/
  /*
        RMII_REF_CLK ----------------------> PA1
        RMII_MDIO -------------------------> PA2
        RMII_MDC --------------------------> PC1
        RMII_MII_CRS_DV -------------------> PA7
        RMII_MII_RXD0 ---------------------> PC4
        RMII_MII_RXD1 ---------------------> PC5
        RMII_MII_RXER ---------------------> PG2
        RMII_MII_TX_EN --------------------> PG11
        RMII_MII_TXD0 ---------------------> PG13
        RMII_MII_TXD1 ---------------------> PB13
  */
  
  /* Configure PA1, PA2 and PA7 */
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL; 
  GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7 | GPIO_PIN_8;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure PC1, PC4 and PC5 */
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PG2, PG11, PG13 and PG14 */
  GPIO_InitStructure.Pin =  GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  /* Enable the Ethernet global Interrupt */
  HAL_NVIC_SetPriority(ETH_IRQn, 0x7, 0);
  HAL_NVIC_EnableIRQ(ETH_IRQn);
  
  /* Enable ETHERNET clock  */
  __HAL_RCC_ETH_CLK_ENABLE();

  if (heth->Init.MediaInterface == ETH_MEDIA_INTERFACE_MII)
  {
    /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  }

}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
  osSemaphoreRelease(s_xSemaphore);
}

/**
  * @brief  Ethernet IRQ Handler
  * @param  None
  * @retval None
  * @Note   defined in stm32f4xx_it.c
  */
#if 0
void ETHERNET_IRQHandler(void)
{
  HAL_ETH_IRQHandler(&hETH);
}
#endif

/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH) 
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{
  
  uint32_t regvalue  = 0;
  uint32_t regvalue1 = 0;
  uint32_t regvalue2 = 0;

  uint8_t macaddress[6]= { MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5 };
  
  hETH.Instance = ETH;  
  hETH.Init.MACAddr = macaddress;
  hETH.Init.AutoNegotiation = ETH_AUTONEGOTIATION_DISABLE;
  hETH.Init.Speed = ETH_SPEED_10M;
  hETH.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
  hETH.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
  hETH.Init.RxMode = ETH_RXINTERRUPT_MODE;
  hETH.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  hETH.Init.PhyAddress = DP83848_PHY_ADDRESS;
  
  /* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
  if (HAL_ETH_Init(&hETH) == HAL_OK)
  {
    /* Set netif link flag */
    netif->flags |= NETIF_FLAG_LINK_UP;
  }
  
  /* Initialize Tx Descriptors list: Chain Mode */
  HAL_ETH_DMATxDescListInit(&hETH, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
     
  /* Initialize Rx Descriptors list: Chain Mode  */
  HAL_ETH_DMARxDescListInit(&hETH, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);
  
  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set netif MAC hardware address */
  netif->hwaddr[0] =  MAC_ADDR0;
  netif->hwaddr[1] =  MAC_ADDR1;
  netif->hwaddr[2] =  MAC_ADDR2;
  netif->hwaddr[3] =  MAC_ADDR3;
  netif->hwaddr[4] =  MAC_ADDR4;
  netif->hwaddr[5] =  MAC_ADDR5;

  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  //s_pxNetIf =netif;

  /* create a binary semaphore used for informing ethernetif of frame reception */
  osSemaphoreDef(SEM);
  s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1 );

  /* create the task that handles the ETH_MAC */
  osThreadDef(eth0, ethernetif_input, osPriorityRealtime, 0, INTERFACE_THREAD_STACK_SIZE);
  osThreadCreate (osThread(eth0), netif);

  /* Enable MAC and DMA transmission and reception */
  HAL_ETH_Start(&hETH);

   /* Read Register Configuration */
  HAL_ETH_ReadPHYRegister(&hETH, 0x02, &regvalue1);
  HAL_ETH_ReadPHYRegister(&hETH, 0x03, &regvalue2);

  //debug("PHY: 0x%x%x", regvalue1, regvalue2 );


#if 0
  /**** Configure PHY to generate an interrupt when Eth Link state changes ****/
  /* Read Register Configuration */
  HAL_ETH_ReadPHYRegister(&hETH, PHY_MICR, &regvalue);
  
  regvalue |= (PHY_MICR_INT_EN | PHY_MICR_INT_OE);

  /* Enable Interrupts */
  HAL_ETH_WritePHYRegister(&hETH, PHY_MICR, regvalue );
  
  /* Read Register Configuration */
  HAL_ETH_ReadPHYRegister(&hETH, PHY_MISR, &regvalue);
  
  regvalue |= PHY_MISR_LINK_INT_EN;
    
  /* Enable Interrupt on change of link status */
  HAL_ETH_WritePHYRegister(&hETH, PHY_MISR, regvalue);
#endif

#if 0
  HAL_ETH_ReadPHYRegister(&hETH, 0x00, &regvalue2);
  LOG_UART("0x00: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x01, &regvalue2);
  LOG_UART("0x01: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x02, &regvalue2);
  LOG_UART("0x02: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x03, &regvalue2);
  LOG_UART("0x03: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x04, &regvalue2);
  LOG_UART("0x04: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x05, &regvalue2);
  LOG_UART("0x05: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x06, &regvalue2);
  LOG_UART("0x06: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x07, &regvalue2);
  LOG_UART("0x07: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x10, &regvalue2);
  LOG_UART("0x10: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x11, &regvalue2);
  LOG_UART("0x11: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x12, &regvalue2);
  LOG_UART("0x12: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x13, &regvalue2);
  LOG_UART("0x13: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x14, &regvalue2);
  LOG_UART("0x14: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x15, &regvalue2);
  LOG_UART("0x15: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x16, &regvalue2);
  LOG_UART("0x16: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x17, &regvalue2);
  LOG_UART("0x17: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x18, &regvalue2);
  LOG_UART("0x18: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x19, &regvalue2);
  LOG_UART("0x19: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x1A, &regvalue2);
  LOG_UART("0x1A: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x1B, &regvalue2);
  LOG_UART("0x1B: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x1C, &regvalue2);
  LOG_UART("0x1C: 0x%x", regvalue2 );

  HAL_ETH_ReadPHYRegister(&hETH, 0x1D, &regvalue2);
  LOG_UART("0x1D: 0x%x", regvalue2 );
#endif

}


/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
  err_t errval;
  struct pbuf *q;
  uint8_t *buffer = (uint8_t *)(hETH.TxDesc->Buffer1Addr);
  __IO ETH_DMADescTypeDef *DmaTxDesc;
  uint32_t framelength = 0;
  uint32_t bufferoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t payloadoffset = 0;

  DmaTxDesc = hETH.TxDesc;
  bufferoffset = 0;
  
  /* copy frame from pbufs to driver buffers */
  for(q = p; q != NULL; q = q->next)
  {
    /* Is this buffer available? If not, goto error */
    if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
      errval = ERR_USE;
      goto error;
    }
    
    /* Get bytes in current lwIP buffer */
    byteslefttocopy = q->len;
    payloadoffset = 0;
    
    /* Check if the length of data to copy is bigger than Tx buffer size*/
    while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
    {
      /* Copy data to Tx buffer*/
      memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );
      
      /* Point to next descriptor */
      DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);
      
      /* Check if the buffer is available */
      if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
      {
        errval = ERR_USE;
        goto error;
      }
      
      buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);
      
      byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
      payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
      framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
      bufferoffset = 0;
    }
    
    /* Copy the remaining bytes */
    memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
    bufferoffset = bufferoffset + byteslefttocopy;
    framelength = framelength + byteslefttocopy;
  }
  
  /* Prepare transmit descriptors to give to DMA */ 
  HAL_ETH_TransmitFrame(&hETH, framelength);
  
  errval = ERR_OK;
  
error:
  
  /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
  if ((hETH.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
  {
    /* Clear TUS ETHERNET DMA flag */
    hETH.Instance->DMASR = ETH_DMASR_TUS;
    
    /* Resume DMA transmission*/
    hETH.Instance->DMATPDR = 0;
  }
  return errval;
}


/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf * low_level_input(struct netif *netif)
{
  struct pbuf *p = NULL, *q = NULL;
  u16_t len = 0;
  uint8_t *buffer;
  __IO ETH_DMADescTypeDef *dmarxdesc;
  uint32_t bufferoffset = 0;
  uint32_t payloadoffset = 0;
  uint32_t byteslefttocopy = 0;
  uint32_t i=0;
  
  /* get received frame */
  if(HAL_ETH_GetReceivedFrame_IT(&hETH) != HAL_OK)
    return NULL;

  /* Obtain the size of the packet and put it into the "len" variable. */
  len = hETH.RxFrameInfos.length;
  buffer = (uint8_t *)hETH.RxFrameInfos.buffer;
  
  if (len > 0)
  {
    /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  }
  
  if (p != NULL)
  {
    dmarxdesc = hETH.RxFrameInfos.FSRxDesc;
    bufferoffset = 0;
    
    for(q = p; q != NULL; q = q->next)
    {
      byteslefttocopy = q->len;
      payloadoffset = 0;

      /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
      while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
      {
        /* Copy data to pbuf*/
        memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

        /* Point to next descriptor */
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
        buffer = (unsigned char *)(dmarxdesc->Buffer1Addr);

        byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
        payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
        bufferoffset = 0;
      }

      /* Copy remaining data in pbuf */
      memcpy( (u8_t*)((u8_t*)q->payload + payloadoffset), (u8_t*)((u8_t*)buffer + bufferoffset), byteslefttocopy);
      bufferoffset = bufferoffset + byteslefttocopy;
    }
  }
  
  /* Release descriptors to DMA */
  dmarxdesc = hETH.RxFrameInfos.FSRxDesc;

  /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
  for (i=0; i< hETH.RxFrameInfos.SegCount; i++)
  {  
    dmarxdesc->Status |= ETH_DMARXDESC_OWN;
    dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
  }
 
  /* Clear Segment_Count */
  hETH.RxFrameInfos.SegCount =0;

  
  /* When Rx Buffer unavailable flag is set: clear it and resume reception */
  if ((hETH.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
  {
    /* Clear RBUS ETHERNET DMA flag */
    hETH.Instance->DMASR = ETH_DMASR_RBUS;
    /* Resume DMA reception */
    hETH.Instance->DMARPDR = 0;
  }
  return p;
}


/**
  * @brief This function is the ethernetif_input task, it is processed when a packet 
  * is ready to be read from the interface. It uses the function low_level_input() 
  * that should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure for this ethernetif
  */
void ethernetif_input( void const * argument )
{
  struct pbuf *p;
  struct netif *netif = (struct netif *) argument;
  
  for( ;; )
  {
    if (osSemaphoreWait( s_xSemaphore, TIME_WAITING_FOR_INPUT)==osOK)
    {
      do
      {
        p = low_level_input( netif );
        if (p != NULL)
        {
          if (netif->input(p, netif) != ERR_OK )
          //if (s_pxNetIf->input( p, s_pxNetIf) != ERR_OK )
          {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
            pbuf_free(p);
          }
        }
      }while(p!=NULL);
    }
  }
}


/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}


u32_t sys_now(void)
{
  return HAL_GetTick();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/