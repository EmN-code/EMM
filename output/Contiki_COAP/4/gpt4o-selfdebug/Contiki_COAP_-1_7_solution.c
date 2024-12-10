#include "coap-separate.h"
#include "coap-observe.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        static coap_endpoint_t server_endpoint;
        static coap_blocking_request_state_t request_state;
        static coap_message_t request[1];  // CoAP message buffer

        // Build the endpoint with the server IP and port
        coap_endpoint_parse(COAP_SERVER_IP, COAP_SERVER_PORT, &server_endpoint);

        // Prepare the CoAP request
        coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
        coap_set_header_uri_path(request, COAP_URI_PATH);

        // Set the payload
        const char *msg = "Hello, COAP Cloud";
        coap_set_payload(request, (uint8_t *)msg, strlen(msg));

        // Send the CoAP request and handle response
        COAP_BLOCKING_REQUEST(&server_endpoint, request, &request_state) {
            coap_receive(&server_endpoint);
        }

        // Reset the timer for next iteration
        etimer_reset(&et);
    }

    PROCESS_END();
}
