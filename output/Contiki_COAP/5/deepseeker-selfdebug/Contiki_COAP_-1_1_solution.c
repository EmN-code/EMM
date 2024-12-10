
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct coap_link_t resource_list;
static struct coap_endpoint_t server_endpoint;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("No response received\n");
        return;
    }
    char payload[32];
    int len = coap_get_payload(response, (uint8_t *)payload);
    if(len > 0) {
        payload[len] = '\0';
        printf("Response received: %s\n", payload);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;
    static coap_message_t request;

    PROCESS_BEGIN();

    // Initialize CoAP endpoint
    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
    server_endpoint.port = SERVER_PORT;

    // Create CoAP request
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    coap_set_payload(&request, (uint8_t *)payload, strlen(payload));

    // Send CoAP request
    COAP_BLOCKING_REQUEST(&server_endpoint, &request, response_handler);

    // Periodic timer to resend the request every 10 seconds
    etimer_set(&et, CLOCK_SECOND * 10);
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        COAP_BLOCKING_REQUEST(&server_endpoint, &request, response_handler);
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
