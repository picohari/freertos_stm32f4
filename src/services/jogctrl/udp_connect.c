

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "log.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "udp_connect.h"
#include "linuxcnc_ctrl.h"



/**
* @brief  Initialize the server application.
* @param  None
* @retval None
*/
void udp_server_init(void)
{
    struct udp_pcb *upcb;
    err_t err;

    /* Create a new UDP control block  */
    upcb = udp_new();

    if (upcb) {
        /* Bind the upcb to the UDP_PORT port */
        /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
        err = udp_bind(upcb, IP_ADDR_ANY, UDP_LOCAL_PORT);

        if(err == ERR_OK) {
            /* Set a receive callback for the upcb */
            udp_recv(upcb, udp_server_receive_callback, NULL);
        }
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

    /* Check incoming packet */
    if (p == NULL) return;

    /* Print received data */
    //LOG_DEBUG("Got UDP packet: len=%d from %s:%d", p->len, ipaddr_ntoa(addr), port);
    //LOG_DEBUG("Data: %.*s", p->len, (char *)p->payload);

    /* Check if packet is for LinuxCNC controller */
    if (p->len >= sizeof(MachineState_t)) {

        /* Store parsed data */
        MachineState_t pkt;

        if (LinuxCNC_ParseUDP((const uint8_t *)p->payload, p->len, &pkt)) {

            /* Copy parsed data to LinuxCNC machine state */
            LinuxCNC_SetState(&pkt);

            /* Example: print the received values
            LOG_DEBUG("Received axis data: X=%.3f Y=%.3f Z=%.3f Homed=%d",
                       pkt.pos_x,
                       pkt.pos_y,
                       pkt.pos_z,
                       pkt.homed);
            */

        } else {
            LOG_DEBUG("Invalid packet format (or corrupted)");
        }
    } else {
        LOG_DEBUG("Received packet too small: %d bytes", p->len);
    }


    // Optionally send echo for test
    // udp_sendto(upcb, p, addr, port);


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



#if 0

#include <string.h>   // memcpy
#include <stdio.h>    // for printf/debugging

// These functions are just placeholders for your networking code.
// Replace them with your platform-specific UDP send/receive logic.

void udp_send_jog(const JogPacket *pkt)
{
    // Serialize and send over UDP
    // e.g., sendto(socket, pkt, sizeof(JogPacket), ...)
    printf("Send JOG: Axis=%d, Δ=%.4f mm\n", pkt->axis, pkt->delta_mm);
}

int udp_recv_axis_position(AxisPositionPacket *pkt)
{
    // Non-blocking receive example
    // Replace with real socket read or queue check.
    // Return 1 if new data received, 0 if no new data.
    return 0;
}

#endif
