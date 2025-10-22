#ifndef __ETH_IF_H
#define __ETH_IF_H


#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"


/* Exported types ------------------------------------------------------------*/

/* Global Ethernet handle*/
extern ETH_HandleTypeDef hETH;

/* Network interface structure */
extern struct netif gnetif;

/* Link status structure */
struct link_str {
  struct netif *netif;
  osSemaphoreId semaphore;  // Useless if no hardware interrupt is connected to PHY
};

/* Semaphore to signal Ethernet Link state update */
extern osSemaphoreId Netif_LinkSemaphore;

/* Ethernet link thread Argument */
extern struct link_str link_arg;


/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);
     
void ethernetif_set_link(void const *argument);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);

#endif /* __ETH_IF_H */
