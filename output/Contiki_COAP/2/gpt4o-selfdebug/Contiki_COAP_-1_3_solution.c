#include "contiki.h"
#include "contiki-net.h"

#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    static struct etimer et;
    uip_ipaddr_t server_ipaddr;
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    // Set the timer interval to 10 seconds
    etimer_set(&et, CLOCK_SECOND * 10);

    // Resolve server address
    uip_ipaddr(&server_ipaddr, 47, 102, 103, 1);
    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_endpoint);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Initialize coap request message
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, URI_PATH);
        coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

        // Send CoAP request
        COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}