#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include "coap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("No response from server\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_payload(&pdu, (uint8_t *)"Hello COAP Cloud", 16);

    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("gcoap_req_send failed\n");
    }

    return 0;
}