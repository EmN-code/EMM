
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
    int result;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Initialize the CoAP request
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the request
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the request
    gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Main loop to keep the application running
    while (1) {
        // Process any pending events
        xtimer_sleep(1);
    }

    return 0;
}
