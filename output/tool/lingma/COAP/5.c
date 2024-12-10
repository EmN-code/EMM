#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to handle CoAP response
static void coap_response_handler(const uint8_t *buf, size_t len, void *context) {
    printf("Received CoAP response:\n");
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main(void) {
    // Initialize the network stack
    sock_udp_ep_t server_ep = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };
    inet_pton(AF_INET, COAP_SERVER_IP, &server_ep.addr);

    // Create a UDP socket
    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return 1;
    }

    // Initialize CoAP
    kernel_pid_t coap_pid = gcoap_init();
    if (coap_pid < 0) {
        printf("Failed to initialize CoAP: %d\n", coap_pid);
        return 1;
    }

    // Prepare the CoAP POST request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP POST request
    res = gcoap_req_send(buf, gcoap_pkt_len(&pdu), &server_ep, coap_response_handler, NULL, GCOAP_DEFAULT_PORT);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
        return 1;
    }

    // Wait for the response
    xtimer_sleep(5); // Adjust the sleep time as needed

    // Clean up
    sock_udp_close(&sock);

    return 0;
}