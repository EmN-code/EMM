
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/udp-socket.h"
#include "net/coap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }

    char *payload = (char *)coap_get_payload(response);
    if (payload) {
        printf("Response: %s\n", payload);
    } else {
        printf("Empty response payload\n");
    }
}

int main(void) {
    coap_endpoint_t server;
    coap_message_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server, SERVER_IP, SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request and handle the response
    coap_send_request(&request, &server, coap_response_handler);

    // Process events
    while (1) {
        process_poll(&coap_process);
        process_run();
    }

    return 0;
}
