#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

void coap_blocking_request_callback(void *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data) {
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    coap_blocking_request_state_t request_state;

    PROCESS_BEGIN();

    // Initialize the CoAP message
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    // Configure the server endpoint
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_ep, request, coap_blocking_request_callback);

    PROCESS_END();
}

int main() {
    process_start(&coap_example_client, NULL);
    while(1) {
        // Main loop of Contiki
        process_run();
    }
    return 0;
}