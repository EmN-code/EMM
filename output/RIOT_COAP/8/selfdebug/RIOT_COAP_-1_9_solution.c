#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <net/sock/udp.h>
#include <net/gnrc/netif.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT (5683)
#define URI_PATH    "/coap/test"
#define BUF_SIZE    (CONFIG_GCOAP_PDU_BUF_SIZE)

static uint8_t buf[BUF_SIZE];

void send_coap_post(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = gcoap_finish(&pdu, (uint8_t *)"Hello, COAP Cloud", 
                              strlen("Hello, COAP Cloud"));

    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    send_coap_post();
    return 0;
}