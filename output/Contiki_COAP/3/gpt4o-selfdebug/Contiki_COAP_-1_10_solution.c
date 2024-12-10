#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"

#define SERVER_EP "coap://[47.102.103.1]:5683"
#define URI_PATH "/coap/test"

PROCESS(coap_post_client_process, "CoAP POST Client");
AUTOSTART_PROCESSES(&coap_post_client_process);

PROCESS_THREAD(coap_post_client_process, ev, data)
{
  static struct etimer timer;
  static coap_endpoint_t server_ep;
  static coap_message_t request[1];  /* This way the packet can be treated as pointer as usual. */

  PROCESS_BEGIN();
  
  etimer_set(&timer, CLOCK_SECOND * 10);
  coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, URI_PATH);
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);

    etimer_reset(&timer);
  }

  PROCESS_END();
}
