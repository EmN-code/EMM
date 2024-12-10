
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu) {
        printf("CoAP response status: %02u\n", coap_get_code(pdu));
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int res;

    // Initialize the remote endpoint
    remote.port = COAP_SERVER_PORT;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP packet
    res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP packet
    coap_put_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));

    // Finish the CoAP packet
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        printf("Failed to finish CoAP packet\n");
        return 1;
    }

    // Send the CoAP request
    res = gcoap_req_send(&buf[0], res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");

    return 0;
}
