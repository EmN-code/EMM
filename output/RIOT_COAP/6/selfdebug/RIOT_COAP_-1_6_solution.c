#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/gnrc/gcoap.h"
#include <stdio.h>

#define PAYLOAD      "Hello, COAP Cloud"
#define SERVER_IPV4  "47.102.103.1"
#define SERVER_PORT  5683
#define URI_PATH     "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP_RECEIVED) {
        puts("Response received");
    }
    else {
        puts("No response received");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.port = SERVER_PORT;
    
    if (inet_pton(AF_INET, SERVER_IPV4, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server IPv4 address");
        return 1;
    }

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);
    
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("Request sent");

    return 0;
}