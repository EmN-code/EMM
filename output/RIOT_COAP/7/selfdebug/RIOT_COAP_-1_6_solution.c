#include <net/ipv4/addr.h>  // For inet_pton
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"

#define REMOTE_ADDR "47.102.103.1"
#define REMOTE_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // Avoid unused parameter warning
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Setup the payload
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    pdu.payload_len = sizeof(PAYLOAD);

    // Set up the remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = REMOTE_PORT;

    // Convert the IP address from string to network byte order
    if (inet_pton(AF_INET, REMOTE_ADDR, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}