#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include <string.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    int length = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", length, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    coap_endpoint_parse("47.102.103.1:5683", strlen("47.102.103.1:5683"), &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}