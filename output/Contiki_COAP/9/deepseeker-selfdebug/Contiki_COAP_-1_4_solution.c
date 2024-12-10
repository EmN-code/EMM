
#include <stdio.h>
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "net/coap.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "sys/etimer.h"
#include "sys/process.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static void coap_response_handler(coap_message_t *response) {
  if(response == NULL) {
    printf("No response received\n");
    return;
  }
  printf("Response received\n");
}

PROCESS_THREAD(coap_client_process, ev, data) {
  static struct etimer et;
  static coap_endpoint_t server;
  static coap_message_t request;

  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    coap_endpoint_init(&server, UIP_HTONL(uiplib_ipaddr_cmp(SERVER_IP)), UIP_HTONS(SERVER_PORT), 0);

    coap_init_message(&request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(&request, URI_PATH);
    coap_set_payload(&request, "Hello, COAP Cloud", 16);

    COAP_BLOCKING_REQUEST(&server, &request, coap_response_handler);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&coap_client_process, NULL);
  return 0;
}
