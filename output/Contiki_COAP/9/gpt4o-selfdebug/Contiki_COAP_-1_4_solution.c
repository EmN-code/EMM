#include "sys/rtimer.h"
#include "contiki.h"
#include "sys/contiki-default-conf.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define COAP_REQUEST_URI "/coap/test"

#include <stdio.h>
#include <string.h>

static struct etimer et;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, COAP_REQUEST_URI);

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    while(1) {
        COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    }

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_process, NULL);
    while(1) {
        process_run();
        etimer_request_poll();
    }
}