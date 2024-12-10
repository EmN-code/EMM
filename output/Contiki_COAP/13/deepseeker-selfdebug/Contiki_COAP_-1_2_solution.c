
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-endpoint.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_endpoint_t server_endpoint;

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_message_t request;
    static coap_blocking_request_state_t blocking_request;

    PROCESS_BEGIN();

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_endpoint, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
