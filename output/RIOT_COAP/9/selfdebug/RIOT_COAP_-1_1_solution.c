#include <string.h>
#include <stdio.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static gcoap_request_memo_t _memo;
static uint8_t _buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        printf("Response received\n");
        printf("PDU Code: %u\n", coap_get_code(pdu));
        printf("Payload: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    size_t len;
    
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    len = gcoap_req_init(&pdu, _buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    len += sizeof(PAYLOAD);

    gcoap_req_send(_buf, len, &remote, _resp_handler, &_memo, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        ; // Main loop does nothing, waits for response
    }

    return 0;
}