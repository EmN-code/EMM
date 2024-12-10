#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
static char *uri_path = "/coap/test";
static const char *payload = "Hello, COAP Cloud";

PROCESS(coap_test_client, "CoAP Test Client");
AUTOSTART_PROCESSES(&coap_test_client);

PROCESS_THREAD(coap_test_client, ev, data)
{
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];  // This way we allocate space on stack

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);

  // prepare the message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, uri_path);
  coap_set_payload(request, (uint8_t *)payload, strlen(payload));

  // declare a state variable to keep the transaction state
  static coap_blocking_request_state_t request_state;

  // send the request and handle the response
  COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

  PROCESS_END();
}

void response_handler(coap_message_t *response)
{
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}