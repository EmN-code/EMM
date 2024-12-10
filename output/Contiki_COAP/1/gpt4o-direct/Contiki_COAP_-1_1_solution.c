#include "os/dev/radio/cc2538/cc2538-rf.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-debug.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcpip.h"
#include "net/ip/uip-debug.h"
#include "net/ip/simple-udp.h"
#include "dev/leds.h"
#include "sys/tring-buffer.h"
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT UIP_HTONS(5683)
#define COAP_POST_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

PROCESS(coap_client_example, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_example);

static void response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}

PROCESS_THREAD(coap_client_example, ev, data) {
  PROCESS_BEGIN();
  
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];
  uint8_t more;
  uint32_t res_block;
  uint32_t more_offset;

  printf("CoAP client process started\n");

  /* Initialize the CoAP engine */
  coap_init_engine();

  /* Set server address */
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);
  server_ep.port = SERVER_PORT;

  /* Prepare request */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, COAP_POST_PATH);
  coap_set_payload(request, (uint8_t *)COAP_PAYLOAD, sizeof(COAP_PAYLOAD) - 1);

  printf("Sending CoAP POST request to [%s]:%u%s\n", SERVER_IP, SERVER_PORT, COAP_POST_PATH);

  while (1) {
    COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);

    printf("Sleeping for 10 seconds\n");
    etimer_set(&etimer, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etimer));
  }

  PROCESS_END();
}