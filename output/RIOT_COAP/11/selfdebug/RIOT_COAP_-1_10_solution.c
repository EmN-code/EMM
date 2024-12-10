#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    // Define necessary variables
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize remote server endpoint
    remote.port = SERVER_PORT;
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Invalid IP address\n");
        return -1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error creating socket.\n");
        return -1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Copy payload into the packet
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));

    // Send the request
    if (gcoap_req_send(buf, strlen(MESSAGE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending request.\n");
        return -1;
    }

    printf("CoAP POST request sent to %s:%d\n", SERVER_IP, SERVER_PORT);
    return 0;
}

// Dummy response handler
tatic void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}