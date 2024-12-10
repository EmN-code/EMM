#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-request-state.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://47.102.103.1:5683"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

PROCESS_THREAD(coap_example_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // This way the packet can be treated as pointer as usual

  PROCESS_BEGIN();

  // Initialize the CoAP engine
  coap_engine_init();

  // Define server endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  // Prepare the request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)PAYLOAD, sizeof(PAYLOAD)-1);

  // Issue the request
  printf("Client sending request to %s\n", SERVER_EP);
  COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

  PROCESS_END();
}

void client_chunk_handler(coap_message_t *response) {
  const uint8_t *chunk;

  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

int main() {
  process_start(&coap_example_client_process, NULL);
  while (1) {
    PROCESS_WAIT_EVENT();
  }
  return 0;
}