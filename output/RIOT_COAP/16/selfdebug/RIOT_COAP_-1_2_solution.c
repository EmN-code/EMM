#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout\n");
    } else if (pdu->hdr->code == COAP_CODE_205) {
        printf("gcoap: success, got response\n");
    } else {
        printf("gcoap: error\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    len += sizeof("Hello, COAP Cloud") - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
    }

    return 0;
}