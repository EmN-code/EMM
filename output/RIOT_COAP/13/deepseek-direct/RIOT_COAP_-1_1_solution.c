
#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/udp.h"
#include "net/gnrc/coap.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pkt.h"
#include "net/gnrc/coap/option.h"
#include "net/gnrc/coap/pkt.h"
#include "net/gnrc/coap/req.h"
#include "net/gnrc/coap/resp.h"
#include "net/gnrc/coap/well_known.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static gnrc_netreg_entry_t coap_client_reg;

void coap_client_cb(gnrc_pktsnip_t *pkt, gnrc_netreg_entry_t *entry, kernel_pid_t pid)
{
    gnrc_coap_pkt_t *pdu = gnrc_coap_pkt_init(pkt);
    if (pdu == NULL) {
        puts("Error: could not parse CoAP packet");
        return;
    }

    gnrc_pktbuf_release(pkt);
}

int main(void)
{
    gnrc_netreg_entry_init_pid(&coap_client_reg, GNRC_NETTYPE_COAP, thread_getpid());
    gnrc_netreg_register(GNRC_NETTYPE_COAP, &coap_client_reg);

    gnrc_ipv6_netif_t *netif = gnrc_ipv6_netif_get_by_pid(gnrc_netif_default_pid());
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    ipv6_addr_t server_addr;
    if (ipv6_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: invalid server IP address");
        return 1;
    }

    gnrc_coap_pkt_t pdu;
    gnrc_coap_pkt_init(&pdu, COAP_METHOD_POST, strlen(MESSAGE));
    gnrc_coap_pkt_set_type(&pdu, COAP_TYPE_CON);
    gnrc_coap_pkt_set_code(&pdu, COAP_CODE_201_CREATED);
    gnrc_coap_pkt_set_token(&pdu, (uint8_t *)"token", 5);
    gnrc_coap_pkt_add_option(&pdu, COAP_OPT_URI_PATH, URI_PATH, strlen(URI_PATH));
    gnrc_coap_pkt_set_payload(&pdu, MESSAGE, strlen(MESSAGE));

    gnrc_pktsnip_t *pkt = gnrc_coap_build_pkt(&pdu);
    if (pkt == NULL) {
        puts("Error: could not build CoAP packet");
        return 1;
    }

    gnrc_netif_hdr_t *netif_hdr = gnrc_netif_hdr_build(NULL, 0, (uint8_t *)&server_addr, sizeof(server_addr));
    if (netif_hdr == NULL) {
        puts("Error: could not build network interface header");
        gnrc_pktbuf_release(pkt);
        return 1;
    }

    gnrc_pktsnip_t *udp = gnrc_udp_hdr_build(pkt, netif_hdr, SERVER_PORT, SERVER_PORT);
    if (udp == NULL) {
        puts("Error: could not build UDP header");
        gnrc_pktbuf_release(netif_hdr);
        gnrc_pktbuf_release(pkt);
        return 1;
    }

    gnrc_ipv6_hdr_t *ipv6_hdr = gnrc_ipv6_hdr_build(udp, (uint8_t *)&server_addr, sizeof(server_addr));
    if (ipv6_hdr == NULL) {
        puts("Error: could not build IPv6 header");
        gnrc_pktbuf_release(udp);
        gnrc_pktbuf_release(netif_hdr);
        gnrc_pktbuf_release(pkt);
        return 1;
    }

    gnrc_netapi_send(gnrc_netif_default_pid(), ipv6_hdr);

    xtimer_sleep(1);

    return 0;
}
