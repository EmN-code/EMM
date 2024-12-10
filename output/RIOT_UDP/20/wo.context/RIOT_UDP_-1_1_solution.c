#include "net.h"
#include "msg.h"
#include "kernel_types.h"
#include "thread.h"

#include <stdio.h>
#include <string.h>
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netdev.h"
#include "net/gnrc.h"

#define SERVER_ADDR  "47.102.103.1"
#define SERVER_PORT  20001
#define DEVICE_STATE "work"
#define SEND_INTERVAL (5U)

static msg_t _main_msg_queue[8];

int main(void) {
    /* Initialize message queue */
    msg_init_queue(_main_msg_queue, 8);

    /* Initialize UDP */
    gnrc_netdev_t *netdev = gnrc_netdev_get_by_type(NETDEV_ANY_TYPE);
    if (!netdev) {
        puts("Error: no net device available.");
        return 1;
    }

    /* Create UDP connection */
    gnrc_udp_entry_t socket;
    int res = gnrc_udp_bind(&socket, NULL, 0);
    if (res < 0) {
        puts("Error: unable to bind socket.");
        return 1;
    }

    /* Set server address and port */
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, SERVER_ADDR);

    while (1) {
        /* Send device state */
        gnrc_pktsnip_t *payload;
        payload = gnrc_pktbuf_add(NULL, DEVICE_STATE, strlen(DEVICE_STATE), GNRC_NETTYPE_UNDEF);

        gnrc_pktsnip_t *udp;
        udp = gnrc_udp_hdr_build(payload, NULL, 0, SERVER_PORT);

        gnrc_pktsnip_t *netif;
        netif = gnrc_netif_hdr_build(udp, NULL, 0);

        gnrc_send(netif);

        /* Wait 5 seconds */
        xtimer_sleep(SEND_INTERVAL);
    }

    return 0;
}