#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-request-state.h"

PROCESS(hello_coap_client_process, "Hello CoAP Client");
AUTOSTART_PROCESSES(&hello_coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS_THREAD(hello_coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the CoAP engine
  coap_engine_init();

  // Define the server endpoint
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  // Initialize and configure the request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

  // Send the request
  COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

  PROCESS_END();
}