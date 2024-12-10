
#include <stdio.h>
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "contiki-default-conf.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_blocking_request_state coap_state;

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_message_t request;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, URI_PATH);
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
