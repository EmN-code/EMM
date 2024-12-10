#include <stdio.h>
#include "sys/log.h"
#include "coap-engine.h"
#include "contiki-net.h"
#include "contiki.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

/* Log configuration */
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

void response_handler(coap_message_t *response) {
    if (response == NULL) {
        LOG_DBG("Request timed out
");
        return;
    } else {
        LOG_DBG("Received response with code %d
", response->code);
    }
}

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_endpoint);
    server_endpoint.port = UIP_HTONS(SERVER_PORT);

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    const char *msg = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, strlen(msg));

    LOG_DBG("Sending request to %s:%u%s
", SERVER_IP, SERVER_PORT, URI_PATH);

    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}
