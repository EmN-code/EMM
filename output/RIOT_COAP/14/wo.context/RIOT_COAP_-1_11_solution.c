#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include "net/gcoap.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received.\n");
    if (memo->state == GCOAP_MEMO_RESP) {
        // Handle the response
    } else {
        printf("Error in receiving response.\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    memset(&remote, 0, sizeof(remote));
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv6);   // Correcting the IP assignment to use IPV6 directly
    remote.port = COAP_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, sizeof(COAP_PAYLOAD));
    gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("Request sent.\n");

    return 0;
}