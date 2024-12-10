#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "ztimer.h"
#include "gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static uint8_t _buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// This handler is called when a response is received
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received
");
}

int main(void) {
    // Configure the server endpoint
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };

    // Convert string IP address to binary format
    inet_pton(AF_INET, COAP_SERVER_IP, &server.addr.ipv4);

    // Construct the CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, _buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    size_t len = gcoap_response(&pdu, _buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_CODE_EMPTY);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);

    // Send the CoAP request
    ssize_t result = gcoap_req_send(_buf, len + 17, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error sending request: %d\n", result);
    } else {
        printf("Request sent successfully\n");
    }

    return 0;
}