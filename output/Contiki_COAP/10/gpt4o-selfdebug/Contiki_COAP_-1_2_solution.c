#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"

#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;

static void init_coap_client(void) {
    etimer_set(&et, CLOCK_SECOND * 30);  // Set a timer to fire every 30 seconds
}

static void send_coap_request(void) {
    coap_endpoint_t server_endpoint;
    coap_message_t request[1];
    uint8_t buf[128];

    // Define the server endpoint address
    coap_endpoint_parse(SERVER_IP, SERVER_PORT, &server_endpoint);

    // Initialize a POST request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);

    // Prepare payload
    const char *payload = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)payload, strlen(payload));

    // Send the request
    int result = coap_send_message(&server_endpoint, request, buf, sizeof(buf));
    if (result < 0) {
        printf("Failed to send CoAP request, error: %d\n", result);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    init_coap_client();
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        send_coap_request();

        // Reset the etimer for the next iteration
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    printf("Starting CoAP client\n");
    process_start(&coap_client_process, NULL);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));
    return 0;
}