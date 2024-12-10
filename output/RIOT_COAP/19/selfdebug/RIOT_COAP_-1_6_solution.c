#include <string.h>   // For memset and memcpy
#include <stdio.h>    // For basic input-output operations
#include <net/sock/udp.h>  // Network operations related to UDP
#include <net/gcoap.h> // Include gcoap header for CoAP

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Buffer size and other configuration parameters
#define BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // Just to avoid unused variable warning
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else {
        // Print the response
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;

    // Convert IP address from string to binary representation
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_hdr_len(&pdu) + strlen(PAYLOAD);
    pdu.payload_len = strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}