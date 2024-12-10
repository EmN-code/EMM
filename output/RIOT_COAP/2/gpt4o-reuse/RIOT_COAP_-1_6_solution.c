
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include <stdio.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len = gcoap_response(pdu, (uint8_t *)buf, sizeof(buf), COAP_CODE_CONTENT);
    if (len > 0) {
        printf("Received response: %s\n", buf);
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    // Initialize the CoAP request
    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        puts("Error: cannot initialize CoAP request");
        return 1;
    }

    // Add payload to the request
    const char *payload = "Hello, COAP Cloud";
    gcoap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));

    // Send the request
    gcoap_req_send(&buf[0], res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Sleep to allow the request to be sent and response to be received
    xtimer_sleep(2);

    return 0;
}
