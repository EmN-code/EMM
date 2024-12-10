#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap/er-coap-engine.h"
#include "coap-observe-client.h"
#include "coap-request-state.h"

static struct etimer timer;
static coap_endpoint_t coap_server_ep;
static coap_message_t request[1]; /* This way the request is out of the way */

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();
  
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &coap_server_ep);

  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_YIELD();
    if(ev == PROCESS_EVENT_TIMER) {
      printf("Sending CoAP POST request\n");
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);
      COAP_BLOCKING_REQUEST(&coap_server_ep, request, NULL);

      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}