#include <string.h>  // for string functions like memcpy, memset
#include <stdio.h>   // for printf

#include "net/gcoap.h"  // required for CoAP functions
#include "net/gnrc/netif.h"  // for network interface information

// Define the CoAP server IP and the port
#define COAP_SERVER_IP    "47.102.103.1"
#define COAP_SERVER_PORT  CONFIG_GCOAP_PORT

// Define the URI path for the POST request
#define COAP_URI_PATH     "/coap/test"

// Define the message to be sent
#define COAP_POST_MESSAGE "Hello, COAP Cloud"

// This function sends a CoAP POST request
static void send_coap_post_request(void) {
    // Buffer for CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Define a CoAP packet
    coap_pkt_t pdu;

    // Initialize the CoAP request with POST method
    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize the CoAP request\n");
        return;
    }

    // Set the payload for the POST request
    memcpy(pdu.payload, COAP_POST_MESSAGE, strlen(COAP_POST_MESSAGE));
    pdu.payload_len = strlen(COAP_POST_MESSAGE);

    // Define the remote address and port
    sock_udp_ep_t remote = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);
    remote.port = COAP_SERVER_PORT;

    // Send the CoAP request
    res = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP POST request sent successfully\n");
    }
}

// Main function
int main(void) {
    printf("Starting CoAP client\n");

    // Send CoAP POST request
    send_coap_post_request();

    // Placeholder for additional code or an endless loop
    while (1) {
        // Simulate some processing or sleep here
    }

    return 0;
}