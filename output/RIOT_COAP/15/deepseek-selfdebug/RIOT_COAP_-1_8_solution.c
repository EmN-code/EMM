
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
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
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = COAP_SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, coap_pkt_get_total_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
