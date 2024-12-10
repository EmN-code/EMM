
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-endpoint.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response) {
        printf("CoAP response received\n");
    } else {
        printf("No CoAP response received\n");
    }
}

int main(void) {
    coap_endpoint_t server_endpoint;
    coap_message_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Prepare the CoAP request
    coap_request_init(&request, COAP_METHOD_POST, COAP_URI_PATH);
    coap_request_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    return 0;
}
