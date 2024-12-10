#include <net/sock/udp.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Check if CONFIG_GCOAP_PDU_BUF_SIZE is already defined
#ifndef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE      (128)
#endif

#define COAP_METHOD_POST 2

int main() {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    // Initialize the remote address
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = 5683;

    // Initialize CoAP request with POST method
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set the payload of the CoAP request
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending gcoap request: %zd\n", res);
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Received Response\n");
}