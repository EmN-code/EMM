#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-udp-packet.h"

#include "contiki.h"
#include "coap-blocking-api.h"
#include <stdio.h>

#define SERVER_EP "coap://[47.102.103.1]:5683"

#ifndef COAP_MAX_CHUNK_SIZE
#define COAP_MAX_CHUNK_SIZE 64
#endif

PROCESS(coap_client_process, "CoAP client");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;
static coap_endpoint_t server_ep;
static coap_message_t request[1];      // This way the data is not allocated on the stack

PROCESS_THREAD(coap_client_process, ev, data) {

  PROCESS_BEGIN();

  printf("Starting CoAP client\n");

  etimer_set(&et, CLOCK_SECOND * 5);

  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    PROCESS_YIELD();
    if(etimer_expired(&et)) {

      coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
      coap_set_header_uri_path(request, "/coap/test");
      const char msg[] = "Hello, COAP Cloud";
      coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

      printf("Client sending CoAP request.\n");
      COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

      etimer_reset(&et);
    }
  }

  PROCESS_END();
}

int main() {
    process_start(&coap_client_process, NULL);
    while(1) {
        process_run();
        etimer_request_poll();
    }
}