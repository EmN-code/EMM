// main.c implementation
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "stdio.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;

    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout");
        return;
    }
    printf("Response received with code: %u\n", coap_get_code(pdu));
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Resolve server address
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) == 0) {
        puts("Error: unable to parse destination address\n");
        return 1;
    }

    remote.port = SERVER_PORT;

    // Initialize request PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = gcoap_set_payload(pdu.hdr, (uint8_t *)"Hello, COAP Cloud", 17);

    // Send the PDU
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

// Makefile
USEMODULE += gcoap
USEMODULE += gnrc_sock_udp
USEMODULE += xtimer
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif