#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "net/ipv6/uip.h"
#include <string.h>

PROCESS(coap_example_client_process, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client_process);

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);

  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_example_client_process, ev, data) {
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */

  PROCESS_BEGIN();

  printf("COAP client started\n");

  /* Initialize the CoAP server endpoint */
  coap_endpoint_parse("coap://[47.102.103.1]:5683", strlen("coap://[47.102.103.1]:5683"), &server_endpoint);

  /* Initialize and configure the message */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_header_uri_path(request, "/coap/test");

  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t*)msg, sizeof(msg) - 1);

  /* Schedule the request to be sent */
  COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

  PROCESS_END();
}