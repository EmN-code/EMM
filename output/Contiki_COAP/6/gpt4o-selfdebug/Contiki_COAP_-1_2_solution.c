#include "contiki.h"
#include "coap-engine.h"
#include "coap-request-state.h"  // Included for COAP_BLOCKING_REQUEST

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    if (response != NULL) {
        int len = coap_get_payload(response, &payload);
        printf("Response: %.*s\n", len, (char *)payload);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(COAP_SERVER_IP, COAP_SERVER_PORT, &server_endpoint);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_URI_PATH);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}