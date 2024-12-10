#include "coap-request-state.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"

#include <stdio.h>
#include "uip.h"
#include "coap-engine.h"

static struct etimer timer;
static uint8_t coap_payload[] = "Hello, COAP Cloud";
static uip_ipaddr_t dest_ipaddr;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Set the server address
  uip_ip6addr(&dest_ipaddr, 0x47, 0x102, 0x103, 0x01,0,0,0,0);

  // Initialize CoAP engine
  coap_init_engine();

  // Construct the CoAP POST request
  coap_packet_t request[1];
  coap_init_message(request, COAP_TYPE_CON, COAP_POST, coap_get_mid());
  coap_set_header_uri_path(request, "/coap/test");
  coap_set_payload(request, coap_payload, sizeof(coap_payload) - 1);

  etimer_set(&timer, CLOCK_SECOND * 5);
  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER) {
      printf("Sending CoAP POST request to the server.\n");

      // Send the request
      COAP_BLOCKING_REQUEST(&dest_ipaddr, UIP_HTONS(5683), request, NULL);

      // Restart timer
      etimer_reset(&timer);
    }
  }

  PROCESS_END();
}
