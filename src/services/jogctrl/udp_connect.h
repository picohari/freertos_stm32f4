#ifndef __UDP_CONNECT_H__
#define __UDP_CONNECT_H__


#include <stdint.h>
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"
#include "linuxcnc_ctrl.h"


/* IP of remote host (LinuxCNC machine)*/
#define REMOTE_IP_ADDR0   192
#define REMOTE_IP_ADDR1   168
#define REMOTE_IP_ADDR2   0
#define REMOTE_IP_ADDR3   83

#define UDP_REMOTE_PORT 58428   /* define the UDP remote connection port */
#define UDP_LOCAL_PORT  58427   /* define the UDP local connection port */


extern ip4_addr_t remote_addr;


void udp_server_init(void);

void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip4_addr_t *addr, u16_t port);

void udp_send_jogstate(const JogState_t *pkt);


#endif /* __UDP_CONNECT_H__ */