
#include <stdio.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

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
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    remote.port = SERVER_PORT;

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Add payload to the CoAP request
    const char *payload = "Hello, COAP Cloud";
    len = coap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));
    if (len < 0) {
        printf("Failed to add payload to CoAP request\n");
        return 1;
    }

    // Send the CoAP request
    len = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}
