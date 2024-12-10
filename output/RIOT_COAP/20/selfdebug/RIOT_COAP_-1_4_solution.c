#include <string.h>
#include <stdio.h>
#include <gcoap.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_PORT CONFIG_GCOAP_PORT

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int res;

    remote.family = AF_INET;
    remote.port = CONFIG_GCOAP_PORT;

    if ((res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4)) != 1) {
        puts("Error: cannot convert IP address");
        return -1;
    }

    unsigned msg_len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    gcoap_req_send(buf, msg_len + 17, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        puts("Error: timeout");
    }
    else if (pdu->code == COAP_CODE_204) {
        puts("Success: data sent");
    }
    else {
        printf("Error: response code %u\n", pdu->code);
    }
}