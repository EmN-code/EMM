#include "coap-request-state.h"
#include "coap-observe-client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contiki.h"
#include "contiki-net.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define TOGGLE_INTERVAL 30

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
    etimer_set(&et, TOGGLE_INTERVAL * CLOCK_SECOND);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&et)) {
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
            coap_set_header_uri_path(request, "/coap/test");

            COAP_BLOCKING_REQUEST(&server_ep, request,
              coap_blocking_request_callback);
            
            etimer_reset(&et);
        }
    }

    PROCESS_END();
}