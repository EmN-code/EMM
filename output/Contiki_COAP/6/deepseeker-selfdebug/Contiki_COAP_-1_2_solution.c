
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"

static struct coap_link_t resource;
static coap_message_t request;
static coap_message_t response;
static uint8_t buffer[128];

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("Request timed out\n");
        return;
    }

    uint16_t len = coap_get_payload(response, &buffer);
    if(len > 0) {
        printf("Response received: %s\n", buffer);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(SERVER_EP, &request, response_handler);

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
