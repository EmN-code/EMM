
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-request.h"
#include "sys/etimer.h"
#include "net/ip/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "lib/random.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;
static struct uip_ipaddr_t server_addr;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

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

PROCESS_THREAD(coap_client_process, ev, data) {
  PROCESS_BEGIN();

  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  uip_ipaddr_t ipaddr;
  uip_ip6addr(&ipaddr, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 0);
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);

  coap_endpoint_connect(&server_ep, &ipaddr, SERVER_PORT);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5 + random_rand() % (CLOCK_SECOND * 5));

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
  }

  PROCESS_END();
}