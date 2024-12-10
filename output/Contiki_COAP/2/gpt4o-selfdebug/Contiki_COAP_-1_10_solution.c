#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-engine.h"
#include <stdio.h>

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];  // Declare the request message

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

PROCESS_THREAD(coap_client_example, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the CoAP engine
  coap_engine_init();

  // Set the server's IP address and port
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

  // Initialize the CoAP message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");

  // Set the payload of the CoAP message
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Send the request
  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

  PROCESS_END();
}
