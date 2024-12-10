#include <stdio.h>
#include "net/gcoap.h"
#include "net/gnrc.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received from server\n");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT,
    };
    /* Convert server address to binary (IPv4 address here for simplicity */
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    gcoap_req_send(buf, pdu.payload_len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}