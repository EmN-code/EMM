#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gcoap.h"
#include "net/nimble.h"

static gcoap_resp_handler_t _resp_handler(unsigned msg_type,
                                         coap_pkt_t *pkt,
                                         void *cb_args) {
    (void)cb_args;
    if (msg_type == COAP_TYPE_ACK) {
        printf("Response received from server.\n");
    }
    else {
        printf("No ACK received. Type: %d\n", msg_type);
    }
    return 0;
}

int main(void) {
    gnrc_netif_t *netif;
    ipv6_addr_t server_addr;
    coap_pkt_t pkt;
    uint8_t buf[128];
    size_t len;

    printf("RIOT ESP32 CoAP client example\n");

    /* Initialize the IPv6 address */
    ipv6_addr_from_str(&server_addr, "47.102.103.1");

    /* Get the default network interface */
    netif = gnrc_netif_iter(NULL);

    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    /* Initialize the CoAP packet */
    gcoap_pkt_init(&pkt, buf, sizeof(buf), COAP_TYPE_CON, COAP_POST, 0);
    coap_opt_add_format(&pkt, COAP_FORMAT_TEXT);
    gcoap_opt_add_url(&pkt, "/coap/test");
    coap_payload(&pkt, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    len = coap_build(&pkt);

    /* Send a CoAP POST request to the specified server */
    if (gcoap_req_send(buf, len, &server_addr, 5683, _resp_handler, NULL) <= 0) {
        printf("Error: Failed to send CoAP request\n");
    }
    else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}
