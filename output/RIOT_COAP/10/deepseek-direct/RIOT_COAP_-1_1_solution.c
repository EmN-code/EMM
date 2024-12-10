
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _send_coap_request(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (sock_udp_name2ep(&remote, SERVER_IP, NULL) < 0) {
        puts("Error: unable to resolve server IP");
        return;
    }

    gnrc_pktsnip_t *pkt = gnrc_coap_new_message(NULL, COAP_TYPE_CON, COAP_METHOD_POST, 0);
    if (pkt == NULL) {
        puts("Error: unable to create CoAP message");
        return;
    }

    gnrc_coap_set_payload(pkt, MESSAGE, strlen(MESSAGE));
    gnrc_coap_add_uri_path(pkt, URI_PATH);

    gnrc_netreg_entry_t me = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, thread_getpid());
    gnrc_netreg_register(GNRC_NETTYPE_UDP, &me);

    gnrc_netapi_send(gnrc_pktbuf_get_ptr(pkt), &remote);
    gnrc_pktbuf_release(pkt);
}

int main(void)
{
    puts("CoAP client starting");

    // Initialize network interfaces
    gnrc_netif_init_by_dev();

    // Wait for the network interface to be up
    while (gnrc_netif_numof() == 0) {
        xtimer_sleep(1);
    }

    // Send the CoAP request
    _send_coap_request();

    puts("CoAP request sent");

    return 0;
}
