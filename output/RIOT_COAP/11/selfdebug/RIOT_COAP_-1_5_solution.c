#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_METHOD_POST 2  // Assuming POST maps to 2

typedef struct {
    sock_udp_ep_t remote;
    uint8_t buf[128];
} gcoap_params_t;

gcoap_params_t params;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling logic
    printf("Response received\n");
}

int main(void) {
    // Configure the UDP endpoint
    params.remote.family = AF_INET;
    params.remote.netif = SOCK_ADDR_ANY_NETIF;
    params.remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &params.remote.addr.ipv4) != 1) {
        printf("Error: Invalid IPv4 address\n");
        return -1;
    }

    // Initialize the CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, params.buf, sizeof(params.buf), COAP_METHOD_POST, URI_PATH);

    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(params.buf, pdu.payload_len, &params.remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: Failed to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}