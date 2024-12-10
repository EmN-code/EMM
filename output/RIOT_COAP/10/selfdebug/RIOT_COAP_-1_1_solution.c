#include <stdio.h> // for input and output functions
#include <string.h> // for string handling functions
#include "gcoap.h" // for gcoap related functions and definitions
#include "net/gcoap.h" // for gcoap request and response handling

#define COAP_PORT 5683 // Define the CoAP server port
#define COAP_SERVER "47.102.103.1" // Define the CoAP server IP address
#define COAP_METHOD COAP_METHOD_POST // Define the CoAP request method as POST

int main(void) {
    // Buffer to hold the CoAP request payload
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu; // CoAP PDU (Protocol Data Unit) structure

    // Initialize the CoAP request packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    
    // Define the CoAP payload message
    const char *message = "Hello, COAP Cloud";
    
    // Copy the message payload into the CoAP PDU
    memcpy(pdu.payload, message, strlen(message));
    pdu.payload_len = strlen(message);

    // Specify the destination CoAP server endpoint
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_PORT
    };
    memcpy(&remote.addr.ipv4, &COAP_SERVER, sizeof(remote.addr.ipv4));

    // Send the CoAP request
    ssize_t result = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}

// Response handler for the CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Parse the CoAP response (for simplicity, we will just print a message here)
    printf("Received response from CoAP server\n");
}