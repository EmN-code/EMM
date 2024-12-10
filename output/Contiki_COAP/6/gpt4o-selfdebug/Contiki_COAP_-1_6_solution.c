#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include "coap-endpoint.h"
#include <string.h>

// Define constants and types
#define SERVER_EP "coap://47.102.103.1"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define PORT 5683

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_endpoint_t server_ep;
    static coap_message_t request;
    static coap_blocking_request_state_t request_state;
    coap_message_t response;

    PROCESS_BEGIN();

    // Setup the CoAP endpoint
    coap_endpoint_t *ep = coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    if (ep == NULL) {
        printf("Error: could not parse endpoint\n");
        PROCESS_EXIT();
    }

    // Create a CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    etimer_set(&et, CLOCK_SECOND * 2);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Send a blocking CoAP request
    COAP_BLOCKING_REQUEST(&server_ep, &request, coap_receive);

    printf("CoAP request sent to %s\n", SERVER_EP);

    PROCESS_END();
}

void coap_receive(const coap_endpoint_t *src, uint8_t *payload, uint16_t payload_length) {
    printf("Received response: %.*s\n", payload_length, (char *)payload);
}
