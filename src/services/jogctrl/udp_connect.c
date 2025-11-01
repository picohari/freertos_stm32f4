

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"

#include "udp_connect.h"
#include "linuxcnc_ctrl.h"
#include "axis_menu.h"


struct udp_pcb *upcb;

ip_addr_t remote_addr;

uint32_t remote_port;

/**
* @brief  Initialize the server application.
* @param  None
* @retval None
*/
void udp_server_init(void)
{
    err_t err;

    /* Create a new UDP control block  */
    upcb = udp_new();

    if (upcb) {

        /* Assign destination IP address and port */
        IP4_ADDR(&remote_addr, REMOTE_IP_ADDR0, REMOTE_IP_ADDR1, REMOTE_IP_ADDR2, REMOTE_IP_ADDR3 );
        remote_port = UDP_REMOTE_PORT;

        /* Bind the upcb to the UDP_PORT port */
        /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
        err = udp_bind(upcb, IP_ADDR_ANY, UDP_LOCAL_PORT);

        if(err == ERR_OK) {
            /* Set a receive callback for the upcb */
            udp_recv(upcb, udp_server_receive_callback, NULL);
        }

        /* Inform GUI about network settings */
        uint32_t ip = lwip_ntohl(remote_addr.addr);
        axis_helper_setRemoteIP(&ip);

        //uint8_t expected = sizeof(HalState_t);
        //LOG_DEBUG("HalState_t: %d", expected);
        
        
        #if 0
        err = udp_connect(upcb, &remote_addr, UDP_REMOTE_PORT);

        uint8_t iptxt[20];
        sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&remote_addr));
        if(err != ERR_OK) {
            LOG_DEBUG("Could not connect to: %s", iptxt);
        } else {
            LOG_DEBUG("Connected JOG to: %s", iptxt);
        }
        #endif
    }
}



/**
* @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
* @param arg user supplied argument (udp_pcb.recv_arg)
* @param pcb the udp_pcb which received data
* @param p the packet buffer that was received
* @param addr the remote IP address from which the packet was received
* @param port the remote port from which the packet was received
* @retval None
*/
void udp_server_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    /* Not used yet */
    (void) arg;
    (void) upcb;
    (void) addr;
    (void) port;

    /* Check incoming packet */
    if (p == NULL) return;

    /* Print received data */
    //LOG_DEBUG("Got UDP packet: len=%d from %s:%d", p->len, ipaddr_ntoa(addr), port);
    //LOG_DEBUG("Data: %.*s", p->len, (char *)p->payload);

    #if 0
    /* Check if packet is for LinuxCNC controller */
    if (p->len >= sizeof(MachineState_t)) {

        /* Store parsed data */
        MachineState_t pkt;

        if (LinuxCNC_ParseUDP((const uint8_t *)p->payload, p->len, &pkt)) {
            /* Copy parsed data to LinuxCNC machine state */
            LinuxCNC_SetMachineState(&pkt);
        } else {
            LOG_DEBUG("Invalid packet format (or corrupted)");
        }
    } else {
        LOG_DEBUG("Received packet too small: %d bytes", p->len);
    }
    #endif

    /* Check if packet is for LinuxCNC controller */
    if (p->len >= sizeof(HalState_t)) {

        /* Store parsed data */
        HalState_t     pkt;

        if (LinuxCNC_ParseHAL((const uint8_t *)p->payload, p->len, &pkt)) {
            /* Copy parsed data to LinuxCNC machine state */
            LinuxCNC_SetHalState(&pkt);
        } else {
            LOG_DEBUG("Invalid packet format (or corrupted)");
        }
    } else {
        LOG_DEBUG("Received packet too small: %d bytes", p->len);
    }


    #if 0
    /* Connect to the remote client */
    udp_connect(upcb, addr, UDP_REMOTE_PORT);

    /* Tell the client that we have accepted it (send back echo - for testing)*/
    udp_send(upcb, p);
    #endif 

    /* free the UDP connection, so we can accept new clients */
    udp_disconnect(upcb);

    /* Free the p buffer */
    pbuf_free(p);
   
}

void udp_send_jogstate(const JogState_t *pkt)
{
    /* Get size of packet */
    uint16_t len = sizeof(JogState_t);

    /* Allocate pbuf in PBUF_TRANSPORT-Pool */
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    if (!p) {
        LOG_DEBUG("Error: pbuf_alloc failed!");
        return;
    }

    /* Copy JOG data to UDP payload */
    memcpy(p->payload, pkt, len);

    /* Connect to remote host */
    //udp_connect(upcb, addr, UDP_REMOTE_PORT);

    /* Use address set by menu system */
    uint32_t ip;
    axis_helper_getRemoteIP(&ip);
    remote_addr.addr = lwip_htonl(ip);

    /* Send packet */
    err_t err = udp_sendto(upcb, p, &remote_addr, UDP_REMOTE_PORT);
    if (err != ERR_OK) {
        LOG_DEBUG("udp_sendto failed: %d\n", err);
    }

    /* Free the p buffer */
    pbuf_free(p);    
}

