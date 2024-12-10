#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT};
    coap_pkt_t pdu;

    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(coap_payload(&pdu), "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // handle error
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    /* Handle CoAP response */
}