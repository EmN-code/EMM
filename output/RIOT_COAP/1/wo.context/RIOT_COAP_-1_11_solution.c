#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header file for inet_pton function
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"

// Buffer to hold CoAP PDU
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote){
    printf("Response received\n");
    // Additional processing of the response can go here
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Set server IP and port
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Convert IP to binary form
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        fprintf(stderr, "Invalid IP address format\n");
        return -1;
    }

    // Initialize CoAP request
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH) == -1) {
        fprintf(stderr, "Failed to initialize CoAP request\n");
        return -1;
    }

    // Set payload to "Hello, COAP Cloud"
    size_t len = sizeof("Hello, COAP Cloud") - 1;
    memcpy(pdu.payload, "Hello, COAP Cloud", len);
    
    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == -1) {
        fprintf(stderr, "Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}