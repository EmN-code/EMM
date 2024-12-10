#include "contiki.h"
#include "coap.h"
#include "coap-request-state.h"
#include "string.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    static coap_request_state_t request_state;
    static struct etimer et;

    // Initialize the CoAP library
    coap_init_engine();

    // Set timer
    etimer_set(&et, CLOCK_SECOND * 4);

    while(1) {

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Prepare the server address
        coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_ep);

        // Initialize CoAP request
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(request, URI_PATH);
        const char *msg = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, strlen(msg));

        // Send CoAP request
        COAP_BLOCKING_REQUEST(&server_ep, request,
            request_state.state,
            coap_handle_response);

        etimer_reset(&et);
    }

    PROCESS_END();
}