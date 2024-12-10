
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];
    int32_t offset = 0;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    coap_send_request(&request, &server, coap_response_handler);

    // Main loop
    while (1) {
        PROCESS_YIELD();
    }

    return 0;
}
