#include "contiki.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "coap-request-state.h"

#include <stdio.h>

PROCESS(coap_client_process, "CoAP Client Example");
AUTOSTART_PROCESSES(&coap_client_process);

static struct etimer et;

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_blocking_request_state_t request_state;
  static uip_ipaddr_t server_ipaddr;
  
  PROCESS_BEGIN();
  
  uip_ip6addr(&server_ipaddr, 0x2607,0xf8b0,0x4005,0x805::200e);
  
  etimer_set(&et, CLOCK_SECOND * 10);
  printf("CoAP client process started\n");
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    printf("Sending CoAP POST request\n");
    
    COAP_BLOCKING_REQUEST(&server_ipaddr, COAP_REQUEST_POST, "/coap/test",
                          &request_state, "Hello, COAP Cloud");
    
    etimer_reset(&et);
  }
  
  PROCESS_END();
}

int main(void) {
  process_start(&coap_client_process, NULL);
  return 0;
}