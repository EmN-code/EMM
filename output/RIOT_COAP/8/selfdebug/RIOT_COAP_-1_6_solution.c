#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define SERVER_PATH "/coap/test"
#define BUFFER_SIZE 128

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu != NULL) {
        printf("Response received.\n");
    } else {
        printf("No response received.\n");
    }
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, SERVER_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len = coap_block1_init(&pdu);

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    return 0;
}