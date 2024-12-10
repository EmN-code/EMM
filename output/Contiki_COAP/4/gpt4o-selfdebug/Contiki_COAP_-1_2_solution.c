#include "contiki.h"
#include "coap-engine.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define URI_PATH "/coap/test"

static void response_handler(coap_message_t *response) {
  const uint8_t *payload;
  if (response == NULL) {
    printf("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &payload);
  printf("Response:\n%.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  printf("Starting CoAP Client\n");

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

  COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

  printf("Request sent\n");

  PROCESS_END();
}