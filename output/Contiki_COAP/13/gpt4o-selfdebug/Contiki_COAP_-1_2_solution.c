#include "contiki.h"
#include "coap-request-state.h"

PROCESS(coap_client_process, "COAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data) {
  static coap_blocking_request_state_t request_state;
  static struct etimer et;
  uip_ipaddr_t server_ip;
  static char payload[] = "Hello, COAP Cloud";
  
  PROCESS_BEGIN();
  
  uip_ip6addr(&server_ip, 0x47,0x102,0x103,0,0,0,0,0x0001);

  etimer_set(&et, CLOCK_SECOND * 10);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    COAP_BLOCKING_REQUEST(&server_ip, 5683, "/coap/test", &request_state, payload, sizeof(payload));
    
    etimer_reset(&et);
  }

  PROCESS_END();
}