#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define POST_PAYLOAD "Hello, COAP Cloud"

PROCESS(coap_example_client, "CoAP Example Client");
AUTOSTART_PROCESSES(&coap_example_client);

PROCESS_THREAD(coap_example_client, ev, data) {
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // This way the packet can be treated as pointer as usual
  coap_blocking_request_state_t request_state;

  PROCESS_BEGIN();

  // Initialize the CoAP request and set destination endpoint
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, URI_PATH);
  coap_set_payload(request, (uint8_t *)POST_PAYLOAD, sizeof(POST_PAYLOAD)-1);

  // Set server endpoint
  uip_ip6addr(&server_ep.ipaddr, 0x47, 0x102, 0x103, 0x0, 0, 0, 0, 0x1);
  server_ep.port = UIP_HTONS(SERVER_PORT);

  COAP_BLOCKING_REQUEST(&server_ep, request, coap_response_handler);

  PROCESS_END();
}

void coap_response_handler(coap_message_t *response) {
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}