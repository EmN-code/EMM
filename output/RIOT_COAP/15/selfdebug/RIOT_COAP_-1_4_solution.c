#include <string.h>
#include <stdio.h>
#include <net/af.h>
#include <net/sock.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT (5683)
#define EMQX_URI_PATH "/coap/test"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout received");
        return;
    }
    printf("Response %s
", (char *)pdu->payload); 
}

int main(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t bytes;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = EMQX_SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        puts("Error sending CoAP request");
    }

    return 0;
}