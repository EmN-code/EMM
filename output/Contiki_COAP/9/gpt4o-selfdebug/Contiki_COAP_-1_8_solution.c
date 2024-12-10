#include "string.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#define SERVER_EP "coap://47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static coap_endpoint_t server_ep;
static coap_message_t request[1];  // Array used to avoid variable length array warning

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static void response_handler(coap_message_t *response) {
  // Handle the response from the server
  const uint8_t *payload = NULL;
  int len = coap_get_payload(response, &payload);
  printf("Response received: %.*s\n", len, (char *)payload);
}

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize destination endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    // Prepare the CoAP request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_payload(request, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP POST request
    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    // Wait for a specific amount of time before sending again
    PROCESS_WAIT_INTERVAL(CLOCK_SECOND * 10);
  }

  PROCESS_END();
}