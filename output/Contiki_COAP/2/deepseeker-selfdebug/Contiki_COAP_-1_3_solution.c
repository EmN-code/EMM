
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"

#define SERVER_ENDPOINT "coap://47.102.103.1:5683/coap/test"

static void response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("Request timed out\n");
        return;
    }

    char payload[32];
    size_t len = sizeof(payload);
    if (COAP_RESPONSE_STATUS_OK == coap_get_payload(response, (uint8_t **)&payload, &len)) {
        printf("Received: %s\n", payload);
    }
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
        coap_set_header_uri_path(&request, "coap/test");
        coap_set_payload(&request, "Hello, COAP Cloud", 16);

        COAP_BLOCKING_REQUEST(SERVER_ENDPOINT, &request, response_handler);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
