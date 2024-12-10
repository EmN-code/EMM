
#include <stdio.h>
#include "coap-blocking-request.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Received CoAP response\n");
    // Handle the response here
}

int main(void) {
    coap_endpoint_t server_endpoint;
    coap_request_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, SERVER_IP, SERVER_PORT);

    // Prepare the CoAP request
    coap_request_init(&request, COAP_METHOD_POST, URI_PATH);
    coap_request_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    return 0;
}
