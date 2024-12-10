#include "unistd.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_URI_PATH "/coap/test"
#define IPv4_ADDR "47.102.103.1"
#define COAP_PORT 5683

static void _resp_handler(unsigned req_state, coap_pkt_t *pkt, const sock_udp_ep_t *remote);

int main(void) {
    printf("Starting CoAP client example.\n");

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .port = COAP_PORT };
    if (inet_pton(AF_INET, IPv4_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    const char* data = "Hello, COAP Cloud";
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload(&pdu, (uint8_t*)data, strlen(data));

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
        return -1;
    }

    return 0;
}

static void _resp_handler(unsigned req_state, coap_pkt_t *pkt, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: timeout\n");
        return;
    }
    if (req_state == GCOAP_MEMO_ERR) {
        printf("Error: received error\n");
        return;
    }
    printf("Response received, code: %d\n", coap_get_code(pkt));
}