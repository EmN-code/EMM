#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  const uint8_t *payload = NULL;
  if (response == NULL) {
    printf("Request timed out\n");
    return;
  }

  int len = coap_get_payload(response, &payload);
  printf("Response: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];

  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);

  while (1) {
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    printf("Sending POST request to /coap/test\n");
    COAP_BLOCKING_REQUEST(&server_endpoint, request, response_handler);

    etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}

int main(void) {
  process_start(&coap_client_process, NULL);
  while (1) {
    etimer_request_poll();
  }
  return 0;
}