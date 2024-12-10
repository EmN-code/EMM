#include "os/net/app-layer/coap/coap-engine.h"
#include "os/net/app-layer/coap/coap-blocking-api.h"
#include "os/net/ipv6/uip-ds6.h"
#include "os/net/ipv6/uip-nameserver.h"
#include "os/net/ipv6/uip.h"
#include "contiki.h"
#include "contiki-net.h"
#include <stdio.h>

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];  // This needs to be a single element array!

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

PROCESS_THREAD(coap_client_example, ev, data) {
  PROCESS_BEGIN();

  // Define the server endpoint with the IP address and port
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

  // Initialize the CoAP engine
  coap_init_engine();

  // Prepare the CoAP request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

  // Send the CoAP request
  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

  PROCESS_END();
}

/*
* Ensure the project-conf.h has been configured properly to avoid redefinition errors:
* #define UIP_CONF_TCP 0 to disable TCP over the IP stack
*/