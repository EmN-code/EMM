#include <stdint.h>
#include <stdio.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

// Callback function to handle the CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu == NULL) {
        printf("Error receiving response or timed out\n");
        return;
    }
    printf("Received CoAP response\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];  // Buffer for the CoAP packet
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);

    // Set the IP address and port of the remote CoAP server
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, "47.102.103.1");
    remote.port = CONFIG_GCOAP_PORT;

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("CoAP request error\n");
        return 1;
    }

    printf("CoAP POST request sent successfully\n");
    return 0;
}
