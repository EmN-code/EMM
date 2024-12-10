
#include <stdio.h>
#include <string.h>
#include "net/coap/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset) {
    // Handle CoAP response here if needed
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;
    uint8_t payload[] = "Hello, COAP Cloud";
    uint16_t payload_length = sizeof(payload) - 1;

    // Initialize CoAP endpoint
    coap_endpoint_init(&server, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Prepare CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, payload, payload_length);

    // Send CoAP request
    coap_send(&server, &request);

    // Wait for response and handle it
    coap_receive(&server, NULL, 0);

    return 0;
}
