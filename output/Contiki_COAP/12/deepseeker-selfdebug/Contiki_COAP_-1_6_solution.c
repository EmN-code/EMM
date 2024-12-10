
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "coap-timer.h"
#include "coap-observe.h"
#include "coap-observe-client.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_blocking_request_state coap_state;

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_request(&coap_state.state, COAP_TYPE_CON, COAP_POST, URI_PATH);
        coap_set_payload(&coap_state.state, "Hello, COAP Cloud", 16);
        coap_blocking_request(&coap_state, SERVER_IP, SERVER_PORT, coap_response_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
