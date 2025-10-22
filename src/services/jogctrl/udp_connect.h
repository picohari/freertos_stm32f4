#ifndef __UDP_CONNECT_H__
#define __UDP_CONNECT_H__


#include <stdint.h>
#include "jog_control.h"

#define UDP_REMOTE_PORT 58428   /* define the UDP remote connection port */
#define UDP_LOCAL_PORT  58427   /* define the UDP local connection port */


void udp_server_init(void);

void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);




#endif /* __UDP_CONNECT_H__ */