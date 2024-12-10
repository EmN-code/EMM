
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-endpoint.h"
#include "coap-request-state.h"
#include "sys/etimer.h"
#include "coap-timer.h"
#include "sys/etimer.h"
#include "contiki-default-conf.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

int main(void) {
    coap_endpoint_t server_endpoint;
    coap_request_state_t request_state;

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, SERVER_IP, SERVER_PORT);

    // Prepare the CoAP request
    coap_request_init(&request_state, COAP_METHOD_POST, URI_PATH);
    coap_request_set_payload(&request_state, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request_state, coap_response_handler);

    return 0;
}
