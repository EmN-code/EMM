#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static gcoap_req_memo_t req_memo;
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_req_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response status: %u\n", coap_get_code(pdu));
    printf("Response payload: %.*s\n", (int)(pdu->payload_len), (char *)pdu->payload);
}

int main(void) {
    coap_pkt_t pdu;
    ssize_t bytes;
    sock_udp_ep_t remote = { .family = AF_INET,
                             .port = SERVER_PORT };
    
    // Convert IP string to sockaddr
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    // Initialize the CoAP PDU
    bytes = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (bytes < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    // Set payload to "Hello, COAP Cloud"
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    // Send CoAP request
    if (gcoap_req_send(buf, bytes, &remote, _resp_handler, &req_memo, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent to %s\n", SERVER_IP);
    return 0;
}