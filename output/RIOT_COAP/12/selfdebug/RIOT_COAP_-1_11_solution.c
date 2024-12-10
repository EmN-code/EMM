// Import the necessary headers
#include "string.h"
#include "net/sock/udp.h"

// Define necessary constants
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Main function
int main(void) {
    // Declare the CoAP buffer and PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    // Initialize socket endpoint structure
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    
    // Convert the IP address and store it in the structure
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    // Initialize the CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    
    // Copy the payload into the PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    
    // Send the CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    return 0;
}

// Response handler function
void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response here
    // This function will be called once a response is received
}
