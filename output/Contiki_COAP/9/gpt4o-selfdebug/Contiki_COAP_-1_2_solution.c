#include "sys/timer.h"
#include "sys/etimer.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

PROCESS_THREAD(coap_client_example, ev, data)
{
    static struct etimer et;
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    static char payload[] = "Hello, COAP Cloud";
    
    PROCESS_BEGIN();
    
    etimer_set(&et, CLOCK_SECOND * 10);
    
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        coap_set_payload(request, (uint8_t *)payload, sizeof(payload) - 1);

        COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&coap_client_example, NULL);
    while(1) {
        process_run();
        etimer_request_poll();
    }
    return 0;
}