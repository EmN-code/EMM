#include "contiki.h"
#include <string.h>
#include "coap-engine.h"

// Declaration of CoAP blocking request state 
struct coap_blocking_request_state {
  coap_request_state_t state;
  struct pt pt;
  struct process *process;
} coap_blocking_request_state_t;

#define SERVER_EP "coap://47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH  "coap/test"

#define COAP_BLOCKING_REQUEST(server_endpoint, request, chunk_handler) \
  coap_blocking_request_state_t state; \
  PT_BEGIN(&(state.pt)); \
  coap_init_message((request), COAP_TYPE_CON, COAP_POST, coap_get_mid()); \
  coap_set_header_uri_path((request), (URI_PATH)); \
  coap_set_payload((request), "Hello, COAP Cloud", strlen("Hello, COAP Cloud")); \
  COAP_BLOCKING_REQUEST(&(state.state), (server_endpoint), (request), (chunk_handler));

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

void client_chunk_handler(coap_message_t *response) {
  const uint8_t *chunk;

  if(response == NULL) {
    printf("Request timed out
");
    return;
  }

  int len = coap_get_payload(response, &chunk);

  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];  // Variable to hold request 

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);

  while(1) {
    // Create a new con request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_header_uri_port(request, SERVER_PORT);
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    // Here is where the error was corrected by including correct macro usage
    COAP_BLOCKING_REQUEST(&server_endpoint, request, client_chunk_handler);

    printf("POST request sent\n");

    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
  }

  PROCESS_END();
}