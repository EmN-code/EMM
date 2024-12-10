#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "coap-observe.h"

#include <stdio.h>

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];  // Array for request messages

static void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    if(response != NULL) {
        int len = coap_get_payload(response, &payload);
        printf("Response: %.*s\n", len, (char *) payload);
    }
}

PROCESS_THREAD(coap_example_client, ev, data) {
    PROCESS_BEGIN();

    printf("Starting CoAP Example Client\n");
    coap_init_engine();
    // Configure the CoAP server endpoint
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

    // Initiate CoAP request message setup
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);  // Type and code may need adjustment for specific needs
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *) msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}
