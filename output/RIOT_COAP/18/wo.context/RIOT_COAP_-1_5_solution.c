#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define COAP_PORT 5683
#define BUF_SIZE 128
#define SERVER "47.102.103.1"
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    printf("Starting CoAP example\n");

    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    /* Initialize remote endpoint */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    if (inet_pton(AF_INET, SERVER, &remote.addr.ipv4) != 1) {
        printf("Error in parsing IP\n");
        return 1;
    }

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = gcoap_finish(&pdu, (const uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    /* Send CoAP request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }
    
    return 0;
}