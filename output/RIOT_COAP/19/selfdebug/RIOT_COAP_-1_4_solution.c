#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/utils.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define COAP_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, CoAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("timeout received");
    } else if (pdu != NULL) {
        printf("response received: code=%1u.%02u\n",
               coap_get_code_class(pdu), coap_get_code_detail(pdu));
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("inet_pton failed");
        return 1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        puts("gcoap_req_init failed");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("gcoap_req_send failed");
        return 1;
    }

    puts("CoAP request sent");

    return 0;
}
