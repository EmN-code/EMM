#include "net/sock/util.h"
#include "net/sock/udp.h"
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Defined to prevent undeclared error

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handler logic for incoming CoAP response
    printf("Response received\n");
}

int main(void) {
    // Buffer for the CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Initialize the CoAP message
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Copy the payload
    char PAYLOAD[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Define remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF };
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Send CoAP request
    gcoap_req_send(&buf[0], strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}