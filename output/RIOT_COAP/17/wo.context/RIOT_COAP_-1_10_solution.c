#include <string.h>
#include <stdio.h>
#include "net/ipv6/addr.h"
#include "gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_resp_handler_t _resp_handler(unsigned code, coap_pkt_t* pdu, void *ctx) {
    (void)ctx;
    printf("Response code: %u\n", code);
    return 0;
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int len;

    /* initializes remote server endpoint */
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}