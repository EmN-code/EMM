
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu) {
        printf("Received CoAP response\n");
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set payload
    coap_pkt_set_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    gcoap_req_send(&buf[0], coap_pkt_get_total_hdr_len(&pdu) + coap_pkt_get_payload_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}