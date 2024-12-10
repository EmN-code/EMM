#include <contiki.h>
#include "coap-engine.h"

/* Callback function to handle server response */
void response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    if(response == NULL) {
        printf("Request timed out\n");
    } else {
        int len = coap_get_payload(response, &chunk);
        printf("Response: %.*s\n", len, (char *)chunk);
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  /* Array of size 1 to represent CoAP message */

    PROCESS_BEGIN();

    /* Initialize the CoAP endpoint */
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    /* Initialize the CoAP message */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

    /* Send request and wait for response */
    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}