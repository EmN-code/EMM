
#include <stdio.h>
#include "coap-blocking-request.h"
#include "coap-request-state.h"
#include "coap-timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "contiki-default-conf.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

int main(void) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request;

    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
    server_endpoint.port = SERVER_PORT;

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_response_handler);

    while (1) {
        PROCESS_YIELD();
    }

    return 0;
}
