
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-endpoint.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "coap-timer.h"
#include "sys/etimer.h"
#include "coap-observe.h"
#include "coap-observe-client.h"

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
    static coap_endpoint_t server_endpoint;
    static coap_message_t request;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Prepare the CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    return 0;
}
