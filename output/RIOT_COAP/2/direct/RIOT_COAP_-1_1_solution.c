#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/nanocoap_sock.h"
#include "net/af.h"
#include "net/protomux.h"
#include "net/sock.h"
#include "net/gnrc.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/udp.h"
#include "xtimer.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

static gnrc_netif_t *get_ipv6_netif(void) {
    gnrc_netif_t *netif = NULL;
    netif = gnrc_netif_iter(netif);
    if (netif) {
        return netif;
    }
    return NULL;
}

static int send_coap_post(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_UNSPEC_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDR);
    remote.port = COAP_SERVER_PORT;

    uint8_t buf[128];
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    /* create CoAP POST request */
    size_t len = gcoap_req_send(buf, sizeof(buf), COAP_METHOD_POST,
                                (const char *)COAP_URI_PATH,
                                strlen(COAP_URI_PATH),
                                (const uint8_t *)COAP_MSG,
                                strlen(COAP_MSG),
                                (sock_udp_ep_t *)&remote);
    
    if (len <= 0) {
        printf("Error creating CoAP request\n");
        return -1;
    }

    ssize_t res = sock_udp_send(&sock, buf, len, &remote);
    if (res < 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }
    printf("CoAP POST request sent\n");
    sock_udp_close(&sock);

    return 0;
}

int main(void) {
    xtimer_sleep(1); // To ensure network is ready

    gnrc_netif_t *netif = get_ipv6_netif();
    if (!netif) {
        printf("No network interface found\n");
        return -1;
    }

    while (1) {
        send_coap_post();
        xtimer_sleep(10); // wait before sending the next message
    }

    return 0;
}