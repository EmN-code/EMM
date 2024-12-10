#include "net/ipv6/uip-nameserver.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

void client_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    if(response == NULL) {
        printf("Request timed out\n");
        return;
    }
    int len = coap_get_payload(response, &chunk);
    printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    printf("Starting CoAP Client\n");

    coap_endpoint_parse("47.102.103.1:5683", strlen("47.102.103.1:5683"), &server_endpoint);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, client_chunk_handler);

    PROCESS_END();
}