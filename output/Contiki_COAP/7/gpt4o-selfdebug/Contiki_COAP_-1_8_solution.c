#include "contiki-default-conf.h"
#include "coap-request-state.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"

#include "contiki.h"
#include "coap-engine.h"

#include <string.h>
#include <stdio.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define POST_URI "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  // Buffer holds the request
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  // Initialize
  PROCESS_BEGIN();

  // Prepare the message
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, POST_URI);
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  // Perform the blocking request
  COAP_BLOCKING_REQUEST(&server_ep, request,
    NULL);

  PROCESS_END();
}