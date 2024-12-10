#include "contiki.h"
#include "contiki-default-conf.h"
#include "coap-blocking-api.h"

#include <stdio.h>
#include <string.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer timer;
coap_endpoint_t server_ep;
coap_message_t request[1];  // CoAP request

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the server endpoint
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);

  etimer_set(&timer, CLOCK_SECOND * 5);
  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&timer)) {
      // Prepare the request
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      // Send the request
      printf("Sending POST request to coap://47.102.103.1:5683/coap/test\n");
      COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

      // Reset the timer to send another request after some period
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}

/* Note: Ensure that the hardware environment of NRF52840 is configured for Contiki. The networking setup should be correctly aligned with the specified IP and interface configurations. */