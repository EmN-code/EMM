#include "contiki-default-conf.h"
#include "coap-request-state.h"
#include "contiki-conf.h"

#include <stdio.h>
#include <string.h>

#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://47.102.103.1:5683"

static void response_handler(coap_message_t *response) {
    const uint8_t *payload;
    int len = coap_get_payload(response, &payload);
    printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];  // This way the packet can be treated as pointer as usual
  
    PROCESS_BEGIN();
    
    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);
    
    printf("Sending CoAP POST request\n");
    
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);
    
    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    PROCESS_END();
}