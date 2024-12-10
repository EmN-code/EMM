#include "contiki.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static char msg[] = "Hello, COAP Cloud";

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];
  PROCESS_BEGIN();

  coap_endpoint_parse(COAP_SERVER_IP, COAP_SERVER_PORT, &server_endpoint);

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

  PROCESS_END();
}

int main() {
  process_start(&coap_client_process, NULL);
  while(1) {
    process_run();
    etimer_request_poll();
  }
  return 0;
}