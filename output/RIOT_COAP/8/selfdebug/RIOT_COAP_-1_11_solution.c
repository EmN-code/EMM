#include <stdio.h>
#include <string.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define ENABLE_DEBUG (1)
#include <debug.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"

// Buffer settings
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Implement a response handler if needed
    printf("Response received\n");
}

int main(void) {
    // Variable declarations
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    ssize_t bytes;
    char payload[] = "Hello, COAP Cloud";

    // Initialize endpoint
    inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6);
    remote.port = COAP_SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send CoAP request
    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Log the result
    if (bytes <= 0) {
        printf("Failed to send request\n");
    } else {
        printf("CoAP request sent: %zd bytes\n", bytes);
    }

    return 0;
}