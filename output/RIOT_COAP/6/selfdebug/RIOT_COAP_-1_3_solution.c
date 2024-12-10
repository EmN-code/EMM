#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static gcoap_request_memo_t _request_memos[GCOAP_REQUEST_CTX_MAX] = {0};
static const uint8_t _memos_size = ARRAY_SIZE(_request_memos);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_STATE_RESPONSE) {
        puts("Received CoAP response");
    }
    else {
        puts("CoAP error or timeout");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = SERVER_PORT };

    remote.addr.ipv6[0] = 0x2f;
    remote.addr.ipv6[1] = 0x66;
    remote.addr.ipv6[2] = 0x67;
    remote.addr.ipv6[3] = 0x01;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}