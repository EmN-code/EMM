#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-udp-packet.h"

#include <string.h>
#include "contiki.h"
#include "coap-engine.h"


PROCESS(coap_example, "CoAP Example");
AUTOSTART_PROCESSES(&coap_example);

static coap_endpoint_t server_ep;
static coap_message_t request[1];

PROCESS_THREAD(coap_example, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Initialize CoAP engine */
  coap_engine_init();

  /* Define the server address */
  uiplib_ip6addrconv("47.102.103.1", &server_ep.ipaddr);
  server_ep.port = UIP_HTONS(5683);

  /* Initialize the request */
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
  coap_set_header_uri_path(request, "/coap/test");
  const uint8_t msg[] = "Hello, COAP Cloud";
  coap_set_payload(request, (uint8_t *)msg, sizeof(msg) - 1);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    /* Blocking request */
    COAP_BLOCKING_REQUEST(&server_ep, request,
                          COAP_BLOCKING_REQUEST_CALLBACK(NULL));

    printf("Requested message: %s\n", msg);

    etimer_reset(&timer);
  }

  PROCESS_END();
}