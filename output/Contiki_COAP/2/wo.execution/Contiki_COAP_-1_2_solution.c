#include "contiki.h"
#include "coap-blocking-api.h"
#include "contiki-net.h"
#include "contiki-conf.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define SENSOR_URI "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, SENSOR_URI);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}