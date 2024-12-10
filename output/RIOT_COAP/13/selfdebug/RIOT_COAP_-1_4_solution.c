// Prefix
// This program is designed for the ESP32 platform using RIOT OS to send CoAP POST requests to a CoAP server.
// It aims to address the compilation errors concerning variable redefinitions, implicit function declarations,
// and unused variables.

// Header File Inclusions in Descending Order by First Letter
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/coap.h>

// Global Definitions
#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Properly define this macro only once
#define COAP_SERVER "47.102.103.1"     // Define the CoAP server address
#define COAP_PORT 5683                 // Define the CoAP server port
#define COAP_URI_PATH "/coap/test"     // Define the URI path

// Buffer for the CoAP PDU
uint8_t pdu_buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Function to Send CoAP Request
void send_coap_post_request(void) {
    // Set up UDP endpoint structure
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    // Convert IP address to binary form
    if (inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return;
    }

    // Initialize payload and length
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);

    // Initialize CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, pdu_buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Send CoAP request
    ssize_t bytes_sent = gcoap_req_send(pdu_buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }
}

// Main Function
int main(void) {
    // Initialize the network stack
    printf("Initializing CoAP request and network\n");
    send_coap_post_request();
    return 0;
}