#include "uip-ds6-route.h"
#include "uip-ds6-nbr.h"
#include "uip-icmp6.h"
#include "uip-ds6.h"
#include "uip-nameserver.h"
#include "uip-udp-packet.h"
#include "tcp-socket.h"
#include "udp-socket.h"
#include "coap-request-state.h"
#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "coap-blocking-api.h"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Response from server: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

    while(1) {
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        printf("Sending request to COAP server...\n");
        COAP_BLOCKING_REQUEST(&server_endpoint, request, coap_chunk_handler);

        PROCESS_PAUSE();
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}