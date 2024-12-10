#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "coap/test"
#define MESSAGE "Hello, COAP Cloud"

static struct etimer et;

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

PROCESS_THREAD(coap_example_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // Array representing the request
  static coap_blocking_request_state_t request_state;

  PROCESS_BEGIN();

  printf("Starting CoAP Request\n");

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)MESSAGE, sizeof(MESSAGE) - 1);

  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_YIELD();

    if (etimer_expired(&et)) {
      COAP_BLOCKING_REQUEST(&server_ep, request,
                            coap_blocking_request_callback);
      printf("Sent CoAP POST request with message: %s\n", MESSAGE);
      etimer_reset(&et);
    }
  }

  PROCESS_END();
}

void coap_blocking_request_callback(void *response)
{
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}