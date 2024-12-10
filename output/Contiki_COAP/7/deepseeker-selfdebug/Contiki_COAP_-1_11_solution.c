
#include <stdio.h>
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static struct coap_endpoint_t server_ep;
static coap_message_t request;

static void response_handler(coap_message_t *response) {
    if(response == NULL) {
        printf("No response received\n");
        return;
    }
    char payload[64];
    uint16_t len = sizeof(payload) - 1;
    if(coap_get_payload(response, (uint8_t **)&payload, &len)) {
        payload[len] = '\0';
        printf("Response received: %s\n", payload);
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static struct etimer et;

    PROCESS_BEGIN();

    coap_endpoint_init(&server_ep, COAP_SERVER_IP, COAP_SERVER_PORT, COAP_PROTO_UDP);

    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, COAP_URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 17);

    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

    PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    return 0;
}
