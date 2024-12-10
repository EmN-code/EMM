#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "net/gcoap.h"
#include "net/af.h"
#include "net/netif.h"
#include "net/sock/udp.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu->code != COAP_CODE_204) {
        printf("gcoap: msg ID %u, unexpected response code %u\n", coap_get_id(pdu), pdu->code);
    }
    else {
        printf("gcoap: success, code %u\n", pdu->code);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv6);
    if (result != 1) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = strlen("Hello, COAP Cloud");
    memcpy(pdu.payload, "Hello, COAP Cloud", len);
    pdu.payload_len = len;
    gcoap_req_send(buf, pdu.hdr->len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
