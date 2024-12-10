
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static gnrc_pktsnip_t *coap_send_request(gnrc_netif_t *netif, ipv4_addr_t *server_addr)
{
    gnrc_pktsnip_t *pkt, *payload, *coap;
    coap_pkt_t coap_pkt;
    uint8_t buf[64];

    /* Create CoAP packet */
    coap = gnrc_coap_pkt_init(buf, sizeof(buf), COAP_METHOD_POST, strlen(URI_PATH));
    if (coap == NULL) {
        puts("Error: cannot create CoAP packet");
        return NULL;
    }

    /* Set CoAP options */
    gnrc_coap_pkt_add_option(&coap_pkt, COAP_OPT_URI_PATH, (uint8_t *)URI_PATH, strlen(URI_PATH));

    /* Set CoAP payload */
    payload = gnrc_pktbuf_add(NULL, MESSAGE, strlen(MESSAGE), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: cannot create payload");
        gnrc_pktbuf_release(coap);
        return NULL;
    }
    gnrc_pkt_append(coap, payload);

    /* Prepare packet for sending */
    pkt = gnrc_netif_hdr_build(NULL, 0, (uint8_t *)server_addr, sizeof(ipv4_addr_t));
    if (pkt == NULL) {
        puts("Error: cannot create network interface header");
        gnrc_pktbuf_release(coap);
        return NULL;
    }
    gnrc_pkt_append(pkt, coap);

    return pkt;
}

int main(void)
{
    gnrc_netif_t *netif;
    ipv4_addr_t server_addr;
    gnrc_pktsnip_t *pkt;

    /* Initialize network interface */
    netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Convert server IP address to binary format */
    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return 1;
    }

    /* Send CoAP request */
    pkt = coap_send_request(netif, &server_addr);
    if (pkt == NULL) {
        puts("Error: cannot create CoAP request packet");
        return 1;
    }

    /* Send packet */
    if (gnrc_netapi_send(netif->pid, pkt) < 1) {
        puts("Error: cannot send CoAP request");
        gnrc_pktbuf_release(pkt);
        return 1;
    }

    puts("CoAP request sent successfully");

    return 0;
}
