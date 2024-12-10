#include <stdio.h>
#include <stdint.h>
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT (5683)
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static const uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6);

    gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    res = gcoap_req_send((uint8_t *)buf, pdu.payload_len, &remote, _resp_handler, NULL);
    if (res <= 0) {
        printf("[Error] Could not send CoAP request\n");
        return 1;
    }
    printf("CoAP request sent\n");

    return 0;
}