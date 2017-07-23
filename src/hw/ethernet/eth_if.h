#ifndef __ETH_IF_H__
#define __ETH_IF_H__


#include "lwip/err.h"
#include "lwip/netif.h"
#include "cmsis_os.h"


/* Exported types ------------------------------------------------------------*/

/* Global Ethernet handle*/
extern ETH_HandleTypeDef hETH;

extern struct netif gnetif;

struct link_str {
  struct netif *netif;
  osSemaphoreId semaphore;
};

/* Exported functions ------------------------------------------------------- */
err_t ethernetif_init(struct netif *netif);



#endif
