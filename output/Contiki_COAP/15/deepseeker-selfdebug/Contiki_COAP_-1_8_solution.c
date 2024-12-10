
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t buffer_size, int32_t *offset) {
    // Handle CoAP response here if needed
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;
    uint8_t payload[] = "Hello, COAP Cloud";

    // Initialize CoAP endpoint
    coap_endpoint_init(&server, SERVER_IP, SERVER_PORT);

    // Prepare CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, payload, sizeof(payload) - 1);

    // Send CoAP request
    COAP_BLOCKING_REQUEST(&server, &request, coap_handler);

    return 0;
}
