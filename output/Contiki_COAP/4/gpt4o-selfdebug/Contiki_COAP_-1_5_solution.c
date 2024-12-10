#include "coap-request-state.h"
#include "coap-observe.h"
#include "coap-observe-client.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-separate.h"

#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"

PROCESS(hello_world_process, "Hello World Process");
AUTOSTART_PROCESSES(&hello_world_process);

static coap_endpoint_t server_ep;
static coap_message_t request_msg;

PROCESS_THREAD(hello_world_process, ev, data) {
    PROCESS_BEGIN();

    printf("CoAP client process started\n");
    coap_init_message(&request_msg, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request_msg, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(&request_msg, (uint8_t *)msg, sizeof(msg) - 1);

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    COAP_BLOCKING_REQUEST(&server_ep, &request_msg, coap_handler_callback);

    printf("CoAP POST request sent\n");

    PROCESS_END();
}

void coap_handler_callback(coap_message_t *response) {
    const uint8_t *payload;
    if (response) {
        int len = coap_get_payload(response, &payload);
        printf("Response: %.*s\n", len, (char *)payload);
    } else {
        printf("Request timed out\n");
    }
}

int main(void) {
    process_init();
    process_start(&hello_world_process, NULL);
    process_run();
    return 0;
}