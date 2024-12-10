#include "contiki-net.h"
#include "contiki-lib.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // This will hold the request message

  // Set timer for some delay
  etimer_set(&et, CLOCK_SECOND * 5);

  // Parse server endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  // Wait until the timer expires
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  printf("Sending CoAP POST Request\n");

  // Initialize the CoAP message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Perform the CoAP request
  COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

  PROCESS_END();
}

void client_chunk_handler(coap_message_t *response) {
  const uint8_t *chunk;
  size_t len = coap_get_payload(response, &chunk);

  printf("Response: %.*s\n", (int)len, (const char *)chunk);
}