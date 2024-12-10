#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // This way the packet can be treated as pointer
    PROCESS_BEGIN();

    // Set the server address
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

    // Prepare the request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    // Send the request
    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    while(1) {
        process_run();
        etimer_request_poll();
    }
    return 0;
}