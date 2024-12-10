// Solution to fix the implicit function declaration errors by including necessary headers
// The headers must be included in descending alphabetical order by their first letter

#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-sr.h"

#include "contiki.h"
#include "sys/log.h"

#include "lib/random.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"  // Added header for COAP_BLOCKING_REQUEST

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Starting CoAP Client\n");

  coap_endpoint_parse("coap://47.102.103.1:5683", strlen("coap://47.102.103.1:5683"), &server_ep);
  coap_init_engine(); // Initialize the CoAP engine

  while(1) {
    LOG_INFO("Sending POST request\n");
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request,
                          NULL); // sending request without a response handler

    PROCESS_PAUSE();
  }

  PROCESS_END();
}