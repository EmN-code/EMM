
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;

void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("Request timed out\n");
        return;
    }

    char *payload = NULL;
    size_t payload_len = 0;
    coap_get_payload(response, &payload, &payload_len);

    if(payload_len > 0) {
        printf("Response received: %.*s\n", (int)payload_len, payload);
    } else {
        printf("Empty response received\n");
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    static coap_endpoint_t server_ep;
    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
    server_ep.port = SERVER_PORT;

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5); // Send request every 5 seconds
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_message_t request;
        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, URI_PATH);
        const char *payload = "Hello, COAP Cloud";
        coap_set_payload(&request, payload, strlen(payload));

        coap_blocking_request(&server_ep, &request, response_handler);
    }

    PROCESS_END();
}
