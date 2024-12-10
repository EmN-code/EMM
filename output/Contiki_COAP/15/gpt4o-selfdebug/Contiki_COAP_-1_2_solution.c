#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"
#include "coap-request-state.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void client_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // This way the message can be treated as pointer as usual

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 17);

    COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

    PROCESS_END();
}

int main() {
    printf("Starting Contiki CoAP Client\n");
    process_start(&coap_client_process, NULL);
    PROCESS_WAIT_EVENT_UNTIL(0);
    return 0;
}