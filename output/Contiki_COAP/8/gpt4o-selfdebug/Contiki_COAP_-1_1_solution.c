#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1:5683"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static struct etimer timer;
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */
  static char payload[] = "Hello, COAP Cloud";

  PROCESS_BEGIN();

  /* Initialize the REST engine. */
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  /* Prepare the request. */
  etimer_set(&timer, CLOCK_SECOND * 10);
  while(1) {
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    printf("POST Request to %s\n", SERVER_EP);
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)payload, sizeof(payload) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    printf("POST Request sent\n");
    etimer_reset(&timer);
  }

  PROCESS_END();
}