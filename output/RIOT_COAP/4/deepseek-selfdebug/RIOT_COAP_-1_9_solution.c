
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"  // Include this header for inet_pton
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
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Convert IP address from string to binary form
    result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Initialize the CoAP request
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (result < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Finish the CoAP request
    result = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (result < 0) {
        printf("Failed to finish CoAP request\n");
        return 1;
    }

    // Send the CoAP request
    gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
