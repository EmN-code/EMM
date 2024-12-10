
#include <stdio.h>
#include "coap-blocking-request.h"
#include "net/ipv4/uip.h"
#include "net/ipv4/uip-ds6.h"
#include "net/ipv4/uip-udp-packet.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
    if (response == NULL) {
        printf("No response received\n");
        return;
    }
    printf("CoAP response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_endpoint_t server_ep;
    static coap_message_t request;

    PROCESS_BEGIN();

    // Initialize the CoAP endpoint
    coap_endpoint_init(&server_ep, COAP_SERVER_IP, COAP_SERVER_PORT);

    // Create a CoAP POST request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_ep, &request, coap_response_handler);

    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
