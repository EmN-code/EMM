
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

static struct etimer et;
static coap_message_t request;
static coap_endpoint_t server_ep;

PROCESS(coap_post_process, "CoAP POST Process");
AUTOSTART_PROCESSES(&coap_post_process);

static void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

PROCESS_THREAD(coap_post_process, ev, data) {
    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
        coap_set_header_uri_path(&request, URI_PATH);
        const char *payload = "Hello, COAP Cloud";
        coap_set_payload(&request, payload, strlen(payload));

        COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_post_process, NULL);
    return 0;
}