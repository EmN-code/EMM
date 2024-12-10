#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Convert server address to binary
    if (inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: invalid server address\n");
        return -1;
    }
    remote.port = SERVER_PORT;

    // Initialize CoAP request packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload data
    const char *message = "Hello, COAP Cloud";
    memcpy(pdu.payload, message, strlen(message));
    pdu.payload_len = strlen(message);

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}