#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"

#include <stdio.h>
#include <string.h>

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static void
response_handler(coap_message_t *response)
{
  const uint8_t *payload;

  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  int len = coap_get_payload(response, &payload);
  printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // Buffer for the request

  printf("Starting CoAP Client\n");
  
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  // Initialize the request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Issue the CoAP request
  COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

  PROCESS_END();
}
