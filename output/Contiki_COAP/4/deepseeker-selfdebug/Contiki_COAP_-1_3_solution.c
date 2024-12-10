
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

static struct coap_link_t resources[1];

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void client_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    if (coap_get_payload(response, &chunk)) {
        printf("Received: %s\n", (char *)chunk);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    etimer_set(&et, CLOCK_SECOND);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        const char *message = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)message, strlen(message));

        COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}
