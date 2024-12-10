#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "kernel_defines.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE      128
#define COAP_METHOD_POST               2
#define EMQX_URI_PATH                  "/coap/test"
#define EMQX_SERVER_IP                 "47.102.103.1"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle the response here */
    printf("Received response\n");
}

int main(void) {
    sock_udp_ep_t remote;
    size_t len;
    coap_pkt_t pdu;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Invalid IP address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    len = strlen("Hello, COAP Cloud");
    memcpy(buf + sizeof(coap_hdr_t), "Hello, COAP Cloud", len);

    gcoap_req_send(buf, len + sizeof(coap_hdr_t), &remote, _resp_handler, NULL, 0);

    return 0;
}