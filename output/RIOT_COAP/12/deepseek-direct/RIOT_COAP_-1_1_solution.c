
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static gnrc_pktsnip_t *create_coap_message(void)
{
    gnrc_pktsnip_t *pkt, *payload, *coap;
    coap_pkt_t coap_pkt;
    uint8_t *buf;

    /* Create CoAP packet */
    pkt = gnrc_pktbuf_add(NULL, NULL, 64, GNRC_NETTYPE_UNDEF);
    if (pkt == NULL) {
        return NULL;
    }
    buf = pkt->data;

    /* Initialize CoAP packet */
    coap_pkt_init(&coap_pkt, buf, 64, 64);
    coap_pkt_set_version(&coap_pkt, COAP_VERSION_DEFAULT);
    coap_pkt_set_type(&coap_pkt, COAP_TYPE_CON);
    coap_pkt_set_code(&coap_pkt, COAP_METHOD_POST);
    coap_pkt_set_id(&coap_pkt, 0x1234);

    /* Add URI path option */
    coap_opt_add_uri_path(&coap_pkt, URI_PATH);

    /* Add payload */
    payload = gnrc_pktbuf_add(NULL, MESSAGE, strlen(MESSAGE), GNRC_NETTYPE_UNDEF);
    if (payload == NULL) {
        gnrc_pktbuf_release(pkt);
        return NULL;
    }
    coap_pkt_set_payload(&coap_pkt, payload->data, payload->size);

    /* Finalize CoAP packet */
    coap = gnrc_pktbuf_add(pkt, buf, coap_pkt_get_total_hdr_len(&coap_pkt) + coap_pkt_get_payload_len(&coap_pkt), GNRC_NETTYPE_COAP);
    if (coap == NULL) {
        gnrc_pktbuf_release(pkt);
        gnrc_pktbuf_release(payload);
        return NULL;
    }

    return coap;
}

static int send_coap_request(void)
{
    gnrc_pktsnip_t *pkt;
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    int res;

    /* Set remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return -1;
    }

    /* Create CoAP message */
    pkt = create_coap_message();
    if (pkt == NULL) {
        puts("Error: failed to create CoAP message");
        return -1;
    }

    /* Send CoAP message */
    res = gnrc_netapi_dispatch_send(GNRC_NETTYPE_UDP, GNRC_NETREG_DEMUX_CTX_ALL, 0, pkt, &remote);
    if (res < 0) {
        puts("Error: failed to send CoAP message");
        gnrc_pktbuf_release(pkt);
        return -1;
    }

    gnrc_pktbuf_release(pkt);
    return 0;
}

int main(void)
{
    /* Initialize network interface */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Wait for IP address */
    while (ipv6_addr_is_unspecified((ipv6_addr_t *)netif->ipv6.addr[0].addr)) {
        xtimer_sleep(1);
    }

    /* Send CoAP request */
    if (send_coap_request() < 0) {
        return 1;
    }

    puts("CoAP request sent successfully");
    return 0;
}
