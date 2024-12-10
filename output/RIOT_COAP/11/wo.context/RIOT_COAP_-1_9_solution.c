#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#include "msg.h"
#include "shell.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response handler called\n");
}

int send_coap_request(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int res;

    res = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (res == 0) {
        printf("Malformed IP address\n");
        return -1;
    }
    remote.port = 5683;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    int len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        printf("CoAP request sent\n");
    } else {
        printf("Failed to send CoAP request\n");
        return -2;
    }
    return 0;
}

int main(void) {
    send_coap_request();
    return 0;
}