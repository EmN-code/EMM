#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/app-layer/coap-request-state.h"

#include <stdio.h>
#include <string.h>

#define SERVER_EP "[47.102.103.1]:5683"

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1]; /* This way the request packet can be treated as pointer as usual */
static coap_blocking_request_state_t blocking_state;

/*---------------------------------------------------------------------------*/
PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

/*---------------------------------------------------------------------------*/
static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_YIELD();

    if(etimer_expired(&et)) {
      printf("Sending CoAP POST request\n");
      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

      etimer_reset(&et);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/