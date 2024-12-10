#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "contiki-net.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/simple-udp.h"
#include "coap-request-state.h"
#include "stdlib.h"
#include "string.h"
#include "lib/random.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define POST_INTERVAL 5
#define TOGGLE_INTERVAL 10

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // This way the packet can be treated as pointer as usual

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    etimer_set(&et, POST_INTERVAL * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");

    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
  }

  PROCESS_END();
}

void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}