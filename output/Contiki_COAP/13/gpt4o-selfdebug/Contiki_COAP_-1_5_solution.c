#include "coap-observe-client.h"
#include "coap-request-state.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include <stdio.h>

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP

#include "net/log.h"

/* CoAP message variables */
static coap_message_t request[1];  /* This way the _request can be treated as pointer as usual */
static coap_endpoint_t server_ep;
static coap_blocking_request_state_t blocking_state;

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();
  
  LOG_INFO("Starting CoAP Client\n");
  
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  LOG_INFO("Sending CoAP POST request to %s\n", SERVER_EP);

  COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

  LOG_INFO("CoAP POST request sent.\n");

  PROCESS_END();
}

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    LOG_WARN("Request timed out\n");
    return;
  }
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  LOG_INFO("Response received: %*s\n", len, (char *)chunk);
}