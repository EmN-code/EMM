// Including necessary headers in descending order based on alphabetical order by first letter
#include "net/sock/udp.h"
#include "net/gcoap.h"

#include <string.h>

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     5683
#define URI_PATH        "/coap/test"
#define PAYLOAD         "Hello, COAP Cloud"

// Buffer for CoAP PDU
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // Suppress unused parameter warning
    (void)remote; // Suppress unused parameter warning

    if (pdu != NULL) {
        printf("Response received!\n");
    }
    else {
        printf("No response received.\n");
    }
}

int main(void) {
    // Setup remote endpoint
    sock_udp_ep_t remote;
    size_t len;
    coap_pkt_t pdu;

    // Initialize remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    len = coap_payload_len(&pdu);

    // Append payload to the packet
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
