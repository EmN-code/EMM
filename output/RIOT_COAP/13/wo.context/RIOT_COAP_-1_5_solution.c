#include <arpa/inet.h>  // Include inet_pton declarations
#include <net/ipv4/addr.h>  // Ensure correct networking headers
#include <net/sock/udp.h>
#include <stdio.h>

// Definitions for CoAP configuration
#define GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02  // CoAP POST method
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Buffer and PDU settings
uint8_t buf[GCOAP_PDU_BUF_SIZE];
coap_pkt_t pdu;
sock_udp_ep_t remote;

// Handler for CoAP response
void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received!\n");
}

int main(void) {
    // Initialize remote server endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Convert IP string to a usable format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error parsing server address\n");
        return -1;
    }

    // Initialize a CoAP request
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));  // Set the payload

    // Send the CoAP POST request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}