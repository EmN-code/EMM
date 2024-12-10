#include <stdio.h>
#include <string.h>
#include "net/nanocoap_sock.h"
#include "net/gnrc/netif.h"
#include "net/gnrc.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"
#define POST_DATA "Hello, COAP Cloud"

static gnrc_netif_t *netif;

static void _send_coap_post(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, NULL, 0);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_string(&pdu, COAP_OPT_URI_PATH, URI_PATH, '/');
    coap_payload_put(&pdu, (uint8_t *)POST_DATA, strlen(POST_DATA));

    ssize_t res = nanocoap_request(&pdu, &remote, buf, sizeof(buf));
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    netif = gnrc_netif_get_by_pid(1);
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    printf("Sending CoAP POST request to %s:%s\n", SERVER_ADDR, SERVER_PORT);
    _send_coap_post();

    return 0;
}