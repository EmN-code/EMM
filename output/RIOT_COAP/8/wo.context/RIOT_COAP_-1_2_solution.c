#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"
#define PAYLOAD        "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t server_endpoint = { .family = AF_INET, .port = SERVER_PORT };

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    inet_pton(AF_INET, SERVER_ADDRESS, &server_endpoint.addr.ipv4);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &server_endpoint, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        printf("Error sending request: %d\n", (int)res);
    } else {
        printf("CoAP request sent\n");
    }

    return 0;
}