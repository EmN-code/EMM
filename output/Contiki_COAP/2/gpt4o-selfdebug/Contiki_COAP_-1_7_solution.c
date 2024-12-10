#include <stdio.h>
#include "coap-observe-client.h"
#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

#define REMOTE_PORT 5683

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  // Set up server endpoint address
  uip_ip6addr(&server_endpoint.ipaddr, 0x2001, 0x0db8, 0x85a3, 0x0000, 0x0000, 0x8a2e, 0x0370, 0x7334);
  server_endpoint.port = UIP_HTONS(REMOTE_PORT);

  // Initialize CoAP engine
  coap_init_engine();

  // Prepare request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char *msg = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg)-1);

  etimer_set(&et, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    etimer_reset(&et);
    
    uint8_t token[8];
    coap_generate_random_token(token, sizeof(token));
    coap_set_token(request, token, sizeof(token));

    COAP_BLOCKING_REQUEST(&server_endpoint, request,
      COAP_REQUEST_CALLBACK(callback_function));

    printf("CoAP Message Sent\n");
  }

  PROCESS_END();
}

// Callback function for CoAP responses
void callback_function(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}