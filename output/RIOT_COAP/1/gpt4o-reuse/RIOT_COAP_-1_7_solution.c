
#include <stdio.h>
#include <netinet/in.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *payload = (char *)pdu->payload;
    printf("Received response: %s\n", payload);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    // Set up the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP request
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the request
    const char *payload = "Hello, COAP Cloud";
    len = coap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));
    if (len < 0) {
        printf("Failed to add payload to CoAP request\n");
        return 1;
    }

    // Send the request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Sleep to allow the response to be processed
    xtimer_sleep(2);

    return 0;
}
