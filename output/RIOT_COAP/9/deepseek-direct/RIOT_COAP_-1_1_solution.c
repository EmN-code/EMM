
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
    uint8_t payload_buf[] = MESSAGE;

    /* Create CoAP packet */
    coap = gnrc_coap_pkt_init(NULL, 0, COAP_METHOD_POST, strlen(URI_PATH));
    if (coap == NULL) {
        puts("Error: cannot create CoAP packet");
        return NULL;
    }

    /* Set CoAP options */
    gnrc_coap_pkt_set_type(coap, COAP_TYPE_CON);
    gnrc_coap_pkt_set_code(coap, COAP_METHOD_POST);
    gnrc_coap_pkt_set_id(coap, 12345);
    gnrc_coap_pkt_add_option(coap, COAP_OPT_URI_PATH, URI_PATH, strlen(URI_PATH));

    /* Add payload */
    payload = gnrc_pktbuf_add(NULL, payload_buf, sizeof(payload_buf) - 1, GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        puts("Error: cannot add payload");
        gnrc_pktbuf_release(coap);
        return NULL;
    }
    gnrc_pkt_append(coap, payload);

    /* Create UDP packet */
    pkt = gnrc_udp_hdr_build(coap, NULL, server_addr, SERVER_PORT);
    if (pkt == NULL) {
        puts("Error: cannot create UDP packet");
        gnrc_pktbuf_release(coap);
        return NULL;
    }

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

    /* Parse server IP address */
    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return 1;
    }

    /* Send CoAP request */
    pkt = coap_send_request(netif, &server_addr);
    if (pkt != NULL) {
        gnrc_netif_send(netif, pkt);
        puts("CoAP request sent");
    } else {
        puts("Error: failed to send CoAP request");
    }

    return 0;
}
