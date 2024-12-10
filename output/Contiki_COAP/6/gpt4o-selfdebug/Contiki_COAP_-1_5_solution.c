#include "coap-request-state.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"

#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;
static coap_blocking_request_state_t request_state;

void coap_chunk_handler(coap_message_t *response) {
    const uint8_t *chunk;
    int len = coap_get_payload(response, &chunk);
    printf("Received: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_endpoint);

    while (1) {
        PROCESS_YIELD();

        if (etimer_expired(&et)) {
            printf("Sending CoAP POST request\n");

            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, URI_PATH);
            const char *msg = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, strlen(msg));

            COAP_BLOCKING_REQUEST(&server_endpoint, request, coap_chunk_handler);

            etimer_reset(&et);
        }
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}