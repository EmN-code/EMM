#include "coap-request-state.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include <string.h>

// Definitions
#define SERVER_EP "coap://47.102.103.1:5683"
#define REQUEST_URI "/coap/test"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

// CoAP response handler callback
static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_blocking_request_state_t request_state;
  static coap_message_t request[1];  // One request storage

  printf("Starting CoAP Client example\n");
  
  // Prepare the request message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_header_uri_path(request, REQUEST_URI);

  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Send CoAP request
  COAP_BLOCKING_REQUEST(SERVER_EP, request, response_handler);

  printf("Request sent\n");

  PROCESS_END();
}