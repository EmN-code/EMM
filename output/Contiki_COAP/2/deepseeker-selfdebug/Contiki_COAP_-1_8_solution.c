
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include <string.h>
#include <stdio.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define URI_PATH "coap/test"

static struct etimer et;

PROCESS(coap_post_process, "CoAP POST Process");
AUTOSTART_PROCESSES(&coap_post_process);

static void response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }

  const uint8_t *payload;
  size_t payload_len;
  if(coap_get_payload(response, &payload, &payload_len)) {
    printf("Response received: %.*s\n", (int)payload_len, payload);
  }
}

PROCESS_THREAD(coap_post_process, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  etimer_set(&et, CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 17);

  COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

  PROCESS_END();
}
