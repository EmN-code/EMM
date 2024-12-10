#include <string.h>
#include "net/ipv6/uip-nameserver.h"
#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "net/mac/framer/framer.h"
#include "coap-request-state.h"

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

static struct etimer et;
static coap_endpoint_t server_ep;

PROCESS_THREAD(coap_example_client_process, ev, data)
{
    PROCESS_BEGIN();

    // Prepare the server endpoint
    coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Payload to send
        const char *payload = "Hello, COAP Cloud";

        // Construct CoAP request
        static coap_message_t request[1];
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, "/coap/test");
        coap_set_payload(request, (uint8_t *)payload, strlen(payload));

        static coap_blocking_request_state_t request_state;

        // Send request and wait for response
        COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

        etimer_reset(&et);
    }

    PROCESS_END();
}