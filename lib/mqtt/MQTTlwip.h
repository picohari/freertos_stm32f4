/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef __MQTT_LINUX_
#define __MQTT_LINUX_

//#include "ch.h"
//#include "chvt.h"
 
#include "lwip/api.h"
#include "lwip/netbuf.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"




typedef struct Timer 
{
	TickType_t xTicksToWait;
	TimeOut_t xTimeOut;
} Timer;



typedef struct Network Network;


struct Network
{
	struct netconn *netconn;
	struct netbuf *netbuf;
	int netbuf_offset;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};



void InitTimer(Timer* timer);

char expired(Timer* timer);

void countdown(Timer* timer, unsigned int timeout); 

void countdown_ms(Timer* timer, unsigned int timeout_ms);

int left_ms(Timer* timer); 



int mqtt_lwip_read(Network* n, unsigned char* buffer, int len, int timeout_ms);

int mqtt_lwip_write(Network* n, unsigned char* buffer, int len, int timeout_ms);

void mqtt_lwip_disconnect(Network* n);

int mqtt_lwip_connect(Network* n, const ip_addr_t *ip_addr, int port);

void mqtt_lwip_init(Network* n);







#if 0

typedef struct Timer Timer;

struct Timer {
	systime_t end_time;
};



typedef struct Network Network;

struct Network
{
	struct netconn *netconn;
	struct netbuf *netbuf;
	int netbuf_offset;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

char expired(Timer*);
void countdown_ms(Timer*, unsigned int);
void countdown(Timer*, unsigned int);
int left_ms(Timer*);

void InitTimer(Timer*);

int mqtt_lwip_read(Network*, unsigned char*, int, int);
int mqtt_lwip_write(Network*, unsigned char*, int, int);
void mqtt_lwip_disconnect(Network*);
void NewNetwork(Network*);

int ConnectNetwork(Network*, char*, int);
#endif





#endif
