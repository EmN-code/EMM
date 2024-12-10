#include <stdlib.h>  // Standard library definitions
#include <stdio.h>   // Standard I/O definitions
#include <string.h>  // String operations
#include "contiki.h"
#include "contiki-net.h"
// Include additional relevant Contiki and CoAP header files
#include "net/routing/routing.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

PROCESS_THREAD(coap_client_example, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // This way the packet can be treated as pointer as usual
  static struct etimer et;

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    /* Prepare the message */
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    /* Initiate the request */
    COAP_BLOCKING_REQUEST(&server_ep, request, 
                          NULL /* block_handler */);
    printf("Request sent to CoAP server\n");

    /* Wait for a while before sending the next request */
    etimer_set(&et, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}