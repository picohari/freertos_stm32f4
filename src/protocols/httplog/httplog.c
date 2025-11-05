/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Socket_RTOS/Src/app_ethernet.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    17-February-2017
  * @brief   Ethernet specefic module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "XCore407I.h"
  
#include "httplog.h"

#include "lwip/tcp.h"
#include "lwip/err.h"

#if 0
#include "lwip/debug.h"
#include "lwip/opt.h"
#include "lwip/timeouts.h"
#include "lwip/ip_addr.h"
#include "lwip/dns.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/memp.h"
#endif

#define DEBUG_HTTPLOG			
#include "log.h"
  
#ifdef DEBUG_HTTPLOG
#define HTLOG_DEBUG(a...)  LOG_DEBUG("httplog: " a)
#else
#define HTLOG_DEBUG(a...)
#endif

// first string is the GET part including the path
static const char get_string_head[] = "POST " CONF_HTTPLOG_PATH "?";

// and the http footer including the http protocol version and the server name
static const char get_string_foot[] = " HTTP/1.1\n" "Host: " CONF_HTTPLOG_SERVICE "\r\n\r\n";


/* Holds the ULR request to be sent to the httplog server (printf like buffer for parameters) */
static char *httplog_tmp_buf;

/* Holds the HTTP header and paket request */
static char htlog_data[HTTPLOG_BUFFER_LEN * 4];


/* HTTPlog protocol control block */
struct tcp_pcb *htlog_pcb;

/* HTTPlog protocol states */
enum htlog_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};

/* HTTPlog structure to be passed as argument to the tcp callbacks */
struct htlog_client
{
  enum htlog_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};








static void tcp_httplog_close(struct tcp_pcb *tpcb, struct htlog_client * es )
{
	/* remove callbacks */
	tcp_recv(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_poll(tpcb, NULL,0);

	if (es != NULL) {
		mem_free(es);
	}

	if (httplog_tmp_buf != NULL) {
		vPortFree((char *)httplog_tmp_buf);
		httplog_tmp_buf = NULL;
	}

	/* close tcp connection */
	tcp_close(tpcb);
  
}

static void tcp_httplog_send(struct tcp_pcb *tpcb, struct htlog_client * es)
{
	struct pbuf *ptr;
	err_t wr_err = ERR_OK;

	while ((wr_err == ERR_OK) && (es->p_tx != NULL) && (es->p_tx->len <= tcp_sndbuf(tpcb))) {

		/* get pointer on pbuf from es structure */
		ptr = es->p_tx;

		/* enqueue data for transmission */
		wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

		if (wr_err == ERR_OK) { 
			/* continue with next pbuf in chain (if any) */
			es->p_tx = ptr->next;

			if(es->p_tx != NULL) {
				/* increment reference count for es->p */
				pbuf_ref(es->p_tx);
			}

			/* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
			pbuf_free(ptr);

		} else if(wr_err == ERR_MEM) {
			/* we are low on memory, try later, defer to poll */
			es->p_tx = ptr;
		} else {
		/* other problem ?? */
		}
	}
}

static err_t tcp_httplog_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct htlog_client *es;

	LWIP_UNUSED_ARG(len);

	es = (struct htlog_client *)arg;

	if(es->p_tx != NULL) {
		/* still got pbufs to send */
		tcp_httplog_send(tpcb, es);
	}

	return ERR_OK;
}






static err_t tcp_httplog_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ 
	struct htlog_client *es;
	err_t ret_err;

	LWIP_ASSERT("arg != NULL", arg != NULL);

	es = (struct htlog_client *)arg;

	/* if we receive an empty tcp frame from server => close connection */
	if (p == NULL) {
		/* remote host closed connection */
		es->state = ES_CLOSING;
		if(es->p_tx == NULL) {
			/* we're done sending, close connection */
			tcp_httplog_close(tpcb, es);
		} else {    
			/* send remaining data*/
			tcp_httplog_send(tpcb, es);
		}
		ret_err = ERR_OK;
	}   
	/* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
	else if(err != ERR_OK) {
		/* free received pbuf*/
		if (p != NULL) {
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(es->state == ES_CONNECTED) {
		/* increment message count */
		//message_count++;
		     
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);  

		pbuf_free(p);
		tcp_httplog_close(tpcb, es);
		ret_err = ERR_OK;
	}

	/* data received when connection already closed */
	else {
		/* Acknowledge data reception */
		tcp_recved(tpcb, p->tot_len);

		/* free pbuf and do nothing */
		pbuf_free(p);
		ret_err = ERR_OK;
	}

	return ret_err;
}

static err_t tcp_httplog_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct htlog_client *es;

	es = (struct htlog_client*)arg;
	if (es != NULL) {
		if (es->p_tx != NULL) {
			/* there is a remaining pbuf (chain) , try to send data */
			tcp_httplog_send(tpcb, es);
		} else {
			/* no remaining pbuf (chain)  */
			if(es->state == ES_CLOSING) {
				/* close tcp connection */
				tcp_httplog_close(tpcb, es);
			}
		}
		ret_err = ERR_OK;
	} else {
		/* nothing to be done */
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}

	return ret_err;
}











/**
  * @brief Function called when TCP connection established
  * @param tpcb: pointer on the connection contol block
  * @param err: when connection correctly established err should be ERR_OK 
  * @retval err_t: returned error 
  */
static err_t httplog_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	LWIP_UNUSED_ARG(arg);

	struct htlog_client *es = NULL;

	if (err == ERR_OK) {
		/* allocate structure es to maintain tcp connection informations */
		es = (struct htlog_client *)mem_malloc(sizeof(struct htlog_client));

		if (es != NULL) {
			es->state = ES_CONNECTED;
			es->pcb = tpcb;

			strcat((char *)htlog_data,  get_string_head);

			strcat((char *)htlog_data,  httplog_tmp_buf);

			strcat((char *)htlog_data,  get_string_foot);

			//sprintf((char*)data, "sending tcp client message %d", (int)message_count);

			/* allocate pbuf */
			es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)htlog_data) , PBUF_POOL);

			if (es->p_tx) {

				/* copy data to pbuf */
				pbuf_take(es->p_tx, (char*)htlog_data, strlen((char*)htlog_data));

				/* pass newly allocated es structure as argument to tpcb */
				tcp_arg(tpcb, es);

				/* initialize LwIP tcp_recv callback function */ 
				tcp_recv(tpcb, tcp_httplog_recv);

				/* initialize LwIP tcp_sent callback function */
				tcp_sent(tpcb, tcp_httplog_sent);

				/* initialize LwIP tcp_poll callback function */
				tcp_poll(tpcb, tcp_httplog_poll, 1);

				/* send data */
				tcp_httplog_send(tpcb,es);

				return ERR_OK;
			}

		} else {
			/* close connection */
			tcp_httplog_close(tpcb, es);

			/* return memory allocation error */
			return ERR_MEM;  
		}
	} else {
		/* close connection */
		tcp_httplog_close(tpcb, es);
	}

	return err;
}



static void httplog_dns_query_cb(const char* hostname, const ip_addr_t *ipaddr, void *arg)
{
	//LWIP_UNUSED_ARG(hostname);
	LWIP_UNUSED_ARG(arg);

	//printf("%s: %s\n", name, addr ? ip_ntoa(addr) : "<not found>");

	if (ipaddr != NULL) {

		/* Address resolved, send request */
		LWIP_DEBUGF(HTTPLOG_DEBUG, ("httplog: Server %s is: %s\n", hostname, ipaddr_ntoa(ipaddr)));

		/* create new tcp pcb */
	  	htlog_pcb = tcp_new();

		if (htlog_pcb != NULL) {
			/* connect to destination address/port */
			tcp_connect(htlog_pcb, ipaddr, 80, httplog_connected_cb);
		}

	} else {
	/* DNS resolving failed -> try another server */
	LWIP_DEBUGF(HTTPLOG_DEBUG, ("httplog: Failed to lookup server address!\n"));

	// TODO: Try again
	//sntp_try_next_server(NULL);
	}

}

static void httplog_resolve_address(void)
{
	ip4_addr_t httplog_server_address;

#if 0
	ip_addr_set_zero(&httplog_server_address);

	if (dns_gethostbyname(CONF_HTTPLOG_SERVICE, &httplog_server_address, httplog_dns_query_cb, 0) == ERR_OK) {
		httplog_dns_query_cb(CONF_HTTPLOG_SERVICE, &httplog_server_address, 0);
	}
#else
	#ifdef HOME_NET
	IP4_ADDR(&httplog_server_address, 192, 168, 1, 74);
	#else
	IP4_ADDR(&httplog_server_address, 192, 168, 200, 150);
	#endif
	httplog_dns_query_cb(CONF_HTTPLOG_SERVICE, &httplog_server_address, 0);
#endif


}

static uint8_t httplog_buffer_empty(void)
{
  if (httplog_tmp_buf == 0) {
    httplog_tmp_buf = (char *)pvPortMalloc(HTTPLOG_BUFFER_LEN);
    if (httplog_tmp_buf != 0)
      return 1;
  }

  return 0;
}

uint8_t httplog(const char *message, ...)
{
  uint8_t result = httplog_buffer_empty();

  if (result) {
    va_list va;
    va_start(va, message);
    vsnprintf(httplog_tmp_buf, HTTPLOG_BUFFER_LEN, message, va);
    va_end(va);
    httplog_tmp_buf[HTTPLOG_BUFFER_LEN - 1] = 0;

	LWIP_DEBUGF(HTTPLOG_DEBUG, ("httplog: Message: %s\n", httplog_tmp_buf));

    httplog_resolve_address();
  }
  return result;
}
