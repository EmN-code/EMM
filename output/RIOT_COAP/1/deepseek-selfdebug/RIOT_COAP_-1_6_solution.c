
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"  // Include this header for inet_pton
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    remote.family = AF_INET;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the CoAP request
    gcoap_req_send(&buf[0], pdu.payload_len + pdu.payload_pos, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
