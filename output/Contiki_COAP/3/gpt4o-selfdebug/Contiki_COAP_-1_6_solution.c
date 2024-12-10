#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"

#include "contiki.h"
#include "coap-engine.h"
#include <stdio.h>

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_endpoint;
static coap_message_t request[1];  

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();
  
  printf("CoAP client started\n");
  
  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_endpoint);
  
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const char msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);
  
  COAP_BLOCKING_REQUEST(&server_endpoint, request,
                        NULL);  
  
  printf("Request sent\n");
  
  PROCESS_END();
}