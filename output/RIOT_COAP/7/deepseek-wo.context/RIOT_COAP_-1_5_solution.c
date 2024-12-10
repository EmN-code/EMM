
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char resp_buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t resp_len = gcoap_response(pdu, (uint8_t *)resp_buf, sizeof(resp_buf), COAP_CODE_CONTENT);

    if (resp_len > 0) {
        printf("Received response: %s\n", resp_buf);
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    remote.port = COAP_SERVER_PORT;

    // Initialize the CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Set the payload
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Send the CoAP request
    ssize_t len = gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    // Keep the thread alive to receive the response
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
