// Here is a full working example of the required code to send a CoAP POST request from an NRF52840 device using Contiki.

// Import necessary libraries based on the provided documentation in descending alphabetical order by their first letter.
#include "net/ipv6/uip-nameserver.h"
#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include <stdio.h>
#include <string.h>

PROCESS(coap_post_example_process, "CoAP POST Example Process");
AUTOSTART_PROCESSES(&coap_post_example_process);

// Create a buffer for storing the payload
#define PAYLOAD "Hello, COAP Cloud"

STATIC struct etimer et;

PROCESS_THREAD(coap_post_example_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize variables
    static coap_endpoint_t server_ep;
    static coap_message_t request[1];  // This structure percent the request message
    coap_blocking_request_state_t blocking_state;

    // Set timer
    etimer_set(&et, CLOCK_SECOND * 5);

    // Main event loop
    while(1) {
        // Wait for the timer event
        PROCESS_YIELD();

        if (etimer_expired(&et)) {
            // Reset the timer
            etimer_reset(&et);

            // Set the server endpoint (IP address "47.102.103.1", port 5683)
            coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

            // Prepare the request
            coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
            coap_set_header_uri_path(request, "/coap/test");
            coap_set_payload(request, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

            // Send the request
            COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
        }
    }

    PROCESS_END();
}
