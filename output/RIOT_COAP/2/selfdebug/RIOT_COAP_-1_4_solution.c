#include <string.h>
#include <stdio.h>
#include "net/ipv6/addr.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received\n");
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_t addr;
    ipv6_addr_from_str(&addr, SERVER_IP);
    memcpy(&remote.addr.ipv6, &addr.u8, sizeof(addr.u8));

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending message");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
