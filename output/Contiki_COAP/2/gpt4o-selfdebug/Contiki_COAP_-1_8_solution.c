#include "contiki-default-conf.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "coap://47.102.103.1"  // Using the server's IP address
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

void response_handler(coap_message_t *response) {
  const uint8_t *payload = NULL;
  if (response == NULL) {
    printf("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &payload);
  printf("Response: %*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  printf("Starting CoAP Client\n");

  coap_endpoint_parse(SERVER_EP, SERVER_PORT, &server_ep);

  while(1) {
    /* Initialize the CoAP engine */
    coap_init_engine();
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);
    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}

