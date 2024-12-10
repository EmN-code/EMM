#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

#define URI_PATH "/coap/test"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_STATE_RESP) {
        printf("Response received\n");
    }
    else {
        printf("No response or error\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = COAP_SERVER_PORT
    };

    if (inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6) != 1) {
        puts("Error: unable to parse IPv6 address");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
        return -1;
    }

    puts("CoAP request sent");

    return 0;
}