
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_endpoint_t server_ep;
static struct coap_message_t request;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("Response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
    server_ep.port = SERVER_PORT;

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_blocking_request(&server_ep, &request, response_handler);

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
