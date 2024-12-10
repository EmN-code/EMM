#include "contiki.h"
#include "coap-engine.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/routing/routing.h"
#include "coap-blocking-api.h"
#include <stdio.h>

#define SERVER_EP "coap://47.102.103.1"
#define POST_INTERVAL 5

PROCESS(coap_client_process, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];  // Array of one coap_message_t

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  etimer_set(&et, CLOCK_SECOND * POST_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    printf("--Sending POST request to /coap/test\n");

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    const char msg[] = "Hello, COAP Cloud";
    coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    etimer_reset(&et);
  }

  PROCESS_END();
}