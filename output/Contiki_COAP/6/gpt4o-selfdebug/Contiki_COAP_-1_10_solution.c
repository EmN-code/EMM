#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "coap-request-state.h"
 
static const char *server_ip = "47.102.103.1";
static const uint16_t coap_port = 5683;
static const char *resource_uri = "/coap/test";
static const char *payload_data = "Hello, COAP Cloud";

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  // Initialize CoAP Engine
  coap_init_engine();

  // Parse CoAP Server Endpoint
  coap_endpoint_parse(server_ip, coap_port, &server_endpoint);

  // Initialize CoAP request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, resource_uri);
  coap_set_payload(request, (uint8_t *)payload_data, strlen(payload_data));

  // Send CoAP Request
  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  return 0;
}