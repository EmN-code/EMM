// Description of the code solution
// 
// The following program demonstrates how to create a simple Contiki application for the NRF52840 that sends a CoAP POST request to a specified CoAP server. 
// The program creates a connection to the CoAP server and sends a "Hello, COAP Cloud" message to the server at IP address "47.102.103.1", port 5683, with the URI path "/coap/test".

// Imports
#include "contiki.h"
#include "coap-engine.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"

#define SERVER_EP "coap://47.102.103.1"

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data)
{
    static struct etimer et;
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_YIELD();

        if (etimer_expired(&et)) {
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, "/coap/test");
            const char msg[] = "Hello, COAP Cloud";
            coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

            COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

            etimer_reset(&et);
        }
    }

    PROCESS_END();
}