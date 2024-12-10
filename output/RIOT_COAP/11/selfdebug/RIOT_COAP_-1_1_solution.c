#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include <stdio.h>
#include <string.h>
#include <net/netdev.h>
#include <net/gnrc/netif.h>
#include <net/gnrc.h>
#include <net/gcoap.h>

static gnrc_netif_t *get_netif(void) {
    gnrc_netif_t *netif = NULL;
    while ((netif = gnrc_netif_iter(netif))) {
        if (netif->dev->driver->type == NETDEV_TYPE_IEEE802154) {
            return netif;
        }
    }
    return NULL;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");
    remote.port = CONFIG_GCOAP_PORT;

    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") == -EPROTO) {
        printf("Failed to initialize request\n");
        return 1;
    }

    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    gcoap_finish(pdu, strlen("Hello, COAP Cloud"));

    gcoap_send(&pdu, &remote, _resp_handler, NULL);

    return 0;
}