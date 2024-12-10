
#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static struct etimer et;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void client_chunk_handler(coap_message_t *response) {
  const uint8_t *chunk;

  if(response == NULL) {
    printf("Request timed out\n");
    return;
  }

  int len = coap_get_payload(response, &chunk);

  if(len > 0) {
    printf("Response received: %s\n", (char *)chunk);
  }
}

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  // Initialize the CoAP endpoint
  coap_endpoint_parse(SERVER_IP, strlen(SERVER_IP), &server_ep);

  // Prepare the request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 17);

  // Send the request
  COAP_BLOCKING_REQUEST(&server_ep, request, client_chunk_handler);

  PROCESS_END();
}
