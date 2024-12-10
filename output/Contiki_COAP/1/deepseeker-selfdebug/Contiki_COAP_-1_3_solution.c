
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "coap-timer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_blocking_request_state coap_state;

static void coap_response_handler(coap_message_t *response) {
    if (response) {
        printf("CoAP response received\n");
    } else {
        printf("No CoAP response received\n");
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    coap_message_t request;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, COAP_URI_PATH);
        coap_set_payload(&request, "Hello, COAP Cloud", 16);

        COAP_BLOCKING_REQUEST(&coap_state, &request, coap_response_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
