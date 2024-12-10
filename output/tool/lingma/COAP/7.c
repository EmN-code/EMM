#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to initialize the network
void network_init(void) {
    // Initialize the network stack (if needed)
    // This might involve setting up the network interface
    // For simplicity, we assume the network is already initialized
}

// Function to send a CoAP POST request
void send_coap_post_request(void) {
    // Create a UDP endpoint for the server
    sock_udp_ep_t server_ep = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT,
        .addr = { .ipv4 = { .u32 = inet_addr(COAP_SERVER_IP) } }
    };

    // Create a buffer for the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload to the CoAP request
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, coap_pkt_len(&pdu), &server_ep, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
    } else {
        printf("CoAP POST request sent successfully\n");
    }
}

int main(void) {
    // Initialize the network
    network_init();

    // Send the CoAP POST request
    send_coap_post_request();

    // Wait for a while before exiting
    xtimer_sleep(5);

    return 0;
}