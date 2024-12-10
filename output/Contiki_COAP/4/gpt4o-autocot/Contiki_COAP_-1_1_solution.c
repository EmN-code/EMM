"""
This program is written in C targeting the NRF52840 platform, using Contiki OS to build a CoAP client that sends "Hello, COAP Cloud" as a POST request
to a specified CoAP server. The server has IP address 47.102.103.1 and listens on port 5683, and the request is sent to the URI path "/coap/test".
We are using the CoAP APIs provided by Contiki.
"""

#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#define SERVER_EP "coap://47.102.103.1:5683"

static struct etimer et;
PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data)
{
    PROCESS_BEGIN();

    coap_packet_t request[1];  // This will hold the request
    static coap_endpoint_t server_ep;
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    // Prepare the timer to poll every 5 seconds
    etimer_set(&et, 5 * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, "/coap/test");
        const char msg[] = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

        printf("Requesting %s\n", msg);
        COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}
