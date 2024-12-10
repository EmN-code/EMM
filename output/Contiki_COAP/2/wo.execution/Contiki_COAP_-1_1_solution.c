#include "coap.h"
#include "contiki.h"
#include "contiki-net.h"
#include "contiki-conf.h"

PROCESS(coap_post_process, "CoAP POST Request Process");
AUTOSTART_PROCESSES(&coap_post_process);

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_post_process, ev, data) {
  PROCESS_BEGIN();

  coap_endpoint_t server_ep;
  coap_message_t request;
  static coap_blocking_request_state_t request_state;

  printf("Starting CoAP POST Client\n");

  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);
  
  while(1) {
    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, "/coap/test");
    coap_set_payload(&request, (uint8_t *)"Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);

    COAP_BLOCKING_REQUEST(&server_ep, &request, response_handler);

    etimer_set(&(request_state.et), CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&(request_state.et)));
  }

  PROCESS_END();
}