#include <net/sock/udp.h> // Needed for UDP socket
#include <net/ipv4/addr.h> // Needed for IPv4 address handling
#include <string.h> // For manipulating strings and memory
#include <stdio.h>  // Standard I/O functions

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define COAP_MESSAGE "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // Avoid unused parameter warning
    
    if (memo->state != GCOAP_MEMO_TIMEOUT) {
        printf("Response received\n");
    } else {
        printf("Timeout for request\n");
    }

    return 0;
}

int main(void) {
    coap_pkt_t pdu;
    ssize_t result;
    sock_udp_ep_t remote;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    
    // Convert IP address from string to the byte-represented format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting server address\n");
        return -1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);
    
    // Send the CoAP request
    result = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");

    return 0;
}