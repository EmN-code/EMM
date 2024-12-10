#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];  // Array to hold the request structure

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  printf("CoAP client started\n");

  // Configure the server endpoint
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    // Prepare the request message
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    printf("Sending POST request to %s\n", URI_PATH);

    // Send the request
    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    printf("Request sent\n");

    // Wait for some time before sending the next request
    etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}