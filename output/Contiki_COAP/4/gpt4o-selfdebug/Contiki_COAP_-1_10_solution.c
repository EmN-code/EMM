#include "sys/timer.h"
#include "coap-transactions.h"
#include "coap-transport.h"
#include "coap-timer.h"

#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include <stdio.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

#define SERVER_EP "coap://47.102.103.1:5683"

static coap_endpoint_t server_ep;
static coap_message_t request[1];  // Buffer for the request

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  if (response == NULL) {
    printf("Request timed out or error occurred\n");
    return;
  }
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  printf("Starting CoAP Client Process\n");

  // Initialize the CoAP request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while (1) {
    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
    
    // Wait for some time before sending the next request
    etimer_set(&et, 15 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}