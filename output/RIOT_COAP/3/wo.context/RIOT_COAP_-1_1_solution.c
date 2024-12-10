#include <stdio.h>
#include <string.h>
#include <net/gnrc.h>
#include <net/gcoap.h>
#include <net/coap.h>

#define COAP_SERVER_IP  "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu != NULL) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET6, .port = COAP_SERVER_PORT };

    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, COAP_SERVER_IP);

    printf("GCOAP client example\n");

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));

    len += sizeof(COAP_MESSAGE);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}