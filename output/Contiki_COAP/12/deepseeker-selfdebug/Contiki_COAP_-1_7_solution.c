
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;
static coap_blocking_request_state_t request_state;

void response_handler(coap_pkt_t *pkt, coap_reply_t *reply, int status_code) {
    if (status_code == COAP_NO_ERROR) {
        printf("CoAP request successful!\n");
    } else {
        printf("CoAP request failed with status code: %d\n", status_code);
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_request(&request_state);
        coap_set_request_type(&request_state, COAP_TYPE_CON);
        coap_set_request_method(&request_state, COAP_POST);
        coap_set_request_payload(&request_state, "Hello, COAP Cloud", 16);
        coap_set_request_handler(&request_state, response_handler);

        coap_blocking_request(&request_state, SERVER_IP, SERVER_PORT, URI_PATH);

        etimer_reset(&et);
    }

    PROCESS_END();
}
