
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu) {
        printf("Received CoAP response: %d.%02d\n", coap_get_code_class(pdu), coap_get_code_detail(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Set the payload
    coap_pkt_set_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP request
    gcoap_req_send(buf, coap_pkt_get_total_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Sleep to allow the request to be sent and response to be received
    xtimer_sleep(2);

    return 0;
}
