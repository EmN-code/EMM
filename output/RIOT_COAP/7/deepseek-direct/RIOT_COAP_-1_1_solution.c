
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc.h"
#include "net/sock/udp.h"
#include "net/sock/ip.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static gnrc_pktsnip_t *coap_send_request(gnrc_netif_t *netif, ipv6_addr_t *server_addr, uint16_t port, const char *uri_path, const char *payload)
{
    gnrc_pktsnip_t *pkt, *payload_pkt, *coap_pkt;
    coap_pkt_t coap_pkt_struct;
    uint8_t type = COAP_TYPE_CON;
    uint8_t code = COAP_METHOD_POST;
    uint8_t token[2] = {0x01, 0x02};
    uint8_t token_len = sizeof(token);
    uint8_t options[2] = {COAP_OPT_URI_PATH, COAP_OPT_URI_PATH};
    uint8_t options_len[2] = {strlen(URI_PATH), 0};
    uint8_t *options_data[2] = {(uint8_t *)URI_PATH, NULL};

    payload_pkt = gnrc_pktbuf_add(NULL, payload, strlen(payload), GNRC_NETTYPE_UNDEF);
    if (payload_pkt == NULL) {
        return NULL;
    }

    coap_pkt = gnrc_coap_build(&coap_pkt_struct, type, code, token, token_len, options, options_len, options_data, 1, payload_pkt);
    if (coap_pkt == NULL) {
        gnrc_pktbuf_release(payload_pkt);
        return NULL;
    }

    pkt = gnrc_ipv6_hdr_build(NULL, NULL, server_addr, netif);
    if (pkt == NULL) {
        gnrc_pktbuf_release(coap_pkt);
        return NULL;
    }

    pkt = gnrc_udp_hdr_build(pkt, (uint16_t)port, (uint16_t)port);
    if (pkt == NULL) {
        gnrc_pktbuf_release(coap_pkt);
        return NULL;
    }

    gnrc_pktbuf_add(pkt, coap_pkt->data, coap_pkt->size, GNRC_NETTYPE_COAP);
    gnrc_pktbuf_release(coap_pkt);

    return pkt;
}

int main(void)
{
    gnrc_netif_t *netif = NULL;
    ipv6_addr_t server_addr;
    gnrc_pktsnip_t *pkt;

    puts("CoAP POST request example");

    netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    if (ipv6_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    pkt = coap_send_request(netif, &server_addr, SERVER_PORT, URI_PATH, MESSAGE);
    if (pkt == NULL) {
        puts("Error building CoAP request");
        return 1;
    }

    if (gnrc_netapi_send(netif->pid, pkt) < 1) {
        puts("Error sending CoAP request");
        gnrc_pktbuf_release(pkt);
        return 1;
    }

    gnrc_pktbuf_release(pkt);
    puts("CoAP request sent");

    return 0;
}
