#include <string.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void on_response(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                        const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu != NULL) {
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    // Copy the payload into the PDU
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    // Set up the remote endpoint
    remote.family = AF_INET;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, on_response, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Failed to send CoAP request\n");
    }

    return 0;
}