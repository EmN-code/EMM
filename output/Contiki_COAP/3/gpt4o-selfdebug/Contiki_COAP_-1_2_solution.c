#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"
#include "coap-separate.h"
#include "coap-request-state.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static int response_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *) chunk);
    return len;
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();
    static coap_message_t request[1];  // CoAP request structure
    static coap_endpoint_t server_ep;
    static coap_context_t coap_ctx;
    static coap_blocking_request_state_t request_state;

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_process, NULL);
    while(1) {
        process_run();
        etimer_request_poll();
    }
    return 0;
}