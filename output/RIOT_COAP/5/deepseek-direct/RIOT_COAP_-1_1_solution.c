
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
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    sock_udp_t sock;
    gnrc_pktsnip_t *pkt;
    gnrc_coap_pkt_t coap_pkt;
    uint8_t buf[128];

    /* Resolve the server IP address */
    if (sock_dns_query(SERVER_IP, &remote.addr.ipv4, NULL) < 0) {
        puts("Error: unable to resolve server IP address");
        return;
    }

    /* Initialize the UDP socket */
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return;
    }

    /* Create a CoAP packet */
    pkt = gnrc_pktbuf_add(NULL, buf, sizeof(buf), GNRC_NETTYPE_UNDEF);
    if (pkt == NULL) {
        puts("Error: unable to allocate packet buffer");
        return;
    }

    /* Initialize the CoAP packet */
    if (gnrc_coap_init_pkt(&coap_pkt, pkt) < 0) {
        puts("Error: unable to initialize CoAP packet");
        return;
    }

    /* Set the CoAP method to POST */
    gnrc_coap_set_code(&coap_pkt, COAP_CODE_POST);

    /* Set the CoAP URI path */
    gnrc_coap_add_uri_path(&coap_pkt, URI_PATH);

    /* Set the CoAP payload */
    gnrc_coap_set_payload(&coap_pkt, MESSAGE, strlen(MESSAGE));

    /* Send the CoAP packet */
    if (gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, pkt) < 0) {
        puts("Error: unable to send CoAP packet");
        return;
    }

    /* Close the UDP socket */
    sock_udp_close(&sock);

    puts("CoAP POST request sent successfully");
}

int main(void)
{
    /* Initialize the network interface */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Wait for the network interface to be up */
    while (gnrc_netif_get_state(netif) != NETOPT_STATE_READY) {
        xtimer_sleep(1);
    }

    /* Send the CoAP request */
    _send_coap_request();

    return 0;
}
