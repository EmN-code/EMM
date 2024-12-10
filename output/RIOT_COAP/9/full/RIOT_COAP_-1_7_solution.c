#include "ztimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_UDP_PORT (5683)
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received: %d\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_UDP_PORT };

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    while (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH) < 0) {
        printf("Error initializing request\n");
    }

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    char payload[] = "Hello, COAP Cloud";
    ssize_t len = coap_payload(&pdu, (uint8_t *)payload, strlen(payload));
    if (len < 0) {
        printf("Error setting payload\n");
        return 1;
    }

    ssize_t res = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
        return 1;
    }

    return 0;
}