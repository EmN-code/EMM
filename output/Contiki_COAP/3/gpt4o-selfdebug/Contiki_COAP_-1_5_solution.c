#include "contiki.h"
#include "coap-engine.h"
#include "sys/etimer.h"
#include "net/ipv6/uiplib.h"
#include "net/ip/uip.h"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static coap_endpoint_t server_ep;
static coap_message_t request[1];
static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data)
{
  PROCESS_BEGIN();

  // Setting the server IP address and port
  uip_ip6addr(&server_ep.ipaddr, 0x2f35, 0x529c, 0x0000, 0x2402, 0x1a02, 0x6e11, 0xcd01, 0x4702);
  server_ep.port = UIP_HTONS(5683);

  // Initialize an event timer
  etimer_set(&et, 2 * CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Prepare the CoAP request
    coap_init_message(request, COAP_TYPE_CON, COAP_POST, 0);
    coap_set_header_uri_path(request, "/coap/test");
    coap_set_payload(request, (uint8_t *)"Hello, COAP Cloud", 16);

    // Send the CoAP request
    COAP_BLOCKING_REQUEST(&server_ep, request, NULL);
    
    // Reset the timer
    etimer_reset(&et);
  }
  
  PROCESS_END();
}