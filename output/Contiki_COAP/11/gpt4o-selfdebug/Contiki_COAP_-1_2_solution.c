#include "contiki.h"
#include "coap.h"
#include "coap-blocking-api.h"
#include "coap-engine.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  printf("Starting CoAP client process\n");

  // Define server endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_endpoint);
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Sending the request
  COAP_BLOCKING_REQUEST(&server_endpoint, request, NULL);

  printf("Request sent to CoAP server\n");

  PROCESS_END();
}

#define SERVER_EP "47.102.103.1"