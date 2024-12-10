
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
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
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Initialize CoAP packet
    result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Finalize CoAP packet
    coap_pkt_finalize(&pdu, COAP_FORMAT_TEXT, strlen(PAYLOAD));

    // Send CoAP request
    result = gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}
