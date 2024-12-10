#include <stdio.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/ipv4.h"
#include "net/gnrc/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static gnrc_netif_t *netif;

static void _send_coap_request(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = atoi(SERVER_PORT) };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pkt_init(&pdu, buf, sizeof(buf), 0, COAP_METHOD_POST, COAP_TYPE_CON);
    coap_hdr_set_code(&pdu, COAP_METHOD_POST);
    coap_opt_add_string(&pdu, COAP_OPT_URI_PATH, URI_PATH, '/');
    coap_payload_put(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    ssize_t res = nanocoap_request(&remote, buf, pdu.payload_len + pdu.hdr_len);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    /* Initialize network interface */
    netif = gnrc_netif_get_by_pid(0);
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    /* Send CoAP request */
    _send_coap_request();

    return 0;
}