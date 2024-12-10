#include <arpa/inet.h>  // For inet_pton
#include <string.h>     // For memcpy
#include <stdio.h>      // For printf
#include "net/sock/udp.h"
#include "net/sock.h"
#include "net/ipv4/addr.h"
#include "kernel_defines.h"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Set the remote CoAP server address and port
    remote.family = AF_INET;
    remote.port = 5683;

    // Convert the string IP address to a binary representation
    if(inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        printf("Invalid server IPv4 address\n");
        return 1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}