
#include <stdio.h>
#include "net/coap.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Received CoAP response\n");
}

int main(void) {
    coap_endpoint_t server_endpoint;
    coap_message_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, SERVER_IP, SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    return 0;
}