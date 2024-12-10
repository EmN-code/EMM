#include <string.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_PORT 5683
#define SERVER_IP "47.102.103.1"

static gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout response\n");
        return;
    }
    if (pdu->hdr->code == COAP_CODE_204) {
        printf("Success 2.04 Changed\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    /* Set payload */
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    /* Set destination server address */
    sock_udp_ep_t server = { .family = AF_INET, .port = COAP_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf), &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}