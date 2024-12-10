#include "net/ipv6/uip-nameserver.h"
#include "contiki.h"
#include "contiki-net.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "coap-observe.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(hello_world_process, "Hello World");
AUTOSTART_PROCESSES(&hello_world_process);

void response_handler(coap_message_t *response) {
    const uint8_t *payload = NULL;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(hello_world_process, ev, data) {
    static coap_message_t request[1];
    static coap_blocking_request_state_t request_state;

    PROCESS_BEGIN();

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg)-1);

    COAP_BLOCKING_REQUEST(&request_state, SERVER_EP, request, response_handler);

    PROCESS_END();
}
