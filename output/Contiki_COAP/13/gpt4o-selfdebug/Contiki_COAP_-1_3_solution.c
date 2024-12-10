#include <contiki.h>
#include <coap-timer.h>
#include <coap-request-state.h>
#include <coap-observe.h>
#include <coap-observe-client.h>

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

static struct etimer et;
static uip_ipaddr_t server_ipaddr;

PROCESS_THREAD(coap_example_client_process, ev, data)
{
    static coap_endpoint_t remote_endpoint;
    static coap_message_t request[1];
    PROCESS_BEGIN();

    // Set the server IP address
    uip_ip6addr(&server_ipaddr, 0x2F, 0x66, 0x67, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01);

    // Convert IPv6 address structure to CoAP endpoint structure
    coap_udp_new(&remote_endpoint, &server_ipaddr, UIP_HTONS(5683));

    // Set the URI path
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    // Initialize the etimer to send requests periodically
    etimer_set(&et, 10 * CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Send the CoAP POST request
        COAP_BLOCKING_REQUEST(&remote_endpoint, request, NULL);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}