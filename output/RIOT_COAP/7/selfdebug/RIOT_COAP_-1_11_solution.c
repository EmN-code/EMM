#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/gcoap.h"

#define SERVER_IP                   "47.102.103.1"
#define SERVER_PORT                 5683
#define COAP_URI_PATH               "/coap/test"
#define COAP_PAYLOAD                "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the server address
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Copy payload into PDU
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request: %d\n", result);
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timed out\n");
    } else {
        printf("Received response\n");
    }
}