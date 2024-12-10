#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"
#include "netstack-ip.h"
#include "coap-engine.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_endpoint;
  static coap_message_t request[1];

  PROCESS_BEGIN();

  // Set CoAP server's details
  uip_ip6addr(&server_endpoint.ipaddr, 0x2401, 0x0db8, 0x85a3, 0x0000, 0x0000, 0x8a2e, 0x0370, 0x7334);
  server_endpoint.port = UIP_HTONS(5683);

  // Prepare the CoAP POST request
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

  COAP_BLOCKING_REQUEST(&server_endpoint, request, COAP_BLOCKING_REQUEST_CALLBACK);

  PROCESS_END();
}

void COAP_BLOCKING_REQUEST_CALLBACK(void *response)
{
  const uint8_t *chunk;
  int len = coap_get_payload(response, &chunk);
  printf("Response: %.*s\n", len, (char *)chunk);
}