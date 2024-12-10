#include "contiki-net.h"
#include "coap-observe-client.h"
#include "coap-request-state.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();
  
  static coap_endpoint_t server_endpoint;
  static coap_message_t request;
  static coap_blocking_request_state_t request_state;
  
  // Configure server endpoint
  uip_ip6addr(&server_endpoint.ipaddr, SERVER_IP);
  coap_init_message(&request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_header_uri_path(&request, COAP_URI_PATH);
  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(&request, (uint8_t *)msg, sizeof(msg) - 1);

  COAP_BLOCKING_REQUEST(&server_endpoint, &request, coap_receive);
  
  PROCESS_END();
}

coap_handler_status_t coap_receive(coap_message_t *response) {
  // Handle the response from the server
  return COAP_HANDLER_STATUS_CONTINUE;
}