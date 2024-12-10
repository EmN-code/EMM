#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

// CoAP server details
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define PAYLOAD_LEN (sizeof(PAYLOAD) - 1)

// Buffers
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Response handler
static void _resp_handler(const gcoap_request_memo_t *mem, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response (if necessary)
    if (pdu != NULL) {
        printf("Received response: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
    else {
        puts("No response received");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Prepare the remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("inet_pton failed");
        return 1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_PATH) == 0) {
        puts("gcoap_req_init failed");
        return 1;
    }

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, PAYLOAD_LEN);
    pdu.payload_len = PAYLOAD_LEN;

    // Send the request
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP request sent");

    // Main loop can perform other tasks
    while (1) {
        // Do something else, or just wait
    }

    return 0;
}
