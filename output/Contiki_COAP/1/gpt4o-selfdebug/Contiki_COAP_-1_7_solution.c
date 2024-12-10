#include "sys/rtimer.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include "coap-observe.h"
#include "coap-observe-client.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"

PROCESS(coap_example_client_process, "CoAP Example Client Process");
AUTOSTART_PROCESSES(&coap_example_client_process);

// Constants for the server endpoint
static char *server_uri = "coap://47.102.103.1:5683/coap/test";

// This function will be used as the callback for the CoAP response
static void coap_response_handler(coap_message_t *response) {
  const uint8_t *payload = NULL;
  if (response == NULL) {
    printf("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &payload);
  printf("Response: %*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_example_client_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // Array for handling the CoAP request
  static coap_blocking_request_state_t request_state;

  PROCESS_BEGIN();

  printf("CoAP client process started\n");

  // Configure the server endpoint
  coap_endpoint_parse(server_uri, strlen(server_uri), &server_ep);

  // Initialize the CoAP message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, strlen(msg));

  // Execute the blocking request
  COAP_BLOCKING_REQUEST(&server_ep, request, coap_response_handler);

  printf("Done sending CoAP POST request\n");

  PROCESS_END();
}