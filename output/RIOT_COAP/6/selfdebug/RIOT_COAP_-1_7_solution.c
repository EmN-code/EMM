#include "net/sock/udp.h"
#include "net/nanocoap.h"
#include "kernel_defines.h"
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from the server
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = CONFIG_GCOAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Convert the IP address string to a byte-represented IP address.
    result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result <= 0) {
        printf("inet_pton() failed\n");
        return -1;
    }

    // Initialize the CoAP request PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Set the payload
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the CoAP request
    ssize_t len = gcoap_req_send(&buf[0], pdu.hdr->len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("gcoap_req_send() failed\n");
        return -1;
    }

    printf("CoAP POST request sent\n");
    return 0;
}