#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "coap-request-state.h"

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];  // CoAP request structure

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize CoAP engine
  coap_init_engine();
  
  // Set the server endpoint address
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  // Set a timer to trigger the CoAP POST request
  etimer_set(&et, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      // Prepare the CoAP POST request
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      // Send the CoAP request
      COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

      printf("CoAP POST Request sent\n");

      // Reset the timer
      etimer_reset_with_new_interval(&et, CLOCK_SECOND * 10);
    }
  }

  PROCESS_END();
}
