#include <string.h>
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;
    
    remote.port = 5683;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    char payload[] = "Hello, COAP Cloud";

    int code = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (code < 0) {
        printf("Failed to initialize request\n");
        return 1;
    }

    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}